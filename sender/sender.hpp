#ifndef SENDER_HPP
#define SENDER_HPP

#include <string>

using namespace std;

int run_client(const string& server_ip, int port, int testcase, const string& payload_str);

#endif