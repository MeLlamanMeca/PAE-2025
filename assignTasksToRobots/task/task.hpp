// Task.hpp
#pragma once
#include "..\point\point.hpp"
#include <string>

struct Task {
    int id = 0;           // identificador único
    Point from;           // punto de recogida
    Point to;             // punto de entrega
    double weight = 0.0;  // peso de la carga
    bool inProgress = false; // indica si la tarea está en curso

    Task() = default;
    Task(int id_, Point f, Point t, double w)
        : id(id_), from(f), to(t), weight(w) {}

    std::string toString() const {
        return "Task " + std::to_string(id) +
                " [from=(" + std::to_string(from.x) + "," + std::to_string(from.y) + 
                "), to=(" + std::to_string(to.x) + "," + std::to_string(to.y) + 
                "), weight=" + std::to_string(weight) + "]";
    }

    bool isInProgress() const {
        return inProgress;
    }

    void setInProgress() {
        inProgress = true;
    }
};
