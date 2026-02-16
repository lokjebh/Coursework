class GBN_sender:
    """Sender side of the Go-Back-N protocol."""
    def __init__(self, input_file, window_size, packet_len, nth_packet, send_queue, ack_queue, timeout_interval, logger):
        """Initialize sender state and prepare packet metadata."""
        self.input_file = input_file
        self.window_size = window_size
        self.packet_len = packet_len
        self.nth_packet = nth_packet
        self.send_queue = send_queue
        self.ack_queue = ack_queue
        self.timeout_interval = timeout_interval
        self.logger = logger

        self.base = 0
        self.packets = self.prepare_packets()
        # One flag per packet to track whether its ACK has been received.
        self.acks_list = [False] * len(self.packets)
        # One timer slot per packet (None until started).
        self.packet_timers = [None] * len(self.packets)
        self.dropped_list = []

    def prepare_packets(self):
        """Read input file and split it into encoded packets."""
        with open(self.input_file, 'r') as f:
            data = f.read()

        bitstream = ''.join(format(ord(ch), '08b') for ch in data)
        data_bits_len = self.packet_len - 16
        if data_bits_len <= 0:
            raise ValueError("packet_len must be greater than 16 to fit data bits")

        packets = []
        if not bitstream:
            return packets

        seq_num = 0
        for i in range(0, len(bitstream), data_bits_len):
            chunk = bitstream[i:i + data_bits_len]
            seq_bits = format(seq_num, '016b')
            packets.append(chunk + seq_bits)
            seq_num += 1

        return packets

    def send_packets(self):
        """Send all packets currently within the sliding window."""
        pass

    def send_next_packet(self):
        """Advance the window and send the next packet."""
        pass

    def send_packets(self):
        """Send all packets currently within the sliding window."""
        pass

    def send_next_packet(self):
        """Advance the window and send the next packet."""
        pass

    def check_timers(self):
        """Check for timeouts within the current window."""
        pass

    def receive_acks(self):
        """Receive ACKs and advance the window accordingly."""
        pass

    def run(self):
        """Main sender loop: send, receive ACKs, and handle timeouts."""
        pass

class GBN_receiver:
    """Receiver side of the Go-Back-N protocol."""
    def __init__(self, output_file, send_queue, ack_queue, logger):
        """Initialize receiver state and output destination."""
        self.output_file = output_file
        self.send_queue = send_queue
        self.ack_queue = ack_queue
        self.logger = logger

        self.packet_list = []
        self.expected_seq_num = 0

    def process_packet(self, packet):
        """Process a single incoming packet and send ACKs as needed."""
        pass

    def write_to_file(self):
        """Write the reconstructed data to the output file."""
        pass

    def run(self):
        """Main receiver loop: read packets and finalize output."""
        pass
