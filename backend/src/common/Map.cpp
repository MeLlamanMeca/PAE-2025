#include "Map.h"
#include <stdexcept>

// ---------------- GETTERS ----------------

const std::vector<std::vector<char>>& Map::getMap() const {
    return map;
}

const std::unordered_map<int, std::reference_wrapper<POI>>& Map::getPOIs() const {
    return pointsOfInterest;
}

const std::unordered_map<int, std::reference_wrapper<Robot>>& Map::getRobots() const {
    return robots;
}

Robot& Map::getRobot(int robotID) {
    auto it = robots.find(robotID);
    if (it != robots.end()) 
        return it->second.get();

    throw std::out_of_range("Robot with the given ID does not exist.");
}

// ---------------- DISTANCE ----------------

float Map::getDistance(Point a, Point b) {
    if (a == b) return 0;

    std::pair<Point, Point> key =
        (a < b) ? std::make_pair(a, b) : std::make_pair(b, a);

    auto it = distanceMap.find(key);
    if (it != distanceMap.end())
        return it->second;

    return 1.0f; // TODO: implementar A*
}

// ---------------- ADD / REMOVE ----------------

void Map::addRobot(Robot& r) {
    int id = r.getID();
    robots.insert({id, r});
}

void Map::addPoi(POI& poi) {
    poi.setID(pointsOfInterest.size());
    pointsOfInterest.insert({poi.getID(), poi});
}

void Map::deletePoi(Point position) {
    for (auto it = pointsOfInterest.begin(); it != pointsOfInterest.end(); ++it) {
        if (it->second.get().getPos() == position) {
            pointsOfInterest.erase(it);
            return;
        }
    }
    throw std::out_of_range("POI at the given position does not exist.");
}

// ---------------- ZONES ----------------

void Map::createForbidenZone(Point tl, Point br) {
    // TODO
}

void Map::createSlowZone(Point tl, Point br) {
    // TODO
}

void Map::createCommonZone(Point tl, Point br) {
    // TODO
}

TaskData startTask(Point start, Point end, Point robotPos) {
    // TODO
    throw std::logic_error("startTask not yet implemented.");
}
