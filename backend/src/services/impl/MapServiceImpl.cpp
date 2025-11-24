#include "MapServiceImpl.h"
//privates

TaskAssignation taskAssigner(Map& map, Task& task) {
 return TaskAssignation(1, 1); // Update arguments to match the constructor
}


//publics

Map& MapServiceImpl::get(int mapID) {
    return controllerFactory.getMapController().get(mapID);
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

void MapServiceImpl::createMap(int mapID, std::vector<std::vector<char>> map) {
    controllerFactory.getMapController().create(mapID, map);
}

void MapServiceImpl::deletePoi(int mapID, Point position) {
    Map& map = controllerFactory.getMapController().get(mapID);
    map.deletePoi(position);
}
