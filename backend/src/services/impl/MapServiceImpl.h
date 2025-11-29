#pragma once
#include "../MapService.h"
#include "../../controllers/ControllerFactory.h"
#include "../../common/Point.h"
#include "../../common/Route.h"
#include "../../POIs/POI.h"
#include "../../POIs/POIType.h"
#include <vector>

class MapServiceImpl : public MapService {
    private:
        ControllerFactory& controllerFactory;

    public:
        MapServiceImpl(ControllerFactory& cf) : controllerFactory(cf) {}
        Map& getMap(int mapID) override;
        TaskAssignation assignTask(Task& task, int mapID) override;
        void addPoi(int mapID, std::unique_ptr<POI> poi) override;
        void addRobot(int mapID, Robot& r) override;
        void createForbidenZone(int mapID, Point tl, Point br) override;
        void createSlowZone(int mapID, Point tl, Point br) override;
        void createCommonZone(int mapID, Point tl, Point br) override;
        Map& createMap(int mapID, std::vector<std::vector<unsigned char>> map) override;
        Map& deleteMap(int mapID) override;
        void deletePoi(int mapID, Point position) override;
};