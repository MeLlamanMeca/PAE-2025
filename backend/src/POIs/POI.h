#pragma once

#include "POIType.h"
#include "../common/Point.h"

class POI {
    private:
        Point pos;
        std::string name;
        int ID;

    protected:
        POI(Point pos, std::string name) : pos(pos), name(name), ID(-1) {}
    
    public:
        virtual const POIType getType() const = 0;
        Point getPos() const { return pos; }
        std::string getName() const { return name; }
        void setID(int id) { ID = id; }
        int getID() const { return ID; }
};