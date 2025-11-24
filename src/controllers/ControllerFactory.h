#pragma once
#include "./TaskController.h"
#include "./RobotController.h"
#include "./MapController.h"
class ControllerFactory {
    public:
        virtual TaskController& getTaskController() = 0;
        virtual RobotController& getRobotController() = 0;
        virtual MapController& getMapController() = 0;
};