#ifndef BIT_UTILS_HPP
#define BIT_UTILS_HPP

#include <cstdint>
#include <vector>

using namespace std;

vector<int> bytes_to_bits(const vector<uint8_t>& bytes);
vector<uint8_t> bits_to_bytes(const vector<int>& bits);
void flip_bits(vector<int>& bits, const vector<size_t>& positions);
void flip_burst(vector<int>& bits, size_t startPos, size_t len);

#endif