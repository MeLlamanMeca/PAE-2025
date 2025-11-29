#pragma once
#include <memory>
#include <nlohmann/json.hpp>
#include "../services/ServiceFactory.h"
#include "../common/TaskAssignation.h"

class App {
    private:
        ServiceFactory& serviceFactory;

    public:
        explicit App(ServiceFactory& sf) :  serviceFactory(sf) {}

        TaskAssignation createTask(int mapID, Point ini, Point fin);
        void deleteTask(int robotID, int taskID);

        void endCurrentTask(int robotID);
        void startTask(int robotID);
        void updateRobotPosition(int robotID, Point pos);

        void createRobot(int mapID, Point position, int maxWeight);
        void deleteRobot(int robotID);

        void createCommonPoi(int mapID, Point position, std::string name);
        void deletePoi(int mapID, Point position);
        
        
        void createForbidenZone(int mapID, Point tl, Point br);
        void createSlowZone(int mapID, Point tl, Point br);
        void createCommonZone(int mapID, Point tl, Point br);

        void createMap(int mapID, std::vector<std::vector<unsigned char>> map);
        void deleteMap(int mapID);

        Task& getTask(int taskID);
        Robot& getRobot(int robotID);
        Map& getMap(int mapID);
};
