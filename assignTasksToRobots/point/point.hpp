// Point.hpp
#pragma once
#include <cstdint>
#include <functional>
#include <cmath>

struct Point {
    int x = 0;
    int y = 0;

    Point() = default;
    Point(int x_, int y_) : x(x_), y(y_) {}

    bool operator==(const Point& other) const noexcept {
        return x == other.x && y == other.y;
    }
    bool operator!=(const Point& other) const noexcept {
        return !(*this == other);
    }

    Point operator+(const Point& other) const noexcept {
        return Point{x + other.x, y + other.y};
    }

    Point operator-(const Point& other) const noexcept {
        return Point{x - other.x, y - other.y};
    }

    // Distancia Euclídea
    double distanceTo(const Point& other) const noexcept {
        double dx = double(x - other.x);
        double dy = double(y - other.y);
        return std::sqrt(dx*dx + dy*dy);
    }

    // Distancia Manhattan
    int manhattanTo(const Point& other) const noexcept {
        return std::abs(x - other.x) + std::abs(y - other.y);
    }
};

// Hash para usar Point como clave en unordered_map/unordered_set
struct PointHash {
    std::size_t operator()(const Point& p) const noexcept {
        uint64_t a = static_cast<uint32_t>(p.x);
        uint64_t b = static_cast<uint32_t>(p.y);
        uint64_t z = (a << 32) | b;
        // Mezcla simple (splitmix64-like)
        z += 0x9e3779b97f4a7c15ULL;
        z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9ULL;
        z = (z ^ (z >> 27)) * 0x94d049bb133111ebULL;
        z = z ^ (z >> 31);
        return static_cast<std::size_t>(z);
    }
};