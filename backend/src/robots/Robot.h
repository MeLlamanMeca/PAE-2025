#pragma once

#include <memory>
#include "../common/Point.h"
#include "../tasks/Task.h"
#include "./states/RobotState.h"
#include "../common/TaskData.h"

class Map;

class Robot {
    protected:
        int ID;
        Map& map;
        Point position;
        Point chargingBayPosition;
        double load = 0.0;
        std::unique_ptr<RobotState> state;
        std::list<std::reference_wrapper<Task>> tasks;

    public:
        Robot(Point position, int ID, Map& map) : chargingBayPosition(position), position(position), ID(ID), state(/*TODO poner estado standby por defecto*/), map(map){}

        // --- GETTERS (default) ---
        const std::list<std::reference_wrapper<Task>> getTasks() const { return tasks; }
        int getID() const { return ID; }
        const Point& getPosition() const { return position; }
        const Map& getMap() const { return map; }
        double getLoad() const { return load; }

        // --- SETTERS (default) ---
        void setState(std::unique_ptr<RobotState> newState) { state = std::move(newState); }
        void setPosition(Point newPos) { position = newPos; }

        // --- STATE functions ---
        void endCurrentTask() { state->endCurrentTask(*this); }
        TaskData startTask() { return state->startTask(*this); }

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

        // TODO: Actualizar desgaste acumulado
};