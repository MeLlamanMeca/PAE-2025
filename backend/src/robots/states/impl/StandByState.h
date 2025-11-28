#pragma once
#include "../RobotState.h"
#include "../../Robot.h"
#include "../../../common/Map.h"

#include "WorkingState.h"

class StandByState : public RobotState {
    private:

    public: 
        StateType getState() override { return StateType::STANDBY; };
        
        void endCurrentTask(Robot& robot) override {
            throw std::runtime_error("No task in progress.");
        }
        
        void startTask(Robot& robot) override {
            auto& tasks = robot.getTasks();
            if(tasks.empty()) throw std::logic_error("The robot has no tasks to start.");

            Task& task = tasks.front().get();
            Map& map = robot.getMap();
            Point position = robot.getPosition();
            Point start = task.getIni();
            Point end = task.getFin();

            Route route = map.startTask(start, end, position);

            robot.deleteTask(task.getID());
            robot.setState(std::make_unique<WorkingState>(task, route));
        }

        Task& getActiveTask() const {
            throw std::runtime_error("No active task available in StandByState.");
        }
        Route& getActiveRoute() const {
            throw std::runtime_error("No active route available in StandByState.");
        }
};
