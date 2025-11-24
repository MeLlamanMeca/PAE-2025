// Robot.hpp
#pragma once
#include "..\point\point.hpp"
#include "..\task\task.hpp"
#include <queue>
#include <deque>
#include <string>


class Robot {
private:
    int id;                     // Identificador único del robot
    Point position;             // Posición actual
    double maxLoad;             // Capacidad máxima de carga
    std::deque<Task> tasks;     // Cola de tareas asignadas
    double desgasteAcumulado = 0.0; // Suma de distancias recorridas * carga transportada (escalado para evitar overflow)

    // TODO, por ahora lo dejo así para simplificarlo
    std::queue<Point> currentPath; // Ruta completa que el sistema le dio

public:
    Robot(int id_, const Point& startPos, double maxLoad_);

    // ---- Métodos de estado ----
    void assignTask(const Task& task, int index);   // Añadir una tarea a la cola en un índice
    bool canCarry(double weight) const;             // Verificar si puede con una carga
    void moveTo(const Point& target);               // Simular movimiento

    /*
        TODO: Por ahora lo dejo así para testear
        Pre:    El punto final del path es el punto "to" de la tarea a realizar
                El path deben ser puntos completamente consecutivos (temporal)
    */
    void setCurrentPath(const std::queue<Point>& path); // Asigna la ruta

    /*
        TODO: Mejorable
        - Si devuelve -1 no está haciendo nada
        - Si devuelve 0 está en trayecto
        - Si devuelve 1 ha terminado el path (necesita que le pasen el siguiente)
    */
    int tick();   // Avanza un punto en currentPath



    // ---- Getters ----
    int getId() const { return id; }
    const Point& getPosition() const { return position; }
    double getMaxLoad() const { return maxLoad; }
    const std::deque<Task>& getTasks() const { return tasks; }
    double getDesgasteAcumulado() const { return desgasteAcumulado; }
    const std::queue<Point>& getCurrentPath() const { return currentPath; }

    // ---- Utilidades ----
    std::string statusString() const;
};