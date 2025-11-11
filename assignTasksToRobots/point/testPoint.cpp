#include "point.hpp"
#include <iostream>
#include <cmath>

int main() {
    std::cout << "--- TEST POINT ---\n";

    Point p1(1, 2);
    Point p2(3, 5);
    Point p3(1, 2);

    // Comparaciones
    std::cout << "p1 == p2 ? " << (p1 == p2) << " (esperado: 0)\n";
    std::cout << "p1 == p3 ? " << (p1 == p3) << " (esperado: 1)\n";

    // Distancia
    double dist = p1.distanceTo(p2);
    std::cout << "Distancia p1->p2: " << dist << " (esperado: " << std::sqrt(13) << ")\n";

    return 0;
}