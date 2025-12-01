#pragma once

#include "../RobotService.h"
#include "../../controllers/ControllerFactory.h"
#include "../../common/TaskAssignation.h"

class RobotServiceImpl : public RobotService {
private:
    ControllerFactory& controllerFactory;

public:
    RobotServiceImpl(ControllerFactory& cf) : controllerFactory(cf) {}
    void deleteTask(int robotID, int taskID) override;
    void endCurrentTask(int robotID) override;
    void updateRobotPosition(int robotID, Point pos) override;
    Robot& createRobot(Point position, Map& map, int maxWeight) override;
    Robot& deleteRobot(int robotID) override;
    void startTask(int robotID) override;
    Robot& getRobot(int robotID) override;
};
