#pragma once
#include "../RobotState.h"
#include "../../Robot.h"

#include "StandByState.h"
class WorkingState : public RobotState {
    private:
        Task active;
        Route route;

    public: 
        WorkingState(Task active, Route route) : active(active), route(route) {}

        StateType getState() const override { return StateType::WORKING; };
        
        void to_json(nlohmann::json& j) const override {
            j = json{
            {"state", StateTypeToString(getState())},
            {"task", active},
            {"route", route}
            };
        }
        
        void endCurrentTask(Robot& robot) override {
            robot.setState(std::make_unique<StandByState>());
        }

        void startTask(Robot& robot) override {
            throw std::runtime_error("Task in progress.");
        }
        Task& getActiveTask() const {
            return const_cast<Task&>(active);
        }
        Route& getActiveRoute() const {
            return const_cast<Route&>(route);
        }
};

    inline void to_json(json& j, const WorkingState& p) {
        p.to_json(j);
    };