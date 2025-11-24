#ifndef A_STAR_H
#define A_STAR_H

#include <vector>
#include <utility>
#include "./assignTasksToRobots/point/point.hpp"
using namespace std;

pair<vector<Point>, int> aStar(
    const vector<vector<int>>& matrix,
    Point inici,
    Point fi
);

#endif