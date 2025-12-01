#pragma once

#include "../TaskService.h"
#include "../../controllers/ControllerFactory.h"
#include "../../common/Point.h"
#include "../../tasks/Task.h"

class TaskServiceImpl : public TaskService {
private:
    ControllerFactory& controllerFactory;

public:
    TaskServiceImpl(ControllerFactory& cf) : controllerFactory(cf) {}

    Task& createTask(Point ini, Point fin) override;
    Task& deleteTask(int taskID) override;
    Task& getTask(int taskID) override;
};
