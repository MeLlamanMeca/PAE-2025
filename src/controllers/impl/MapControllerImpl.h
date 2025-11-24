#pragma once
#include "../MapController.h"

class MapControllerImpl : public MapController {
    private:
        std::map<int,Map> maps;
        
    public:
        Map& get(int mapID) override {
            if (maps.find(mapID) != maps.end()) return maps.at(mapID);
            else throw std::out_of_range("Map ID not found");
        }

        Map& create(int mapID, std::vector<std::vector<char>> map) override {
            auto result = maps.emplace(mapID, Map(map));
            if (!result.second) throw std::invalid_argument("Map ID already exists");
            return result.first->second;
        }

        void deleteMap(int mapID) {
            auto it = maps.find(mapID);
            if (it != maps.end()) maps.erase(it);
            else throw std::out_of_range("Map ID not found");
        }
    
};