#pragma once
#include "../ServiceFactory.h"
#include "../../controllers/ControllerFactory.h"
#include "./MapServiceImpl.h"
#include "./RobotServiceImpl.h"
#include "./TaskServiceImpl.h"

class ServiceFactoryImpl : public ServiceFactory {
    private:
        std::unique_ptr<TaskServiceImpl> taskService;
        std::unique_ptr<MapServiceImpl> mapService;
        std::unique_ptr<RobotServiceImpl> robotService;

    public:
        ServiceFactoryImpl(ControllerFactory& cf)
            : taskService(std::make_unique<TaskServiceImpl>(cf)),
            mapService(std::make_unique<MapServiceImpl>(cf)),
            robotService(std::make_unique<RobotServiceImpl>(cf)) {}

        TaskService& getTaskService() override { return *taskService; }
        RobotService& getRobotService() override { return *robotService; }
        MapService& getMapService() override { return *mapService; }
};