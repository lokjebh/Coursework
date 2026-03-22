# Socket Chatroom

A multi-user chatroom implemented in Python using both **TCP** and **UDP** protocols. Multiple clients can connect to a server and communicate in real-time.

---

## Files

| File | Description |
|---|---|
| `chatroom.py` | Core library — contains all four classes |
| `server.py` | Runs the TCP server on port 12345 |
| `client.py` | Runs a TCP client (pass a name with `--name`) |

---

## Classes

### TCP
| Class | Role |
|---|---|
| `ServerTCP` | Listens for connections, broadcasts messages, manages clients |
| `ClientTCP` | Connects to the server, sends and receives messages |

### UDP
| Class | Role |
|---|---|
| `ServerUDP` | Single-threaded server; routes all messages via one socket |
| `ClientUDP` | Connects by sending a `join` message; identified by address |

---

## How to Run (TCP)

**Terminal 1 — start the server:**
```bash
python server.py
```

Expected output:
```
TCP CHATROOM
This is the server side.
I am ready to receive connections on port 12345
Press Ctrl+C to shut down the server
Waiting for clients to connect...
```

**Terminal 2, 3, … — connect clients:**
```bash
python client.py --name Alice
python client.py --name Bob
```

Expected output (per client):
```
TCP CHATROOM
This is the client side.
Welcome
You are now connected to the chatroom
Type 'exit' to leave the chatroom

Alice:
```

---

## Usage

| Action | How |
|---|---|
| Send a message | Type text and press Enter at the `Name: ` prompt |
| Leave the chatroom | Type `exit` and press Enter |
| Shut down the server | Press `Ctrl+C` in the server terminal |

---

## Behaviour

- **Duplicate names** — if a client tries to join with a name already in use, the server responds `Name already taken` and the client exits.
- **Client leaves** — all other clients see `User X left`; the server logs it too.
- **Server shuts down** — all clients receive `Server has shut down.` and exit automatically.
- **Incoming messages** — displayed without interrupting the current input prompt.

---

## How to Run (UDP)

Change the imports in `server.py` and `client.py` to use `ServerUDP` / `ClientUDP`:

```python
# server.py
from chatroom import ServerUDP
server = ServerUDP(12345)
server.run()
```

```python
# client.py
from chatroom import ClientUDP
...
client = ClientUDP(args.name, 12345)
client.run()
```

---

## Key Implementation Details

| Concept | Where used | Why |
|---|---|---|
| `select.select()` with 1s timeout | All recv/accept loops | Prevents blocking so threading events can be polled cleanly |
| `threading.Event` | Server & client | Coordinates shutdown across threads without shared mutable state |
| One thread per TCP client | `ServerTCP.handle_client()` | Allows the server to serve multiple clients simultaneously |
| `SO_REUSEADDR` | `ServerTCP.__init__()` | Allows the port to be reused immediately after server restart |
| `os.kill(SIGINT)` | `ClientTCP.receive()` | Unblocks `input()` in the main thread when the server shuts down |
| `daemon=True` threads | Server & client | Threads die automatically when the main process exits |

---

## Requirements

- Python 3.x (no third-party packages required)
