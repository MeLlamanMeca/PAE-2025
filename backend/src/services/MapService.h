#pragma once

#include <vector>
#include <map>
#include <list>
#include "../common/Route.h"
#include "../POIs/POI.h"
#include "../POIs/POIType.h"
#include "../common/Map.h"
#include "../common/TaskAssignation.h"

class MapService {

    public:
        virtual Map& getMap(int mapID) = 0;
        virtual Map& createMap(int mapID, std::vector<std::vector<unsigned char>> map) = 0;
        virtual Map& deleteMap(int mapID) = 0;

        virtual TaskAssignation assignTask(Task& task, int mapID) = 0;
        virtual void addPoi(int mapID, std::unique_ptr<POI> poi) = 0;
        virtual void deletePoi(int mapID, Point position) = 0;
        virtual void addRobot(int mapID, Robot& r) = 0;
        virtual void createForbidenZone(int mapID, Point tl, Point br) = 0;
        virtual void createSlowZone(int mapID, Point tl, Point br) = 0;
        virtual void createCommonZone(int mapID, Point tl, Point br) = 0;
        
};