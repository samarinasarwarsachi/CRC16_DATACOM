#ifndef MANCHESTER_HPP
#define MANCHESTER_HPP

#include <vector>
#include <utility>

using namespace std;

vector<int> manchester_encode(const vector<int>& bits);
pair<vector<int>, int> manchester_decode(const vector<int>& enc);

#endif