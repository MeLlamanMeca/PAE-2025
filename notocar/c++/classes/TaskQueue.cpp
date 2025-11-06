#pragma once
#include "./tasks/Task.cpp"
#include <iostream>
#include <queue>

class TaskQueue {
private:
    std::queue<Task> commonTasks;
    std::queue<Task> priorityTasks;
    std::queue<Task> VITasks;


public:
    TaskQueue() noexcept = default;

    TaskQueue(std::queue<Task> common, std::queue<Task> priority, std::queue<Task> VIT) noexcept : commonTasks{common}, priorityTasks{priority}, VITasks{VIT} {}
    
};

