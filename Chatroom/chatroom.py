import socket
import threading
import select
import sys
import os
import signal


class ServerTCP:
    def __init__(self, server_port):
        self.server_port = server_port
        # SOCK_STREAM = TCP; creates a reliable, connection-oriented socket
        self.server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        # SO_REUSEADDR lets the server reuse the port immediately after restart,
        # avoiding "Address already in use" errors during development
        self.server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        addr = socket.gethostbyname(socket.gethostname())
        self.server_socket.bind((addr, server_port))
        self.server_socket.listen()  # start accepting incoming TCP connections
        self.clients = {}  # client_socket -> name
        # threading.Event flags allow threads to signal each other cleanly;
        # set() signals stop, is_set() checks the flag, clear() resets it
        self.run_event = threading.Event()     # stops the accept loop in run()
        self.handle_event = threading.Event()  # stops all handle_client() threads

    def accept_client(self):
        # select() makes accept() non-blocking: it returns immediately if no
        # connection is pending (timeout=1s), so run_event can be checked each loop
        readable, _, _ = select.select([self.server_socket], [], [], 1)
        if not readable:
            return False
        try:
            client_socket, addr = self.server_socket.accept()
            # first message from client is their chosen name
            name = client_socket.recv(1024).decode()
            if name in self.clients.values():
                # reject duplicate names before adding to the dict
                client_socket.send('Name already taken'.encode())
                client_socket.close()
                return False
            client_socket.send('Welcome'.encode())
            self.clients[client_socket] = name
            self.broadcast(client_socket, 'join')  # notify existing clients
            print(f'User {name} joined')
            return True
        except Exception:
            return False

    def close_client(self, client_socket):
        try:
            del self.clients[client_socket]
            client_socket.close()
            return True
        except Exception:
            return False

    def broadcast(self, client_socket_sent, message):
        name = self.clients.get(client_socket_sent, '')
        # build the appropriate notification string based on the event type
        if message == 'join':
            text = f'User {name} joined'
        elif message == 'exit':
            text = f'User {name} left'
        else:
            text = f'{name}: {message}'
        # iterate over a snapshot (list()) so dict can be modified by other threads
        # without causing a RuntimeError during iteration
        for sock in list(self.clients):
            if sock != client_socket_sent:  # don't echo back to sender
                try:
                    sock.send(text.encode())
                except Exception:
                    pass  # skip dead connections silently

    def shutdown(self):
        # notify all clients before closing so they can exit gracefully
        for sock in list(self.clients):
            try:
                sock.send('server-shutdown'.encode())
                sock.close()
            except Exception:
                pass
        self.clients.clear()
        # set both events to unblock run() and all handle_client() threads
        self.run_event.set()
        self.handle_event.set()
        try:
            self.server_socket.close()
        except Exception:
            pass
        print('\nServer has shut down.')

    def get_clients_number(self):
        return len(self.clients)

    def handle_client(self, client_socket):
        # runs in its own thread (one per connected client)
        while not self.handle_event.is_set():
            try:
                # select() with 1s timeout prevents recv() from blocking forever,
                # allowing the loop to re-check handle_event each second
                readable, _, _ = select.select([client_socket], [], [], 1)
                if not readable:
                    continue
                message = client_socket.recv(1024).decode()
                # empty string from recv() means the TCP connection was dropped
                if not message or message == 'exit':
                    self.broadcast(client_socket, 'exit')
                    print(f'User {self.clients[client_socket]} left')
                    self.close_client(client_socket)
                    break
                self.broadcast(client_socket, message)
            except Exception:
                self.close_client(client_socket)
                break

    def run(self):
        print('TCP CHATROOM')
        print('This is the server side.')
        print(f'I am ready to receive connections on port {self.server_port}')
        print('Press Ctrl+C to shut down the server')
        print('Waiting for clients to connect...\n')
        try:
            while not self.run_event.is_set():
                accepted = self.accept_client()
                if accepted:
                    # retrieve the socket just added by accept_client()
                    client_socket = list(self.clients.keys())[-1]
                    # daemon=True means the thread dies automatically when the
                    # main thread exits, preventing the process from hanging
                    t = threading.Thread(target=self.handle_client, args=(client_socket,), daemon=True)
                    t.start()
        except KeyboardInterrupt:
            pass
        finally:
            self.shutdown()


