#include "App.h"
#include "../POIs/impl/CommonPOI.h" 
#include "../POIs/impl/ChargingBayPOI.h" 

//----------------------------------------------------------------------------------------
// PRIVATE FUNCTIONS ----------------------------------------------------------------------
// ----------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------
// PUBLIC FUNCTIONS -----------------------------------------------------------------------
// ----------------------------------------------------------------------------------------

void App::endCurrentTask(int robotID) {
    int activeID = serviceFactory.getRobotService().getRobot(robotID).getActiveTask().getID();
    serviceFactory.getRobotService().endCurrentTask(robotID);
    serviceFactory.getTaskService().deleteTask(activeID);
}

void App::startTask(int robotID) {
    return serviceFactory.getRobotService().startTask(robotID);
}

void App::updateRobotPosition(int robotID, Point pos) {
    serviceFactory.getRobotService().updateRobotPosition(robotID, pos);
}

void App::createForbidenZone(int mapID, Point tl, Point br) {
    serviceFactory.getMapService().createForbidenZone(mapID, tl, br);
}
void App::createSlowZone(int mapID, Point tl, Point br) {
    serviceFactory.getMapService().createSlowZone(mapID, tl, br);
}
void App::createCommonZone(int mapID, Point tl, Point br) {
    serviceFactory.getMapService().createCommonZone(mapID, tl, br);
}

void App::createMap(int mapID, std::vector<std::vector<char>> map) {
    serviceFactory.getMapService().createMap(mapID, map);
}

void App::createRobot(int mapID, Point position){
    Map& m = serviceFactory.getMapService().getMap(mapID);
    Robot& r = serviceFactory.getRobotService().createRobot(position, m);
    serviceFactory.getMapService().addRobot(mapID, r);

    ChargingBayPOI poi(position);
    serviceFactory.getMapService().addPoi(mapID, poi);
}

TaskAssignation App::createTask(int mapID, Point ini, Point fin) {
    Task& task = serviceFactory.getTaskService().createTask(ini, fin);

    TaskAssignation result = serviceFactory.getMapService().assignTask(task, mapID);
    return result;
}

void App::createCommonPoi(int mapID, Point position, std::string name){
    CommonPOI poi(position, name);
    serviceFactory.getMapService().addPoi(mapID, poi);
}

void App::deleteMap(int mapID) {/*no implementada*/}

void App::deleteRobot(int robotID) {/*no implementada*/}

void App::deleteTask(int robotID, int taskID) {
    serviceFactory.getRobotService().deleteTask(robotID, taskID);
    serviceFactory.getTaskService().deleteTask(taskID);
}

void App::deletePoi(int mapID, Point position) {
    serviceFactory.getMapService().deletePoi(mapID, position);
}

Task& App::getTask(int taskID) {
    return serviceFactory.getTaskService().getTask(taskID);
}
Robot& App::getRobot(int robotID) {
    return serviceFactory.getRobotService().getRobot(robotID);
}
Map& App::getMap(int mapID) {
    return serviceFactory.getMapService().getMap(mapID);
}


