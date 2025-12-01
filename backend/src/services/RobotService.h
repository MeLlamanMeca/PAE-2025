#pragma once

#include <list>
#include "../robots/Robot.h"
#include "../tasks/Task.h"

class RobotService {
    public:
        virtual void deleteTask(int robotID,int taskID) = 0;
        virtual void endCurrentTask(int robotID) = 0;
        virtual void startTask(int robotID) = 0;
        virtual void updateRobotPosition(int robotID, Point pos) = 0;
        
        virtual Robot& createRobot(Point position, Map& map, int maxWeight) = 0;
        virtual Robot& getRobot(int robotID) = 0;
        virtual Robot& deleteRobot(int robotID) = 0;
    
};