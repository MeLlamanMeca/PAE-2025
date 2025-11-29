#pragma once

enum class StateType {
    WORKING,
    STANDBY
};

inline auto StateTypeToString = [](StateType t) -> std::string {
    switch (t) {
        case StateType::WORKING: return "working";
        case StateType::STANDBY: return "standby";
        default: return "unknown";
    }
};