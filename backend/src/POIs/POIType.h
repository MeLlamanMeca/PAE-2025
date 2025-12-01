#pragma once
#include <string>

enum class POIType {
    COMMON,
    CHARGINGBAY
};

inline auto POITypeToString = [](POIType t) -> std::string {
            switch (t) {
                case POIType::COMMON: return "common";
                case POIType::CHARGINGBAY: return "chargingbay";
                default: return "unknown";
            }
        };