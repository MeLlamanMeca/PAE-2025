#pragma once

#include "./Point.h"
#include <list>

class Route {
    private:
        std::list<Point> route;

    public:
        Route(const std::list<Point>& points) : route(points) {}
};