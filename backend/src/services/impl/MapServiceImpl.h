#pragma once
#include "../MapService.h"
#include "../../controllers/ControllerFactory.h"
#include "../../common/Point.h"
#include "../../common/Route.h"
#include "../../POIs/POI.h"
#include "../../POIs/POIType.h"
#include "../../common/InsertionResult.h"
#include <vector>

class MapServiceImpl : public MapService {
    private:
        ControllerFactory& controllerFactory;

        // ===== Privates: Asignación de tareas =====
        static std::pair<int, int> findBestInsertionIndex(const Robot& robot, Task& newTask, Map& map);
        static int calculateTotalTaskCost(const Robot& robot, const std::list<std::reference_wrapper<Task>>& tasks, Map& map);
        static TaskAssignation taskAssigner(Map& map, Task& task);

    public:
        MapServiceImpl(ControllerFactory& cf) : controllerFactory(cf) {}
        Map& get(int mapID) override;
        TaskAssignation assignTask(Task& task, int mapID) override;
        void addPoi(int mapID, POI& poi) override;
        void addRobot(int mapID, Robot& r) override;
        void createForbidenZone(int mapID, Point tl, Point br) override;
        void createSlowZone(int mapID, Point tl, Point br) override;
        void createCommonZone(int mapID, Point tl, Point br) override;
        void createMap(int mapID, std::vector<std::vector<char>> map) override;
        void deletePoi(int mapID, Point position) override;
};