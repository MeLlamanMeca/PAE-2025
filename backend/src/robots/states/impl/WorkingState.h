#pragma once
#include "../RobotState.h"
#include "../../Robot.h"

#include "StandByState.h"

class WorkingState : public RobotState {
    private:

    public: 
        StateType getState() override { return StateType::WORKING; };
        
        void endCurrentTask(Robot& robot) override {
            auto tasks = robot.getTasks();
            if (!tasks.empty()) {
                auto primeraTarea = tasks.front();
                robot.deleteTask(primeraTarea.get().getID());
                robot.setState(std::make_unique<StandByState>());
            }
            else throw std::runtime_error("Robot has no tasks.");
        }

        TaskData startTask(Robot& robot) override {
            throw std::runtime_error("Task in progress.");
        }
};
