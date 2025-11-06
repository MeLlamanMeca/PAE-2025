#pragma once
#include <iostream>
#include "Route.cpp"
#include "TaskQueue.cpp"
#include "../utils/Point.cpp"

class Robot {
private:
    int load;
    Point position;
    Route activeRoute;
    TaskQueue taskQueue;

public:
    Robot() : load(0), position(), activeRoute(), taskQueue() {}
    
    Robot(int load, Point position) 
        : load(load), position(position), activeRoute(), taskQueue() {}
    
    // Getters
    int getLoad() const { return load; }
    Point getPosition() const { return position; }
    Route getActiveRoute() const { return activeRoute; }
    TaskQueue getTaskQueue() const { return taskQueue; }
    
    // Setters
    void setLoad(int newLoad) { load = newLoad; }
    void setPosition(Point newPosition) { position = newPosition; }
    void setActiveRoute(Route newRoute) { activeRoute = newRoute; }
    void setTaskQueue(TaskQueue newQueue) { taskQueue = newQueue; }
};