class ClientTCP:
    def __init__(self, client_name, server_port):
        self.server_addr = socket.gethostbyname(socket.gethostname())
        self.client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.server_port = server_port
        self.client_name = client_name
        self.exit_run = threading.Event()      # signals the input loop to stop
        self.exit_receive = threading.Event()  # signals the receive thread to stop

    def connect_server(self):
        try:
            self.client_socket.connect((self.server_addr, self.server_port))
            # handshake: send name first, then wait for server's accept/reject
            self.client_socket.send(self.client_name.encode())
            response = self.client_socket.recv(1024).decode()
            print(response)  # prints 'Welcome' or 'Name already taken'
            return 'Welcome' in response
        except Exception:
            return False

    def send(self, text):
        try:
            self.client_socket.send(text.encode())
        except Exception:
            pass

    def receive(self):
        # runs in a background daemon thread concurrently with the input loop
        while not self.exit_receive.is_set():
            try:
                # non-blocking check so exit_receive is polled every second
                readable, _, _ = select.select([self.client_socket], [], [], 1)
                if not readable:
                    continue
                message = self.client_socket.recv(1024).decode()
                if message == 'server-shutdown':
                    print('\nServer has shut down.')
                    self.exit_run.set()
                    self.exit_receive.set()
                    # input() in run() blocks the main thread even after exit_run
                    # is set; sending SIGINT forces it to raise KeyboardInterrupt
                    # so the client exits immediately without waiting for user input
                    os.kill(os.getpid(), signal.SIGINT)
                    break
                # \r clears the current input line, prints the message,
                # then reprints the prompt so the user can keep typing
                sys.stdout.write(f'\r{message}\n{self.client_name}: ')
                sys.stdout.flush()
            except Exception:
                break

    def run(self):
        print('TCP CHATROOM')
        print('This is the client side.')
        if not self.connect_server():
            print('Failed to connect to the chatroom')
            return
        print('You are now connected to the chatroom')
        print("Type 'exit' to leave the chatroom\n")
        # receive() runs concurrently so the user can type and receive simultaneously
        t = threading.Thread(target=self.receive, daemon=True)
        t.start()
        try:
            while not self.exit_run.is_set():
                text = input(f'{self.client_name}: ')
                if self.exit_run.is_set():  # server may have shut down during input()
                    break
                self.send(text)
                if text == 'exit':
                    self.exit_receive.set()
                    break
        except KeyboardInterrupt:
            self.send('exit')
            self.exit_receive.set()
        finally:
            self.client_socket.close()


