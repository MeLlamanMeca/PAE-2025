#pragma once

#include "../POIType.h"
#include "../POI.h"
#include "../../common/Point.h"

class ChargingBayPOI : public POI {
    public:
        ChargingBayPOI(Point pos) : POI(pos, "ChargingZone") {}

        const POIType getType() const override { return POIType::CHARGINGBAY; }
};