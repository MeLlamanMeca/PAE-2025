#pragma once
#include "./StateType.h"
#include "../../common/Route.h"
#include "../../tasks/Task.h"
class Robot;

class RobotState {
    public: 
        virtual StateType getState() = 0;
        
        virtual void endCurrentTask(Robot& robot) {
            throw std::runtime_error("An error occurred while ending the current task.");
        }
        virtual void startTask(Robot& robot) {
            throw std::runtime_error("An error occurred while starting the current task.");
        }
        virtual Task& getActiveTask() const {
            throw std::runtime_error("An error occurred while getting active task.");
        }
        virtual Route& getActiveRoute() const {
            throw std::runtime_error("An error occurred while gettign active route.");
        }
};
