#pragma once

#include <functional>
#include <utility>
#include "./Point.h"

struct PointHash {
    std::size_t operator()(const Point& p) const noexcept {
        return std::hash<int>()(p.x)
             ^ (std::hash<int>()(p.y) << 1);
    }
};

struct PointPairHash {
    std::size_t operator()(const std::pair<Point, Point>& p) const noexcept {
        std::size_t h1 = PointHash{}(p.first);
        std::size_t h2 = PointHash{}(p.second);
        return h1 ^ (h2 << 1);
    }
};
