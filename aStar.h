#ifndef A_STAR_H
#define A_STAR_H

#include <vector>
#include <utility>
using namespace std;

vector<pair<int, int>> aStar(const vector<vector<int>>& matriz,
                                 pair<int, int> inicio,
                                 pair<int, int> fin);

#endif