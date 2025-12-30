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

int run_server(int port) {
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0) { perror("socket"); return 1; }
    
    int opt = 1;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);
    
    if (::bind(listenfd, (sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind");
        close(listenfd);
        return 2;
    }

    if (listen(listenfd, 1) < 0) {
        perror("listen");
        close(listenfd);
        return 3;
    }
    
    cerr << "[Server] Listening on port " << port << "\n";
    
    sockaddr_in cli{};
    socklen_t clilen = sizeof(cli);
    int conn = accept(listenfd, (sockaddr*)&cli, &clilen);

    if (conn < 0) {
        perror("accept");
        close(listenfd);
        return 4;
    }
    
    char cip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &cli.sin_addr, cip, INET_ADDRSTRLEN);
    cerr << "[Server] Accepted connection from " << cip << ":" << ntohs(cli.sin_port) << "\n";
    
    uint8_t lenbuf[4];
    if (!recv_all(conn, lenbuf, 4)) {
        cerr << "[Server] Failed to read length\n";
        close(conn);
        close(listenfd);
        return 5;
    }
    
    uint32_t netlen;
    memcpy(&netlen, lenbuf, 4);
    uint32_t len = ntohl(netlen);
    cerr << "[Server] Expecting " << len << " channel bits\n";
    
    vector<uint8_t> chbuf(len);
    if (len > 0 && !recv_all(conn, chbuf.data(), len)) {
        cerr << "[Server] Failed to receive full bit payload\n";
        close(conn);
        close(listenfd);
        return 6;
    }
    
    cerr << "[Server] Received " << len << " bytes (ascii '0'/'1')\n";
    
    vector<int> chan_bits;
    chan_bits.reserve(len);
    for (uint8_t c : chbuf) {
        chan_bits.push_back(c == '1' ? 1 : 0);
    }
    
    auto [data_bits, manch_violations] = manchester_decode(chan_bits);
    cerr << "[Server] Manchester decode produced " << data_bits.size() << " data bits, violations: " << manch_violations << "\n";
    
    vector<uint8_t> bytes = bits_to_bytes(data_bits);
    if (bytes.size() < 2) {
        cerr << "[Server] Not enough bytes for CRC\n";
        close(conn);
        close(listenfd);
        return 7;
    }
    
    size_t nbytes = bytes.size();
    uint16_t received_crc = ((uint16_t)bytes[nbytes-2] << 8) | bytes[nbytes-1];
    
    vector<uint8_t> payload(bytes.begin(), bytes.end() - 2);
    uint16_t computed_crc = crc16_ccitt(payload);
    
    cerr << "[Server] Received CRC: 0x" << hex << setw(4) << setfill('0') << received_crc << " Computed CRC: 0x" << setw(4) << computed_crc << dec << setfill(' ') << "\n";
    
    bool crc_ok = (received_crc == computed_crc);
    
    if (crc_ok && manch_violations == 0) {
        cout << "[Server] CRC OK and no Manchester violations -> no error detected\n";
    }
    else {
        cout << "[Server] ERROR detected!\n";
        if (!crc_ok) cout << " - CRC mismatch\n";
        if (manch_violations > 0) cout << " - Manchester violations: " << manch_violations << "\n";
    }
    
    string payload_str(payload.begin(), payload.end());
    cout << "[Server] Payload (as received, possibly corrupted):\n" << payload_str << "\n";
    
    close(conn);
    close(listenfd);
    return 0;
}