#pragma once

#include <memory>
#include "../common/Point.h"
#include "../tasks/Task.h"
#include "./states/RobotState.h"
#include "./states/StateType.h"

class Map;

class Robot {
    protected:
        int ID;
        int maxWeight;
        Point position;
        Point chargingBayPosition;
        int load = 0;
        
        std::unique_ptr<RobotState> state;
        std::list<std::reference_wrapper<Task>> tasks;
        Map& map;

    public:
        Robot(Point position, int ID, Map& map, int maxWeight) : chargingBayPosition(position), position(position), ID(ID), state(/*TODO poner estado standby por defecto*/), map(map), maxWeight(maxWeight) {}

        // --- GETTERS (default) ---
        const std::list<std::reference_wrapper<Task>>& getTasks() const { return tasks; }
        int getID() const { return ID; }
        const Point& getPosition() const { return position; }
        Map& getMap() const { return map; }
        const int getMaxWeight() const { return maxWeight; }
        StateType getState() const { return state->getState(); }


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

        int canHandle(int weight) const { return weight <= maxWeight; }
        Task& getActiveTask() const { state->getActiveTask(); }
        Route& getActiveRoute() const { state->getActiveRoute(); }
        
};