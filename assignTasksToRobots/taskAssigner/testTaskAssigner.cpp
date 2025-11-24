#include "taskAssigner.hpp"
#include "..\robot\robot.hpp"
#include "..\task\task.hpp"
#include "..\point\point.hpp"
#include <iostream>
#include <vector>

int main() {
    std::cout << "==== TEST: ASIGNADOR DE TAREAS ====\n\n";

    // Crear robots
    std::vector<Robot> robots = {
        Robot(1, Point(0, 0), 20.0),
        Robot(2, Point(10, 10), 10.0),
        Robot(3, Point(5, 5), 30.0)
    };

    // Crear tareas
    std::vector<Task> tasks = {
        Task(101, Point(1, 1), Point(3, 3), 10.0),
        Task(102, Point(6, 5), Point(7, 7), 20.0),
        Task(103, Point(9, 8), Point(12, 10), 15.0),
        Task(104, Point(2, 3), Point(4, 4), 10.0),
        Task(105, Point(8, 1), Point(10, 2), 5.0)
    };

    // Crear el asignador
    TaskAssigner assigner;

    // Asignar tareas con greedy
    assigner.assignTasksGreedy(robots, tasks);

    // Mostrar estado final
    std::cout << "\n---- ESTADO FINAL DE LOS ROBOTS ----\n";
    for (const auto& r : robots) {
        std::cout << r.statusString() << "\n";
        const auto& rtasks = r.getTasks();
        for (const auto& t : rtasks) {
            std::cout   << "   Tarea " << t.id 
                        << " (" << t.from.x << "," << t.from.y << " -> " 
                        << t.to.x << "," << t.to.y << ") peso=" << t.weight
                        << "\n";
        }
        std::cout << "-----------------------------------\n";
    }

    std::cout   << "\nDistancias calculadas en cache: " 
                << assigner.cacheSize() << "\n";
    std::cout   << "Distancias reutilizadas desde cache: " 
                << assigner.getReusedDistances() << "\n";

    assigner.printCache();

    std::cout << "\n==== FIN TEST ====\n";
}