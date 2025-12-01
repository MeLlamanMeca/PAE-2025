#pragma once

#include <vector>
#include <list>
#include <unordered_map>
#include <functional>

#include "../POIs/POI.h"
#include "./Point.h"
#include "./PointHash.h"
#include "../robots/Robot.h"
#include "../common/TaskData.h"

class Map {
private:
    std::vector<std::vector<char>> map;
    std::unordered_map<int, std::reference_wrapper<POI>> pointsOfInterest;
    std::unordered_map<int, std::reference_wrapper<Robot>> robots;
    std::unordered_map<std::pair<Point, Point>, int, PointPairHash> distanceMap;

    static Route aStar(const std::vector<std::vector<char>>& map, Point ini, Point fin);

public:
    Map(std::vector<std::vector<char>> map) : map(std::move(map)) {}
    
    const std::vector<std::vector<char>>& getMap() const;
    const std::unordered_map<int, std::reference_wrapper<POI>>& getPOIs() const;
    const std::unordered_map<int, std::reference_wrapper<Robot>>& getRobots() const;
    Robot& getRobot(int robotID);

    int getDistance(const Point& a, const Point& b);

    void addRobot(Robot& r);
    void deleteRobot(int robotID);
    void addPoi(POI& poi);
    void deletePoi(Point position);

    void createForbidenZone(Point tl, Point br);
    void createSlowZone(Point tl, Point br);
    void createCommonZone(Point tl, Point br);

    TaskData startTask(Point start, Point end, Point robotPos);
};
