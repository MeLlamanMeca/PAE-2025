#pragma once
#include "../robot/robot.hpp"
#include "../task/task.hpp"
#include "../point/point.hpp"
#include <unordered_map>
#include <vector>
#include <deque>
#include <string>
#include <utility>
#include <cmath>

struct InsertionResult {
    int index;
    double cost;
};

class TaskAssigner {
private:
    std::unordered_map<std::string, double> distanceCache;
    int reusedDistances = 0;

    // Devuelve una clave única para cachear distancias
    std::string keyFor(const Point& a, const Point& b) const;

public:
    TaskAssigner() = default;

    // Calcula distancia mínima (temporalmente euclídea)
    double getDistanceCached(const Point& a, const Point& b);

    // Busca el mejor índice donde insertar una tarea en un robot
    InsertionResult findBestInsertionIndex(const Robot& robot, const Task& newTask);

    // Calcula el coste total de las tareas de un robot
    double calculateTotalTaskCost(const Point& robotPosition, const std::deque<Task>& tasks);

    // Asignador principal (greedy)
    void assignTasksGreedy(std::vector<Robot>& robots, std::vector<Task>& tasks);

    // Limpia cache
    void clearCache();

    // Tamaño del cache
    size_t cacheSize() const { return distanceCache.size(); }

    // Muestra el contenido del caché de distancias
    void printCache() const;

    int getReusedDistances() const { return reusedDistances; }

    void incrementReusedDistances() { ++reusedDistances; }
};
