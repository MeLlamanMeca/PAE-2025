#include "MapServiceImpl.h"
//privates

TaskAssignation taskAssigner(Map& map, Task& task) {
    throw std::logic_error("Not implemented");
}


//publics

Map& MapServiceImpl::getMap(int mapID) {
    return controllerFactory.getMapController().get(mapID);
}

Map& MapServiceImpl::deleteMap(int mapID) {
    throw std::logic_error("Not implemented");
    //todo
}

TaskAssignation MapServiceImpl::assignTask(Task& task, int mapID) {
    Map& map = controllerFactory.getMapController().get(mapID);

    TaskAssignation result = taskAssigner(map, task);

    Robot& candidate = map.getRobot(result.robotID);
    candidate.addTask(task, result.position);

    return result;
}

void MapServiceImpl::addPoi(int mapID, POI& poi) {
    Map& map = controllerFactory.getMapController().get(mapID);
    map.addPoi(poi);
}

void MapServiceImpl::addRobot(int mapID, Robot& r) {
    Map& map = controllerFactory.getMapController().get(mapID);
    map.addRobot(r);
}

void MapServiceImpl::createForbidenZone(int mapID, Point tl, Point br) {
    Map& map = controllerFactory.getMapController().get(mapID);
    map.createForbidenZone(tl, br);
}

void MapServiceImpl::createSlowZone(int mapID, Point tl, Point br) {
    Map& map = controllerFactory.getMapController().get(mapID);
    map.createSlowZone(tl, br);
}

void MapServiceImpl::createCommonZone(int mapID, Point tl, Point br) {
    Map& map = controllerFactory.getMapController().get(mapID);
    map.createCommonZone(tl, br);
}

Map& MapServiceImpl::createMap(int mapID, std::vector<std::vector<char>> map) {
    return controllerFactory.getMapController().create(mapID, map);
}

void MapServiceImpl::deletePoi(int mapID, Point position) {
    Map& map = controllerFactory.getMapController().get(mapID);
    map.deletePoi(position);
}
