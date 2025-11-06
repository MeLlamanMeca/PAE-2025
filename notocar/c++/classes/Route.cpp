#include "../utils/Point.cpp"
#include <queue>

class Route {
private:
    std::queue<Point> route;

public:
    Route() noexcept = default;

    Route(std::queue<Point> q) noexcept : route{q} {}
    
};