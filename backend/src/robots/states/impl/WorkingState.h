#pragma once

#include <memory>
#include <stdexcept>
#include <nlohmann/json.hpp>
#include "../RobotState.h"
#include "../../../tasks/Task.h"   
#include "../../../common/Route.h"

class Robot;
class StandByState;

using json = nlohmann::json;

class WorkingState : public RobotState {
private:
    Task active;
    Route route;

public: 
    // Constructor (se puede mantener inline)
    WorkingState(Task active, Route route) : active(active), route(route) {}

    // Métodos virtuales y getters
    StateType getState() const override;
    void to_json(nlohmann::json& j) const override;
    
    // Declaraciones de funciones que necesitan tipos completos (se implementan en .cpp)
    void endCurrentTask(Robot& robot) override;
    void startTask(Robot& robot) override;

    Task& getActiveTask() const;
    Route& getActiveRoute() const;
};

// Función de serialización to_json (helper)
inline void to_json(json& j, const WorkingState& p) {
    p.to_json(j);
}