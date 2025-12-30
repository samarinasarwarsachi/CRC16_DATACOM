#include <bits/stdc++.h>

#include "sender/sender.hpp"
#include "receiver/receiver.hpp"

using namespace std;

int main(int argc, char** argv) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    if (argc < 2) {
        cerr << "Usage:\n"
             << "  Receiver: " << argv[0] << " server <port>\n"
             << "  Sender:   " << argv[0] << " client <server_ip> <port> <testcase> [payload ...]\n";
        return 1;
    }
    
    string mode = argv[1];
    
    if (mode == "server") {
        if (argc < 3) {
            cerr << "Server requires port\n";
            return 1;
        }
        int port = atoi(argv[2]);
        return run_server(port);
    } 
    else if (mode == "client") {
        if (argc < 5) {
            cerr << "Client requires server_ip port testcase [payload ...]\n";
            return 1;
        }
        string ip = argv[2];
        int port = atoi(argv[3]);
        int testcase = atoi(argv[4]);
        string payload_str;
        if (argc >= 6) {
            payload_str = argv[5];
            for (int i = 6; i < argc; ++i) {
                payload_str += " " + string(argv[i]);
            }
        } else {
            payload_str = "Hello, CRC16 test payload!";
        }
        return run_client(ip, port, testcase, payload_str);
    }
    else {
        cerr << "Unknown mode: " << mode << "\n";
        return 1;
    }
}