class ServerUDP:
    def __init__(self, server_port):
        self.server_port = server_port
        # SOCK_DGRAM = UDP; connectionless — no accept(), no per-client socket
        self.server_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        addr = socket.gethostbyname(socket.gethostname())
        self.server_socket.bind((addr, server_port))
        self.clients = {}   # client_addr (ip, port) -> name
        # messages stores (sender_addr, text) tuples; broadcast() reads the last entry
        self.messages = []  # list of (client_addr, message_str)

    def accept_client(self, client_addr, message):
        # UDP has no real "connection"; we simulate it by tracking client_addr
        # message format from ClientUDP.send(): '{name}: join'
        name = message.split(': ')[0]
        if name in self.clients.values():
            self.server_socket.sendto('Name already taken'.encode(), client_addr)
            return False
        self.server_socket.sendto('Welcome'.encode(), client_addr)
        self.clients[client_addr] = name
        self.messages.append((client_addr, f'User {name} joined'))
        self.broadcast()
        return True

    def close_client(self, client_addr):
        try:
            name = self.clients[client_addr]
            del self.clients[client_addr]
            self.messages.append((client_addr, f'User {name} left'))
            self.broadcast()
            return True
        except Exception:
            return False

    def broadcast(self):
        if not self.messages:
            return
        # always broadcast the most recently appended message
        sender_addr, msg = self.messages[-1]
        for addr in list(self.clients):
            if addr != sender_addr:  # don't echo back to sender
                try:
                    self.server_socket.sendto(msg.encode(), addr)
                except Exception:
                    pass

    def shutdown(self):
        # UDP has no connection to close per client; just send the signal
        for addr in list(self.clients):
            try:
                self.server_socket.sendto('server-shutdown'.encode(), addr)
            except Exception:
                pass
        # copy keys first because close_client() modifies the dict
        for addr in list(self.clients.keys()):
            self.close_client(addr)
        try:
            self.server_socket.close()
        except Exception:
            pass

    def get_clients_number(self):
        return len(self.clients)

    def run(self):
        print(f'UDP Server started on port {self.server_port}')
        try:
            while True:
                # select() prevents recvfrom() from blocking forever;
                # UDP server is single-threaded — all routing happens here
                readable, _, _ = select.select([self.server_socket], [], [], 1)
                if not readable:
                    continue
                data, addr = self.server_socket.recvfrom(1024)
                message = data.decode()
                # all UDP messages arrive on the same socket, identified by addr;
                # format is always '{name}: {text}' (set by ClientUDP.send())
                parts = message.split(': ', 1)
                text = parts[1] if len(parts) > 1 else ''
                if text == 'join':
                    self.accept_client(addr, message)
                elif text == 'exit':
                    self.close_client(addr)
                else:
                    if addr in self.clients:
                        name = self.clients[addr]
                        self.messages.append((addr, f'{name}: {text}'))
                        self.broadcast()
        except KeyboardInterrupt:
            pass
        finally:
            self.shutdown()


class ClientUDP:
    def __init__(self, client_name, server_port):
        self.server_addr = socket.gethostbyname(socket.gethostname())
        # SOCK_DGRAM = UDP; no connect() needed — sendto() specifies the destination each time
        self.client_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.server_port = server_port
        self.client_name = client_name
        self.exit_run = threading.Event()
        self.exit_receive = threading.Event()

    def connect_server(self):
        try:
            # UDP has no real connection; "joining" just means sending 'join'
            # and confirming the server recognises us
            self.send('join')
            # wait up to 5s for the server's Welcome/rejection reply
            readable, _, _ = select.select([self.client_socket], [], [], 5)
            if not readable:
                return False
            response, _ = self.client_socket.recvfrom(1024)
            return 'Welcome' in response.decode()
        except Exception:
            return False

    def send(self, text):
        try:
            # prefix every message with the client name so the server knows who sent it
            message = f'{self.client_name}: {text}'
            self.client_socket.sendto(message.encode(), (self.server_addr, self.server_port))
        except Exception:
            pass

    def receive(self):
        # background thread: listens for broadcasts from the server
        while not self.exit_receive.is_set():
            try:
                readable, _, _ = select.select([self.client_socket], [], [], 1)
                if not readable:
                    continue
                data, _ = self.client_socket.recvfrom(1024)
                message = data.decode()
                if message == 'server-shutdown':
                    print('\nServer has shut down.')
                    self.exit_run.set()
                    self.exit_receive.set()
                    break
                sys.stdout.write(f'\r{message}\n')
                sys.stdout.flush()
            except Exception:
                break

    def run(self):
        if not self.connect_server():
            print('Failed to connect to server.')
            return
        print(f'Connected as {self.client_name}. Type "exit" to leave.')
        # receive() runs concurrently so sending and receiving happen simultaneously
        t = threading.Thread(target=self.receive, daemon=True)
        t.start()
        try:
            while not self.exit_run.is_set():
                text = input()
                if self.exit_run.is_set():
                    break
                self.send(text)
                if text == 'exit':
                    self.exit_receive.set()
                    break
        except KeyboardInterrupt:
            self.send('exit')
            self.exit_receive.set()
        finally:
            self.client_socket.close()
