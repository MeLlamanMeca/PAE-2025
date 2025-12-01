#pragma once
#include "../RobotController.h"
#include "../../robots/states/impl/StandByState.h"

class RobotControllerImpl : public RobotController {
    private:
        int id = 0;
        std::map<int, Robot> robots;

    public:
        Robot& get(int robotID) { 
            auto it = robots.find(robotID);
            if (it != robots.end()) return it->second;
            throw std::out_of_range("Robot ID not found");
         }

        Robot& create(Point position, Map& map, int maxWeight) {
            auto result = robots.emplace(std::piecewise_construct,
                        std::forward_as_tuple(id),
                        std::forward_as_tuple(position, id, map, maxWeight));
            id++;
            if (!result.second) throw std::runtime_error("Robot ID already exists");
            result.first->second.setState(std::make_unique<StandByState>());
            return result.first->second;
        }

        void deleteRobot(int robotID) { 
            auto it = robots.find(robotID);
            if (it != robots.end()) robots.erase(it);
            else throw std::out_of_range("Robot ID not found"); 
        }
};