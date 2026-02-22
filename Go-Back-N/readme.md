# Go-Back-N ARQ protocol simulation

## Overview
This project implements a simulated Go-Back-N (GBN) ARQ protocol in Python. It provides a sender and receiver that exchange packets via queues, handles retransmissions on timeout, and reconstructs a file at the receiver.

## Goals
- Practice sliding window protocols
- Implement timeout and retransmission logic
- Simulate packet loss deterministically
- Produce structured, verifiable logs

## Protocol Summary
- Packets carry data bits plus a 16-bit sequence number appended at the end.
- Sender maintains a sliding window `[base, base + window_size)`.
- Receiver only accepts in-order packets and re-ACKs the last in-order packet when out-of-order data arrives.
- Packet loss is simulated by dropping every `nth_packet` once.

## Class Responsibilities
### `GBN_sender`
- Read and packetize the input file into fixed-size bitstrings
- Send packets within the current window
- Track ACKs and advance the window
- Detect timeouts and retransmit window packets

### `GBN_receiver`
- Read packets from the sender queue
- Verify sequence order and send ACKs
- Buffer in-order data and write output when complete

## Running the Test
```bash
python test_gbn.py
```

Expected output:
```
Data transmitted successfully!
```

Logs are written to `simulation.log`.

## Common Pitfalls
- Forgetting to include the 16-bit sequence number in each packet
- Blocking queue operations without a timeout
- Not retransmitting the entire window on timeout
- Writing output before all packets are received

## Practical Upscaling
Example: a simple file transfer utility over UDP where you add CRC checks, adaptive timeouts, and congestion control on top of this GBN logic.
