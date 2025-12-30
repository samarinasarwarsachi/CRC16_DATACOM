#include "bit_utils.hpp"

using namespace std;

vector<int> bytes_to_bits(const vector<uint8_t>& bytes) {
    vector<int> bits;
    for (uint8_t b : bytes) {
        for (int i = 7; i >= 0; --i)
            bits.push_back((b >> i) & 1);
    }
    return bits;
}

vector<uint8_t> bits_to_bytes(const vector<int>& bits) {
    vector<uint8_t> bytes;
    int n = bits.size();
    int padded = ((n + 7) / 8) * 8;
    for (int i = 0; i < padded; i += 8) {
        uint8_t val = 0;
        for (int j = 0; j < 8; ++j) {
            int idx = i + j;
            int bit = (idx < n) ? bits[idx] : 0;
            val = (val << 1) | (bit & 1);
        }
        bytes.push_back(val);
    }
    return bytes;
}

void flip_bits(vector<int>& bits, const vector<size_t>& positions) {
    for (size_t p : positions) {
        if (p < bits.size())
            bits[p] ^= 1;
    }
}

void flip_burst(vector<int>& bits, size_t startPos, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        size_t p = startPos + i;
        if (p < bits.size())
            bits[p] ^= 1;
    }
}