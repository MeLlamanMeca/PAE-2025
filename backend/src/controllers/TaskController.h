#pragma once
#include "../tasks/Task.h"
#include "../common/Point.h"

class TaskController {
    public:
        virtual Task& get(int taskID) = 0;
        virtual Task& create(Point ini, Point fin) = 0;
        virtual Task& deleteTask(int taskID) = 0;
};