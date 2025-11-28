#pragma once
#include "../tasks/Task.h"
#include "../common/Point.h"

class TaskService {
    public:
        virtual Task& createTask(Point ini, Point fin) = 0;
        virtual void deleteTask(int taskID) = 0;

};