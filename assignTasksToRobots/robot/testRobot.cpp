#include "robot.hpp"
#include "..\task\task.hpp"
#include "..\point\point.hpp"
#include <iostream>
#include <vector>
#include <queue>

int main() {
    std::cout << "--- TEST ROBOT ---\n";

    // Crear robot
    Robot r1(1, Point(0,0), 50.0);

    // Crear tareas
    Task t1(101, Point(1,2), Point(4,6), 20.0);
    Task t2(102, Point(5,5), Point(2,3), 60.0);

    // Asignar tareas
    r1.assignTask(t1, 0);
    r1.assignTask(t2, 1);   // Debería dar error al intentar asignar

    // Crear rutas para las tareas (queue)
    // La primera va desde el punto inicial del robot hasta el final de la tarea1
    std::queue<Point> path1;
    path1.push(Point(0,1));
    path1.push(Point(1,1));
    path1.push(t1.from);
    path1.push(Point(2,2));
    path1.push(Point(3,2));
    path1.push(Point(4,2));
    path1.push(Point(4,3));
    path1.push(Point(4,4));
    path1.push(Point(4,5));
    path1.push(t1.to);

    // Ejecutar path
    r1.setCurrentPath(path1);
    while (!r1.getCurrentPath().empty()) {
        r1.tick();
        std::cout << r1.statusString() << "\n";
    }

    return 0;
}