#include "Map.h"
#include <stdexcept>

Route Map::aStar(const std::vector<std::vector<unsigned char>>& map, Point ini, Point fin) {
    throw std::logic_error("aStar not yet implemented.");
}

// ---------------- GETTERS ----------------

const std::vector<std::vector<unsigned char>>& Map::getMap() const {
    return map;
}

const std::unordered_map<int, std::unique_ptr<POI>>& Map::getPOIs() const {
    return pointsOfInterest;
}

const std::unordered_map<int, std::reference_wrapper<Robot>>& Map::getRobots() const {
    return robots;
}

Robot& Map::getRobot(int robotID) {
    auto it = robots.find(robotID);
    if (it != robots.end()) 
        return it->second.get();

    throw std::out_of_range("Robot with the given ID is not in map.");
}

// ---------------- DISTANCE ----------------

int Map::getDistance(Point a, Point b) {
    if (a == b) return 0;

    std::pair<Point, Point> key =
        (a < b) ? std::make_pair(a, b) : std::make_pair(b, a);

    auto it = distanceMap.find(key);
    if (it != distanceMap.end())
        return it->second;

    else {
        int cost = Map::aStar(this->map, key.first, key.second).getCost();
        distanceMap.emplace(key, cost);
        return cost;
    }
}

// ---------------- ADD / REMOVE ----------------

void Map::addRobot(Robot& r) {
    int id = r.getID();
    robots.insert({id, r});
}

void Map::addPoi(std::unique_ptr<POI> poi) {
    if (!poi) return;
    poi->setID(pointsOfInterest.size());
    pointsOfInterest.insert({poi->getID(), std::move(poi)});
}

void Map::deletePoi(Point position) {
    for (auto it = pointsOfInterest.begin(); it != pointsOfInterest.end(); ++it) {
        if (it->second->getPos() == position) {
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

Route Map::startTask(Point start, Point end, Point robotPos) const {
    // TODO
    //crear mapa temporal con caminos de robots
    //llamar a aStar(mapatemporal, robotPos, iniPos)
    //llamar a aStar(mapatemporal, iniPos, endPos)
    throw std::logic_error("startTask not yet implemented.");
}
