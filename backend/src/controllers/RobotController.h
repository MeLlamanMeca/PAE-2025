#pragma once
#include "../robots/Robot.h"
#include "../common/Point.h"

class RobotController {
    public:
        virtual Robot& get(int robotID) = 0;
        virtual Robot& create(Point position, Map& map, int maxWeight) = 0;
        virtual void deleteRobot(int robotID) = 0;
};
