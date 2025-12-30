#include "manchester.hpp"

using namespace std;

vector<int> manchester_encode(const vector<int>& bits) {
    vector<int> enc;
    enc.reserve(bits.size() * 2);
    for (int b : bits) {
        if (b == 0) {
            enc.push_back(0);
            enc.push_back(1);
        }
        else {
            enc.push_back(1);
            enc.push_back(0);
        }
    }
    return enc;
}

pair<vector<int>, int> manchester_decode(const vector<int>& enc) {
    vector<int> bits;
    int violations = 0;
    if (enc.size() % 2 != 0) {
        violations++;
    }
    for (size_t i = 0; i + 1 < enc.size(); i += 2) {
        int a = enc[i], b = enc[i + 1];
        if (a == 0 && b == 1)
            bits.push_back(0);
        else if (a == 1 && b == 0)
            bits.push_back(1);
        else
            violations++;
    }
    return {bits, violations};
}