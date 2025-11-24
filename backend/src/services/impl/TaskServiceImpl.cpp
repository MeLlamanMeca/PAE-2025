#include "TaskServiceImpl.h"

Task& TaskServiceImpl::createTask(Point ini, Point fin) {
    return controllerFactory.getTaskController().create(ini, fin);
}

void TaskServiceImpl::deleteTask(int taskID) {
    controllerFactory.getTaskController().deleteTask(taskID);
}
