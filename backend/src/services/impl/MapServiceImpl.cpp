#include "MapServiceImpl.h"
//privates

// ===== Asignación de tareas =====

std::pair<int, int> MapServiceImpl::findBestInsertionIndex(const Robot& robot, Task& newTask, Map& map) {
    const auto& tasks = robot.getTasks();
    double bestCost = 1e9;
    int bestIndex = tasks.size();   // por defecto al final

    Point iniPos;
    if (robot.isWorking()) iniPos = robot.getActiveTask().getFin();
    else iniPos = robot.getPosition();

    // Si no hay tareas en la lista
    if (tasks.size() == 0) {
        bestCost = map.getDistance(iniPos, newTask.getIni());
        return {0, bestCost};
    }

    // Si hay tareas en la lista, probar todas las posiciones posibles
    int index = 0;
    for (const auto& t : tasks) {
        double cost = 0.0;

        const auto& currentTask = t.get();

        const Point& prev = (index == 0) ? iniPos : currentTask.getFin();
        const Point& next = (index == tasks.size()) ? newTask.getFin() : currentTask.getIni();

        // Distancia previa -> nueva tarea
        cost += map.getDistance(prev, newTask.getIni());
        // Nueva tarea -> siguiente tarea
        cost += map.getDistance(newTask.getFin(), next);

        if (cost < bestCost) {
            bestCost = cost;
            bestIndex = index;
        }

        ++index;
    }

    return {bestIndex, bestCost};
}


int MapServiceImpl::calculateTotalTaskCost(const Robot& robot, const std::list<std::reference_wrapper<Task>>& tasks, Map& map) {
    if (tasks.empty()) return 0.0;

    int totalCost = 0.0;
    auto it = tasks.begin();
    Task& currentTask = it->get();

    Point iniPos;
    if (robot.isWorking()) iniPos = robot.getActiveTask().getFin();
    else iniPos = robot.getPosition();

    // Primera tarea: desde la posición del robot hasta from, y luego de from a to
    totalCost += map.getDistance(iniPos, currentTask.getIni());
    totalCost += map.getDistance(currentTask.getIni(), currentTask.getFin());

    // Resto de tareas: enlazar desde la to anterior hasta la from actual, y sumar from->to
    Point prevTo = currentTask.getFin();
    ++it;
    for (; it != tasks.end(); ++it) {
        currentTask = it->get();
        totalCost += map.getDistance(prevTo, currentTask.getIni());
        totalCost += map.getDistance(currentTask.getIni(), currentTask.getFin());
        prevTo = currentTask.getFin();
    }

    return totalCost;
}


TaskAssignation MapServiceImpl::taskAssigner(Map& map, Task& task) {
    // Calcular máximos globales (para normalizar)
    double maxCost = 0.0;
    double maxDesgaste = 0.0;
    double maxDistancia = 0.0;

    const auto& robots = map.getRobots();
    std::map<int, InsertionResult> insertionResults;

    // Buscar máximos para normalizar
    for (auto& r : robots) {
        const Robot& robot = r.second.get();
        if (!robot.canCarry(task.getWeight())) continue;

        // Mejor índice para cada robot
        std::pair<int, int> bestIndexResult = findBestInsertionIndex(robot, task, map);

        // Actualizar máximos
        maxCost = std::max(maxCost, double(bestIndexResult.second));        // Coste asociado a la mejor inserción
        maxDesgaste = std::max(maxDesgaste, robot.getLoad());  // Desgaste actual del robot

        // ================================================================================================
        // Copia de la lista de tareas para simular la inserción
        auto tareas = robot.getTasks();
        auto temp;  // temp es una copia de la lista de tareas
        // ================================================================================================
        // Insertar la nueva tarea en la posición óptima
        auto it = temp.begin();
        std::advance(it, bestIndexResult.first);
        temp.insert(it, std::ref(task));
        // ================================================================================================
        // insertar en la posición 0 la tarea actual (si existe)
        // ================================================================================================
        int dist = calculateTotalTaskCost(robot, temp, map);
        maxDistancia = std::max(maxDistancia, double(dist));                // Distancia total tras la inserción 

        // Guardar resultado de inserción
        InsertionResult res = {bestIndexResult.first, double(bestIndexResult.second), dist, robot.getDesgasteAcumulado()};
        insertionResults[robot.getID()] = res;
    }

    // Calcular puntuación normalizada y elegir mejor robot
    double bestScore = 1e9;
    int bestRobot = -1;
    int bestIndex = -1;

    constexpr double W_COST = 1.5;
    constexpr double W_DESGASTE = 1.0;
    constexpr double W_DISTANCIA = 2.0;

    for (auto& r : robots) {
        const Robot& robot = r.second.get();
        if (!robot.canCarry(task.getWeight())) continue;

        InsertionResult res = insertionResults[robot.getID()];  // Analizo resultado guardado

        double normCost = maxCost > 0 ? res.cost / maxCost : 0.0;
        double normDesgaste = maxDesgaste > 0 ? res.desgaste / maxDesgaste : 0.0;
        double normDistancia = maxDistancia > 0 ? res.distancia / maxDistancia : 0.0;

        double score = W_COST * normCost + W_DESGASTE * normDesgaste + W_DISTANCIA * normDistancia;

        if (score < bestScore) {
            bestScore = score;
            bestRobot = robot.getID();
            bestIndex = res.index;
        }
    }

    // Preparar resultado
    if (bestRobot != -1) return TaskAssignation(bestRobot, bestIndex);
    throw std::runtime_error("No se ha encontrado robot disponible para asignar la tarea");
}

// ===== Fin Asignación de tareas =====

//publics

Map& MapServiceImpl::getMap(int mapID) {
    return controllerFactory.getMapController().get(mapID);
}

Map& MapServiceImpl::deleteMap(int mapID) {
    throw std::logic_error("Not implemented");
    //todo
}

TaskAssignation MapServiceImpl::assignTask(Task& task, int mapID) {
    Map& map = controllerFactory.getMapController().get(mapID);

    TaskAssignation result = taskAssigner(map, task);

    Robot& candidate = map.getRobot(result.robotID);
    candidate.addTask(task, result.position);

    return result;
}

void MapServiceImpl::addPoi(int mapID, std::unique_ptr<POI> poi) {
    Map& map = controllerFactory.getMapController().get(mapID);
    map.addPoi(std::move(poi));
}

void MapServiceImpl::addRobot(int mapID, Robot& r) {
    Map& map = controllerFactory.getMapController().get(mapID);
    map.addRobot(r);
}

void MapServiceImpl::createForbidenZone(int mapID, Point tl, Point br) {
    Map& map = controllerFactory.getMapController().get(mapID);
    map.createForbidenZone(tl, br);
}

void MapServiceImpl::createSlowZone(int mapID, Point tl, Point br) {
    Map& map = controllerFactory.getMapController().get(mapID);
    map.createSlowZone(tl, br);
}

void MapServiceImpl::createCommonZone(int mapID, Point tl, Point br) {
    Map& map = controllerFactory.getMapController().get(mapID);
    map.createCommonZone(tl, br);
}

Map& MapServiceImpl::createMap(int mapID, std::vector<std::vector<unsigned char>> map) {
    return controllerFactory.getMapController().create(mapID, map);
}

void MapServiceImpl::deletePoi(int mapID, Point position) {
    Map& map = controllerFactory.getMapController().get(mapID);
    map.deletePoi(position);
}
