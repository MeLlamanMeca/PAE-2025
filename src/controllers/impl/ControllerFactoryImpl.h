#pragma once
#include "../ControllerFactory.h"
#include "./MapControllerImpl.h"
#include "./RobotControllerImpl.h"
#include "./TaskControllerImpl.h"

class ControllerFactoryImpl : public ControllerFactory {
    private:
        std::unique_ptr<TaskControllerImpl> taskController;
        std::unique_ptr<MapControllerImpl> mapController;
        std::unique_ptr<RobotControllerImpl> robotController;

    public:
        ControllerFactoryImpl()
            : taskController(std::make_unique<TaskControllerImpl>()),
            mapController(std::make_unique<MapControllerImpl>()),
            robotController(std::make_unique<RobotControllerImpl>()) {}

        TaskController& getTaskController() override { return *taskController; }
        RobotController& getRobotController() override { return *robotController; }
        MapController& getMapController() override { return *mapController; }
};