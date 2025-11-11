#include "taskAssigner.hpp"
#include <iostream>
#include <cmath>

// -------------------------------------------------------------------------------------------------
// Genera clave para cache
// -------------------------------------------------------------------------------------------------
std::string TaskAssigner::keyFor(const Point& a, const Point& b) const {
    // Orden lexicográfico: primero por x, luego por y
    const Point& p1 = (a.x < b.x || (a.x == b.x && a.y <= b.y)) ? a : b;
    const Point& p2 = (&p1 == &a) ? b : a;

    return  std::to_string(p1.x) + "," + std::to_string(p1.y) + "|" +
            std::to_string(p2.x) + "," + std::to_string(p2.y);
}

// -------------------------------------------------------------------------------------------------
// Devuelve la distancia mínima entre dos puntos
// Coste: A* o O(1)
// -------------------------------------------------------------------------------------------------

// --- Implementación temporal de la distancia euclídea ---
std::pair<std::vector<Point>, double> getDistance(const Point& a, const Point& b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return {std::vector<Point>{}, std::sqrt(dx * dx + dy * dy)};
}

double TaskAssigner::getDistanceCached(const Point& a, const Point& b) {
    if (a == b) return 0.0;
    
    std::string key = keyFor(a, b);
    auto it = distanceCache.find(key);
    if (it != distanceCache.end()) {
        incrementReusedDistances();
        return it->second;
    }

    // Aquí debería llamarse a la función real del A* que calcule la distancia

    std::pair<std::vector<Point>, double> d = getDistance(a, b);

    distanceCache[key] = d.second;
    return d.second;
}

// -------------------------------------------------------------------------------------------------
// Busca mejor índice para insertar tarea
// Coste: O(n) donde n = tamaño de la lista de tareas del robot
// -------------------------------------------------------------------------------------------------
InsertionResult TaskAssigner::findBestInsertionIndex(const Robot& robot, const Task& newTask) {
    const auto& tasks = robot.getTasks();
    double bestCost = 1e9;
    int bestIndex = tasks.size();   // por defecto al final

    for (int i = 0; i <= static_cast<int>(tasks.size()); ++i) {
        double cost = 0.0;

        Point prev = (i == 0) ? robot.getPosition() : tasks[i - 1].to;
        Point next = (i == tasks.size()) ? newTask.to : tasks[i].from;

        // Distancia previa -> nueva tarea
        cost += getDistanceCached(prev, newTask.from);
        // Nueva tarea -> siguiente tarea
        cost += getDistanceCached(newTask.to, next);

        if (cost < bestCost) {
            bestCost = cost;
            bestIndex = i;
            // Si ya hay una tarea en curso en el índice 0, no se puede insertar ahí
            if (bestIndex == 0 && !tasks.empty() && tasks[0].isInProgress()) {
                bestIndex = tasks.size();
            }
        }
    }

    return {bestIndex, bestCost};
}

// -------------------------------------------------------------------------------------------------
// Calcula el coste total de las tareas de un robot
// Coste: O(n) donde n = tamaño de la lista de tareas del robot
// -------------------------------------------------------------------------------------------------
double TaskAssigner::calculateTotalTaskCost(const Point& robotPosition, const std::deque<Task>& tasks) {
    if (tasks.empty()) return 0.0;

    double totalCost = 0.0;
    auto it = tasks.begin();

    // Primera tarea: desde la posición del robot hasta from, y luego de from a to
    totalCost += getDistanceCached(robotPosition, it->from);
    totalCost += getDistanceCached(it->from, it->to);

    // Resto de tareas: enlazar desde la to anterior hasta la from actual, y sumar from->to
    Point prevTo = it->to;
    ++it;
    for (; it != tasks.end(); ++it) {
        totalCost += getDistanceCached(prevTo, it->from);
        totalCost += getDistanceCached(it->from, it->to);
        prevTo = it->to;
    }

    return totalCost;
}


