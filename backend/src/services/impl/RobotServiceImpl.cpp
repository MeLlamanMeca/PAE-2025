#include "RobotServiceImpl.h"

void RobotServiceImpl::deleteTask(int robotID, int taskID) {
    Robot& robot = controllerFactory.getRobotController().get(robotID);
    robot.deleteTask(taskID);
}

void RobotServiceImpl::endCurrentTask(int robotID) {
    Robot& robot = controllerFactory.getRobotController().get(robotID);
    robot.endCurrentTask();
}

void RobotServiceImpl::updateRobotPosition(int robotID, Point pos) {
    Robot& robot = controllerFactory.getRobotController().get(robotID);
    robot.setPosition(pos);
}

Robot& RobotServiceImpl::createRobot(Point position, Map& map) {
    Robot& robot = controllerFactory.getRobotController().create(position, map);
    return robot;
}

Robot& RobotServiceImpl::deleteRobot(int robotID) {
    throw std::logic_error("Not implemented");
}

Robot& RobotServiceImpl::getRobot(int robotID) {
    return controllerFactory.getRobotController().get(robotID);
}

void RobotServiceImpl::startTask(int robotID) {
    Robot& robot = controllerFactory.getRobotController().get(robotID);
    return robot.startTask();
}
