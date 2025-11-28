#pragma once
#include "./MapService.h"
#include "./RobotService.h"
#include "./TaskService.h"

class ServiceFactory {
    public:
        virtual TaskService& getTaskService() = 0;
        virtual RobotService& getRobotService() = 0;
        virtual MapService& getMapService() = 0;
};