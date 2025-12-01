#pragma once
#include <vector>
#include "../common/Map.h"

class MapController {
    public:
        virtual Map& get(int mapID) = 0;
        virtual Map& create(int mapID, std::vector<std::vector<unsigned char>> map) = 0;
        virtual void deleteMap(int mapID) = 0;
};