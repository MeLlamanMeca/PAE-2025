#pragma once
#include "../TaskController.h"

class TaskControllerImpl : public TaskController {
    private:
        int id = 0;
        std::map<int,Task> taskMap;

    public:
        Task& get(int taskID) { 
            auto it = taskMap.find(taskID);
            if (it != taskMap.end()) return it->second;
            throw std::out_of_range("Task ID not found");
        }

        Task& create(Point ini, Point fin) {
            int taskID = id++;
            auto result = taskMap.emplace(std::piecewise_construct,
                std::forward_as_tuple(taskID),
                std::forward_as_tuple(ini, fin, taskID));

            if (!result.second) throw std::runtime_error("Task ID already exists");
            return result.first->second;
        }

        void deleteTask(int taskID) { 
            auto it = taskMap.find(taskID);
            if (it != taskMap.end()) taskMap.erase(it);
            else throw std::out_of_range("Task ID not found"); 
        }
};