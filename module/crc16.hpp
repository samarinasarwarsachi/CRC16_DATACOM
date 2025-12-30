#ifndef CRC16_HPP
#define CRC16_HPP

#include <cstdint>
#include <vector>

using namespace std;

uint16_t crc16_ccitt(const vector<uint8_t>& data);

#endif