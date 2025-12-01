#pragma once

#include <memory>
#include "../common/Point.h"
#include "../tasks/Task.h"
#include "./states/RobotState.h"
#include "./states/StateType.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;
class Map;

class Robot {
    protected:
        int ID;
        int maxWeight;
        Point position;
        Point chargingBayPosition;
        double load = 0.0;
        std::unique_ptr<RobotState> state;
        std::list<std::reference_wrapper<Task>> tasks;
        Map& map;

    public:
        Robot(Point position, int ID, Map& map, int maxWeight) 
            : chargingBayPosition(position), position(position), ID(ID), map(map), maxWeight(maxWeight) {}

        // --- GETTERS (default) ---
        const std::list<std::reference_wrapper<Task>> getTasks() const { return tasks; }
        int getID() const { return ID; }
        const Point& getPosition() const { return position; }
        const Point& getChargingBayPosition() const { return chargingBayPosition; }
        const Map& getMap() const { return map; }
        const int getMaxWeight() const { return maxWeight; }
        RobotState& getState() const { return *state; }
        double getLoad() const { return load; }

        // --- SETTERS (default) ---
        void setState(std::unique_ptr<RobotState> newState) { state = std::move(newState); }
        void setPosition(Point newPos) { position = newPos; }

        // --- STATE functions ---
        void endCurrentTask() { state->endCurrentTask(*this); }
        void startTask() { return state->startTask(*this); }

        // others
        void addTask(Task& task, int position) {
            if (position < 0 || position > tasks.size()) throw std::out_of_range("Position is out of range");
            auto it = tasks.begin();
            std::advance(it, position);
            tasks.insert(it, task);
        }

        void deleteTask(int taskID) {
            tasks.remove_if([taskID](const Task& task) {
                return task.getID() == taskID;
            });
        }

        bool canCarry(int weight) const {
            return weight <= load;
        }
        Task& getActiveTask() const { return state->getActiveTask(); }
        Route& getActiveRoute() const { return state->getActiveRoute(); }
        bool isWorking() const { return (state->getState() == StateType::WORKING); }
        
};

    inline void to_json(json& j, const Robot& p) {

        json tasks_array = json::array();
        for (const auto& task : p.getTasks()) tasks_array.push_back(task);

        json state = json::object();
        p.getState().to_json(state);
        
        j = json{
            {"ID", p.getID()},
            {"maxWeight", p.getMaxWeight()},
            {"position", p.getPosition()},
            {"chargingBayPosition", p.getChargingBayPosition()},
            {"tasks", p.getTasks()},
            {"state", state}
        };
    }