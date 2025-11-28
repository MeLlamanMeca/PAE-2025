#pragma once

#include "../RobotService.h"
#include "../../controllers/ControllerFactory.h"
#include "../../common/TaskAssignation.h"
#include "../../common/TaskData.h"

class RobotServiceImpl : public RobotService {
private:
    ControllerFactory& controllerFactory;

public:
    RobotServiceImpl(ControllerFactory& cf) : controllerFactory(cf) {}
    void deleteTask(int robotID, int taskID) override;
    void endCurrentTask(int robotID) override;
    void updateRobotPosition(int robotID, Point pos) override;
    Robot& createRobot(Point position, Map& map) override;
    void deleteRobot(int robotID) override;
    TaskData startTask(int robotID) override;
};
