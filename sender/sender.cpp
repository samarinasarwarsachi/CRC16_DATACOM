#include <bits/stdc++.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "../module/crc16.hpp"
#include "../module/manchester.hpp"
#include "../module/bit_utils.hpp"
#include "../module/socket_utils.hpp"

using namespace std;

int run_client(const string& server_ip, int port, int testcase, const string& payload_str) {
    vector<uint8_t> payload(payload_str.begin(), payload_str.end());
    
    vector<uint8_t> msg = payload;
    uint16_t crc = crc16_ccitt(payload);
    msg.push_back((uint8_t)((crc >> 8) & 0xFF));
    msg.push_back((uint8_t)(crc & 0xFF));
    
    vector<int> data_bits = bytes_to_bits(msg);

    vector<int> chan_bits = manchester_encode(data_bits);
    
    vector<int> tx_bits = chan_bits;
    size_t N = tx_bits.size();
    
    auto pick_safe_pos = [&](size_t prefer_start, size_t len) -> size_t {
        if (N == 0) return 0;
        if (prefer_start + len <= N) return prefer_start;
        if (len >= N) return 0;
        return N / 4;
    };
    
    switch (testcase) {
        case 0:
            cerr << "[Client] Testcase 0: no error\n";
            break;
        case 1: {
            cerr << "[Client] Testcase 1: single bit error\n";
            size_t p = N / 3;
            flip_bits(tx_bits, {p});
            cerr << "[Client] Flipped bit at position " << p << " (of " << N << " channel bits)\n";
            break;
        }
        case 2: {
            cerr << "[Client] Testcase 2: two isolated single-bit errors\n";
            size_t p1 = N / 4;
            size_t p2 = N * 3 / 4;
            flip_bits(tx_bits, {p1, p2});
            cerr << "[Client] Flipped bits at " << p1 << " and " << p2 << "\n";
            break;
        }
        case 3: {
            cerr << "[Client] Testcase 3: odd number (3) of isolated bit flips\n";
            size_t p1 = N / 5, p2 = N / 2, p3 = N * 4 / 5;
            flip_bits(tx_bits, {p1, p2, p3});
            cerr << "[Client] Flipped bits at " << p1 << "," << p2 << "," << p3 << "\n";
            break;
        }
        case 4: {
            cerr << "[Client] Testcase 4: burst error length 8\n";
            size_t start = pick_safe_pos(10, 8);
            flip_burst(tx_bits, start, 8);
            cerr << "[Client] Flipped burst starting at " << start << " len=8\n";
            break;
        }
        case 5: {
            cerr << "[Client] Testcase 5: burst error length 17\n";
            size_t start = pick_safe_pos(20, 17);
            flip_burst(tx_bits, start, 17);
            cerr << "[Client] Flipped burst starting at " << start << " len=17\n";
            break;
        }
        case 6: {
            cerr << "[Client] Testcase 6: burst error length 22\n";
            size_t start = pick_safe_pos(30, 22);
            flip_burst(tx_bits, start, 22);
            cerr << "[Client] Flipped burst starting at " << start << " len=22\n";
            break;
        }
        default:
            cerr << "[Client] Unknown testcase " << testcase << ", sending no error\n";
    }

    uint32_t len = static_cast<uint32_t>(tx_bits.size());
    uint32_t netlen = htonl(len);
    vector<uint8_t> outbuf;
    outbuf.resize(4 + len);
    memcpy(outbuf.data(), &netlen, 4);
    for (size_t i = 0; i < len; ++i)
        outbuf[4 + i] = '0' + tx_bits[i];
    
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        return 1;
    }
    
    sockaddr_in serv{};
    serv.sin_family = AF_INET;
    serv.sin_port = htons(port);
    if (inet_pton(AF_INET, server_ip.c_str(), &serv.sin_addr) <= 0) {
        perror("inet_pton");
        close(sock);
        return 2;
    }
    
    if (connect(sock, (sockaddr*)&serv, sizeof(serv)) < 0) {
        perror("connect");
        close(sock);
        return 3;
    }
    
    cerr << "[Client] Connected to " << server_ip << ":" << port << ", sending " << len << " channel bits\n";
    
    if (!send_all(sock, outbuf.data(), outbuf.size())) {
        cerr << "[Client] send failed\n";
        close(sock);
        return 4;
    }
    
    cerr << "[Client] Data sent. Closing.\n";
    close(sock);
    return 0;
}