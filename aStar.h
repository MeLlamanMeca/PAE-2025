#ifndef A_STAR_H
#define A_STAR_H

#include <vector>
#include <utility>
#include "./assignTasksToRobots/point/point.hpp"
using namespace std;

pair<vector<Point>, int> aStar(
    const vector<vector<int>>& mapa,
    const vector<vector<Point>>& arrayRutes,
    const vector<Point>& arrayPosicionsRobots,
    Point posIni,
    Point posFi
);

#endif