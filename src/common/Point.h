#pragma once
#include <cmath>
#include <iostream>


struct Point {
    int x{0};
    int y{0};

    constexpr Point() noexcept = default;
    constexpr Point(int x_, int y_) noexcept : x(x_), y(y_) {}

    constexpr int getX() const noexcept { return x; }
    constexpr int getY() const noexcept { return y; }
    constexpr void setX(int v) noexcept { x = v; }
    constexpr void setY(int v) noexcept { y = v; }

    constexpr bool operator==(const Point& o) const noexcept { return x == o.x && y == o.y; }
    constexpr bool operator!=(const Point& o) const noexcept { return !(*this == o); }
    constexpr bool operator<(const Point& o) const noexcept { return (x < o.x) || (x == o.x && y < o.y); }
    constexpr bool operator>(const Point& o) const noexcept { return (x > o.x) || (x == o.x && y > o.y); }

    [[nodiscard]] constexpr Point operator+(const Point& o) const noexcept { return Point(x + o.x, y + o.y); }
    [[nodiscard]] constexpr Point operator-(const Point& o) const noexcept { return Point(x - o.x, y - o.y); }

    [[nodiscard]] double distanceTo(const Point& o) const noexcept {
        double dx = double(x - o.x);
        double dy = double(y - o.y);
        return std::sqrt(dx * dx + dy * dy);
    }
};

    inline std::ostream& operator<<(std::ostream& os, const Point& p) {
        return os << "Point{" << p.x << ", " << p.y << "}";
    }