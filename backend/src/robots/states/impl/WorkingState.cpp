#include "./WorkingState.h" // La declaración de nuestra propia clase

#include "../../Robot.h" 
#include "./StandByState.h"

StateType WorkingState::getState() const { 
    return StateType::WORKING; 
}

void WorkingState::to_json(nlohmann::json& j) const {
    // Asumiendo que StateTypeToString está en un archivo incluido por RobotState.h
    j = json{
        {"state", StateTypeToString(getState())},
        {"task", active},
        {"route", route}
    };
}

void WorkingState::endCurrentTask(Robot& robot) {
    // La implementación del cambio de estado reside aquí.
    robot.setState(std::make_unique<StandByState>());
}

void WorkingState::startTask(Robot& robot) {
    throw std::runtime_error("Task in progress.");
}

// **Advertencia:** El uso de const_cast es generalmente un indicador de mal diseño.
// Es mejor modificar la clase RobotState o Robot para permitir acceso no constante 
// o reevaluar la necesidad de modificar el estado en WorkingState.
Task& WorkingState::getActiveTask() const {
    return const_cast<Task&>(active);
}

Route& WorkingState::getActiveRoute() const {
    return const_cast<Route&>(route);
}