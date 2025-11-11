#include "task.hpp"
#include "..\point\point.hpp"
#include <iostream>

int main() {
    std::cout << "--- TEST TASK ---\n";

    Point from(1, 2);
    Point to(4, 6);
    double weight = 25.0;

    Task t1(101, from, to, weight);

    std::cout << "Task ID: " << t1.id << "\n";
    std::cout << "From: (" << t1.from.x << "," << t1.from.y << ")\n";
    std::cout << "To: (" << t1.to.x << "," << t1.to.y << ")\n";
    std::cout << "Weight: " << t1.weight << "\n";

    return 0;
}