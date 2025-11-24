#include "AppAdapter.h"
#include <iostream>

// ----------------------------------------------------------------------------------------
// PRIVATE FUNCTIONS ----------------------------------------------------------------------
// ----------------------------------------------------------------------------------------

void AppAdapter::broadcastMessage(const nlohmann::json& msg) {
    if (sendMessageCallback) sendMessageCallback(msg);
}

void AppAdapter::handleEndTask(const nlohmann::json& msg) {
    // parsear el mensaje
    // app.endTask(34,5)
    // enviar mensaje al front con:
    //     - nueva ruta
    //     - id de la task asignada
}

void AppAdapter::handleCreateTask(const nlohmann::json& msg) {
    // parsear el mensaje
    // llamar a controller.createTask
    // enviar mensaje al front con:
    //     - nueva tarea creada
    //     - robot asignado y posicion
}

void AppAdapter::handleDeleteTask(const nlohmann::json& msg) {
//  TODO
}

void AppAdapter::handleUpdateRobotPosition(const nlohmann::json& msg) {
    //  TODO
}

// ----------------------------------------------------------------------------------------
// PUBLIC FUNCTIONS -----------------------------------------------------------------------
// ----------------------------------------------------------------------------------------

void AppAdapter::setSendMessageCallback(std::function<void(const nlohmann::json&)> cb) {
    sendMessageCallback = std::move(cb);
}

void AppAdapter::handleMessage(const nlohmann::json& msg) {
    // confirma que el mensaje es un json con el formato correcto
    // llama a la funcion correcta según el "type" del json
    try {
        std::string type = msg.at("type").get<std::string>();

        if (type == "createTask") {
            handleCreateTask(msg);
        } else if (type == "endTask") {
            handleEndTask(msg);
        } else if (type == "deleteTask") {
            handleDeleteTask(msg);
        } else if (type == "updateRobotPosition") {
            handleUpdateRobotPosition(msg);
        } else {
            std::cerr << "Unknown message type: " << type << "\n";
        }
    } catch (const std::exception& e) {
        std::cerr << "Error handling message: " << e.what() << "\n";
    }
}
