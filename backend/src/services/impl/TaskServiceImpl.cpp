#include "TaskServiceImpl.h"

Task& TaskServiceImpl::createTask(Point ini, Point fin) {
    return controllerFactory.getTaskController().create(ini, fin);
}

Task& TaskServiceImpl::deleteTask(int taskID) {
    return controllerFactory.getTaskController().deleteTask(taskID);
}

Task& TaskServiceImpl::getTask(int taskID) {
    return controllerFactory.getTaskController().get(taskID);
}
