#pragma once
#include <vector>
#include <iostream>
#include <list>
#include "Route.cpp"
#include "Robot.cpp"
#include "TaskQueue.cpp"
#include "../utils/Point.cpp"
#include "./PointsOfInterest/Poi.cpp"

class Map {
private:
    std::vector<std::vector<int>> map;
    std::vector<Robot*> robots;
    std::list<Poi> PointsOfInterest;

public:
    Map(std::vector<std::vector<int>> m, std::vector<Robot*> r) : map{m}, robots{r} {}
};