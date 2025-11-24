#pragma once

#include "./Point.h"
#include "./Route.h"
#include <list>

struct TaskData {
    int taskID;
    Point startPoint;
    Point endPoint;
    Route route;

    TaskData(int id, const Point& start, const Point& end, const Route& r)
        : taskID(id), startPoint(start), endPoint(end), route(r) {}
};