// Robot.cpp
#include "robot.hpp"
#include <iostream>
#include <sstream>
#include <cmath>

// Constructor
Robot::Robot(int id_, const Point& startPos, double maxLoad_)
    : id(id_), position(startPos), maxLoad(maxLoad_) {}

// Añadir una tarea a la cola en un índice concreto
void Robot::assignTask(const Task& task, int index) {
    if (!canCarry(task.weight)) {
        std::cerr   << "ERROR: Robot " << id 
                    << " no puede cargar tarea " << task.id 
                    << " (peso " << task.weight << " > capacidad " << maxLoad << ")\n";
        return;
    }

    if (index < 0 || index > static_cast<int>(tasks.size())) {
        // Añadir al final
        tasks.push_back(task);
    } else {
        // Insertar en índice específico
        auto it = tasks.begin() + index;  // deque soporta iteradores aleatorios
        tasks.insert(it, task);
    }
}


// Verificar si puede con una carga
bool Robot::canCarry(double weight) const {
    return weight <= maxLoad;
}

// Añade la nueva ruta al robot
void Robot::setCurrentPath(const std::queue<Point>& path) {
    currentPath = path;
}

// Simular movimiento a un punto y acumular desgaste
void Robot::moveTo(const Point& target) {
    // TODO: Temporalmente distancia Euclídea
    double distance = position.distanceTo(target);
    double weight = tasks.empty() ? 0.0 : tasks.front().weight;

    // Actualizo desgaste (lo acumularé en el task assigner)
    // desgasteAcumulado += distance * weight * 0.001; // Escalado temporal
    
    // Actualizo posición
    position = target;
}

// Update
int Robot::tick() {
    if (currentPath.empty() || tasks.empty()) return -1;

    // Avanzar un punto en la ruta
    Point next = currentPath.front();
    currentPath.pop();

    moveTo(next);

    // Si hemos llegado al final de la ruta
    if (currentPath.empty()) {
        Task completed = tasks.front();
        tasks.pop_front();
        std::cout << "Robot " << id << " completo tarea " << completed.id << "\n";
        return 1;
    }

    return 0;
}


// Devolver una cadena con el estado actual
std::string Robot::statusString() const {
    std::ostringstream ss;
    ss  << "Robot " << id
        << " | Pos=(" << position.x << "," << position.y << ")"
        << " | CargaMax=" << maxLoad
        << " | Tareas=" << tasks.size()
        << " | Desgaste=" << desgasteAcumulado
        << " | PathSize=" << currentPath.size();
    return ss.str();
}
