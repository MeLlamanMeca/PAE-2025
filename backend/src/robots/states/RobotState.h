#pragma once
#include "../../common/TaskData.h"
#include "./StateType.h"
class Robot;

class RobotState {
    private:

    public: 
        virtual StateType getState() = 0;
        
        virtual void endCurrentTask(Robot& robot) {
            throw std::runtime_error("An error occurred while ending the current task.");
        }
        virtual TaskData startTask(Robot& robot) {
            throw std::runtime_error("An error occurred while starting the current task.");
        }
};