// -------------------------------------------------------------------------------------------------
// Asignación greedy de tareas
// Coste: = O(t * r * l) donde t = número de tareas
//                             r = número de robots
//                             l = media del tamaño de la lista de tareas por robot
// -------------------------------------------------------------------------------------------------
// A mejorar:
//  - La asignación depende mucho de dónde empiezan los robots por lo que un buen posicionamiento inicial dará mejores resultados
//  - Actualmente el desgaste no se tiene en cuenta porque no se actualiza (no hay movimientos)
//  - La distancia se calcula de forma euclídea, debería ser A*
//  - La asignación parece buena pero no se garantiza que todos los robots trabajen de forma equilibrada
//  - Hay que limitar la asignación de tareas solo a índices válidos, es decir, si una tarea está en marcha en el índice 0 no se puede insertar ahí
// -------------------------------------------------------------------------------------------------
void TaskAssigner::assignTasksGreedy(std::vector<Robot>& robots, std::vector<Task>& tasks) {
    // Ordenar tareas?

    for (auto& task : tasks) {
        // DEBUG
        std::cout   << "\n--- Tarea " << task.id << " (ID: " << task.id
                    << ", peso=" << task.weight << ") ---\n";
        // DEBUG END


        // TODO: Temporalmente hago cálculos que podrían ser costosos para ver que todo funciona
        // ==============================================
        // Calcular máximos globales (para normalizar)
        // ==============================================
        double maxCost = 0.0;
        double maxDesgaste = 0.0;   // Actualmente el desgaste es 0 siempre porque no se actualiza, no hay movimientos
        double maxDistancia = 0.0;

        for (auto& robot : robots) {
            if (!robot.canCarry(task.weight)) continue;

            InsertionResult res = findBestInsertionIndex(robot, task);
            maxCost = std::max(maxCost, res.cost);
            maxDesgaste = std::max(maxDesgaste, robot.getDesgasteAcumulado());

            auto temp = robot.getTasks();
            temp.insert(temp.begin() + res.index, task);
            double dist = calculateTotalTaskCost(robot.getPosition(), temp);
            maxDistancia = std::max(maxDistancia, dist);
        }

        // DEBUG
        std::cout   << "  [Maximos] Cost=" << maxCost
                    << ", Desgaste=" << maxDesgaste
                    << ", Distancia=" << maxDistancia << "\n";
        // DEBUG END

        // ==============================================
        // Calcular puntuación normalizada y elegir mejor robot
        // ==============================================
        double bestScore = 1e9;
        Robot* bestRobot = nullptr;
        int bestIndex = -1;

        constexpr double W_COST = 1.5;
        constexpr double W_DESGASTE = 1.0;
        constexpr double W_DISTANCIA = 2.0;

        for (auto& robot : robots) {
            // DEBUG
            std::cout << "  Evaluando Robot " << robot.getId() << "...\n";
            // DEBUG END

            if (!robot.canCarry(task.weight)) {
                std::cout << "  - Robot " << robot.getId() << " descartado (peso)\n";
                continue;
            }

            InsertionResult res = findBestInsertionIndex(robot, task);
            auto temp = robot.getTasks();
            temp.insert(temp.begin() + res.index, task);
            double dist = calculateTotalTaskCost(robot.getPosition(), temp);

            double normCost = maxCost > 0 ? res.cost / maxCost : 0.0;
            double normDesgaste = maxDesgaste > 0 ? robot.getDesgasteAcumulado() / maxDesgaste : 0.0;
            double normDistancia = maxDistancia > 0 ? dist / maxDistancia : 0.0;

            double score = W_COST * normCost + W_DESGASTE * normDesgaste + W_DISTANCIA * normDistancia;

            if (score < bestScore) {
                bestScore = score;
                bestRobot = &robot;
                bestIndex = res.index;
            }


            std::cout   << "  Robot " << robot.getId()
                        << " -> cost=" << res.cost
                        << " (n=" << normCost << "), desg=" << robot.getDesgasteAcumulado()
                        << " (n=" << normDesgaste << "), dist=" << dist
                        << " (n=" << normDistancia << ") => score=" << score
                        << " (index=" << res.index << ")\n";
        }

        // ==============================================
        // Asignar tarea al mejor robot
        // ==============================================
        if (bestRobot) {
            // DEBUG
            std::cout   << " Mejor robot: " << bestRobot->getId()
                        << " con score=" << bestScore
                        << " (insertado en indice " << bestIndex << ")\n";
            // DEBUG END
            bestRobot->assignTask(task, bestIndex);
        } else {
            std::cerr << "No hay robot disponible para tarea " << task.id << "\n";
        }

        // DEBUG
        std::cout << "\n========== FIN ASIGNACION ==========\n";
        // DEBUG END
    }
}

// -------------------------------------------------------------------------------------------------
// Limpia la cache
// Coste: O(1)
// -------------------------------------------------------------------------------------------------
void TaskAssigner::clearCache() {
    distanceCache.clear();
}


// -------------------------------------------------------------------------------------------------
// Muestra el contenido del caché de distancias
// -------------------------------------------------------------------------------------------------
void TaskAssigner::printCache() const {
    std::cout << "\n[CACHE DE DISTANCIAS] (" << distanceCache.size() << " entradas)\n";

    if (distanceCache.empty()) {
        std::cout << "  (vacío)\n";
        return;
    }

    int count = 0;
    for (const auto& pair : distanceCache) {
        std::cout << "  [" << (count + 1) << "] " << pair.first << " => " << pair.second << "\n";
        if (++count >= 30) {
            std::cout << "  ... (mostrando primeras 30 de " << distanceCache.size() << ")\n";
            break;
        }
    }
}