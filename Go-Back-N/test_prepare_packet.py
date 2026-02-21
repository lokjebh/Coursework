import logging
import queue
from go_back_n import GBN_sender


def build_sender(in_file, packet_len):
    logger = logging.getLogger(f'prepare_packets_test_{packet_len}')
    logger.setLevel(logging.INFO)
    send_queue, ack_queue = queue.Queue(), queue.Queue()
    return GBN_sender(
        input_file=in_file,
        window_size=4,
        packet_len=packet_len,
        nth_packet=3,
        send_queue=send_queue,
        ack_queue=ack_queue,
        timeout_interval=1,
        logger=logger,
    )


def test_empty_file():
    in_file = 'input_prepare_empty.txt'
    with open(in_file, 'w') as f:
        f.write('')

    sender = build_sender(in_file, 32)
    assert sender.packets == [], "Expected no packets for empty input"


def test_small_packet_len_raises():
    in_file = 'input_prepare_small.txt'
    with open(in_file, 'w') as f:
        f.write('A')

    try:
        build_sender(in_file, 16)
    except ValueError:
        return
    raise AssertionError("Expected ValueError when packet_len <= 16")


def test_sequence_bits():
    in_file = 'input_prepare_seq.txt'
    with open(in_file, 'w') as f:
        f.write('AB')

    sender = build_sender(in_file, 20)
    packets = sender.packets
    assert len(packets) == 4, f"Expected 4 packets, got {len(packets)}"

    for seq, packet in enumerate(packets):
        assert len(packet) <= 20, "Packet exceeds packet_len"
        assert packet[-16:] == format(seq, '016b'), "Sequence bits incorrect"


def main():
    test_empty_file()
    test_small_packet_len_raises()
    test_sequence_bits()
    print("prepare_packets edge-case tests passed")


if __name__ == '__main__':
    main()
