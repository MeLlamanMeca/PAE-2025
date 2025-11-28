#pragma once

#include "./Point.h"
#include <list>

class Route {
    private:
        std::list<Point> route;
        int cost;

    public:
        Route(const std::list<Point>& points) : route(points) {}
        
        const std::list<Point>& getPoints() const { return route; }
        int getCost() const { return cost; }
};