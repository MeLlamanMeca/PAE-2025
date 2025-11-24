#pragma once

#include "../POIType.h"
#include "../POI.h"
#include "../../common/Point.h"

class CommonPOI : public POI {
    public:
        CommonPOI(Point pos, std::string name) : POI(pos, name) {}

        const POIType getType() const override { return POIType::COMMON; }
};