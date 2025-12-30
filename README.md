Compilation (One single command): Run this in the CRC16_DATACOM directory:
g++ -std=c++17 -O2 -o crc16_datacom \
    main.cpp \
    module/crc16.cpp \
    module/manchester.cpp \
    module/bit_utils.cpp \
    sender/sender.cpp \
    receiver/receiver.cpp

How to Run:
You need two terminals:
1. One for the receiver (server)
2. One for the sender (client)

Step 1: Start the Receiver (Server)
In Terminal 1: ./crc16_datacom server 12345 (Or any port you want to use like 5000, 6000)

Expected output:
[Server] Listening on port 12345

Leave this terminal running.


Step 2: Run the Sender (Client) with Test Cases
In Terminal 2, run one test case at a time (replace port if you used a different one).

Test Case 0: No error
./crc16_datacom client 127.0.0.1 12345 0 "A custom message"

Test Case 1: Single bit error
./crc16_datacom client 127.0.0.1 12345 1 "A custom message"

Test Case 2: Two isolated single-bit errors
./crc16_datacom client 127.0.0.1 12345 2 "A custom message"

Test Case 3: Odd number of errors (3 bit flips)
./crc16_datacom client 127.0.0.1 12345 3 "A custom message"

Test Case 4: Burst error of length 8
./crc16_datacom client 127.0.0.1 12345 4 "A custom message"

Test Case 5: Burst error of length 17
./crc16_datacom client 127.0.0.1 12345 5 "A custom message"

Test Case 6: Burst error of length 22
./crc16_datacom client 127.0.0.1 12345 6 "A custom message"


Note: Always use straight double quotes ("), not curly/smart quotes.
      If your message contains '!', use single quotes to avoid shell issues.
Example:
./crc16_datacom client 127.0.0.1 12345 1 'Hello! This works safely'