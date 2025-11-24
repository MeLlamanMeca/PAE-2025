#pragma once
#include "../RobotState.h"

class WorkingState;

class StandByState : public RobotState {
    private:

    public: 
        StateType getState() override { return StateType::STANDBY; };
        
        void endCurrentTask(Robot& robot) override {
            throw std::runtime_error("No task in progress.");
        }
        TaskData startTask(Robot& robot) override {
            //get map
            //get first task
            //llamar a map.startTask(Point start, Point end, Point robotPosition)
            throw std::logic_error("startTask not implemented.");
        }
};
