#include "AppAdapter.h"
#include <iostream>
#include "../common/Point.h"
#include "../common/TaskAssignation.h"
#include <string>

// ----------------------------------------------------------------------------------------
// PRIVATE FUNCTIONS ----------------------------------------------------------------------
// ----------------------------------------------------------------------------------------

void AppAdapter::broadcastMessage(const nlohmann::json& msg) {
    if (sendMessageCallback) sendMessageCallback(msg);
}

void AppAdapter::handleCreateTask(const nlohmann::json& content) {
    try {
        int mapID = content.at("mapID").get<int>();
        auto pointIni = content.at("PointIni");
        auto pointFin = content.at("PointFin");
        Point ini = Point(pointIni.at("x").get<int>(), pointIni.at("y").get<int>());
        Point fin = Point(pointFin.at("x").get<int>(), pointFin.at("y").get<int>());

        TaskAssignation result = app.createTask(mapID, ini, fin);
        
        //sendMessageCallback(/*todo*/) enviar datos de tarea + posicion

    } catch (const std::exception& e) {
        std::cerr << "Error parsing createTask message: " << e.what() << "\n";
    }
}

void AppAdapter::handleDeleteTask(const nlohmann::json& content) {
    try {
        int robotID = content.at("robotID").get<int>();
        int taskID = content.at("taskID").get<int>();

        app.deleteTask(robotID, taskID);

        //sendConfirmation
    }
    catch (const std::exception& e){
        std::cerr << "Error parsing handleDeleteTask message: " << e.what() << "\n";
    }
    
}

void AppAdapter::handleEndCurrentTask(const nlohmann::json& content) {
    try {
        int robotID = content.at("robotID").get<int>();

        app.endCurrentTask(robotID);

        //sendConfirmation
    }
    catch (const std::exception& e){
        std::cerr << "Error parsing handleEndCurrentTask message: " << e.what() << "\n";
    }
    
}

void AppAdapter::handleStartTask(const nlohmann::json& content) {
    try {
        int robotID = content.at("robotID").get<int>();

        app.startTask(robotID);
        //Task& t = app.getRobot(robotID).getCurrentTask();
        //TODO
        //sendremoveTask(taskID), sendactiveTask(robotID, taskInfo)
    }
    catch (const std::exception& e){
        std::cerr << "Error parsing handleEndCurrentTask message: " << e.what() << "\n";
    }
}

void AppAdapter::handleUpdateRobotPosition(const nlohmann::json& content) {
    try {
        int robotID = content.at("robotID").get<int>();
        auto position = content.at("position");
        Point point = Point(position.at("x").get<int>(), position.at("y").get<int>());

        app.updateRobotPosition(robotID, point);

        //sendConfirmation
    }
    catch (const std::exception& e){
        std::cerr << "Error parsing handleUpdateRobotPosition message: " << e.what() << "\n";
    }
}

void AppAdapter::handleCreateRobot(const nlohmann::json& content) {
    try {
        int mapID = content.at("mapID").get<int>();
        auto position = content.at("position");
        Point point = Point(position.at("x").get<int>(), position.at("y").get<int>());

        app.createRobot(mapID, point);

        //not implemented in live
    }
    catch (const std::exception& e){
        std::cerr << "Error parsing handleCreateRobot message: " << e.what() << "\n";
    }
}
void AppAdapter::handleDeleteRobot(const nlohmann::json& content) {
    try {
        int robotID = content.at("robotID").get<int>();

        app.deleteRobot(robotID);

        //not implemented
    }
    catch (const std::exception& e){
        std::cerr << "Error parsing handleDeleteRobot message: " << e.what() << "\n";
    }
}

void AppAdapter::handleCreateCommonPoi(const nlohmann::json& content) {
    try {
        int mapID = content.at("mapID").get<int>();
        auto position = content.at("position");
        Point point = Point(position.at("x").get<int>(), position.at("y").get<int>());
        std::string name = content.at("name").get<std::string>();

        app.createCommonPoi(mapID, point, name);

        //send poicreated()...
    }
    catch (const std::exception& e){
        std::cerr << "Error parsing handleCreateCommonPoi message: " << e.what() << "\n";
    }
}

void AppAdapter::handleDeletePoi(const nlohmann::json& content) {
    try {
        int mapID = content.at("mapID").get<int>();
        auto position = content.at("position");
        Point point = Point(position.at("x").get<int>(), position.at("y").get<int>());

        app.deletePoi(mapID, point);

        //send poideleted()...
    }
    catch (const std::exception& e){
        std::cerr << "Error parsing handleDeletePoi message: " << e.what() << "\n";
    }
}

void AppAdapter::handleCreateForbidenZone(const nlohmann::json& content) {
    try {
        int mapID = content.at("mapID").get<int>();
        auto tl = content.at("tl");
        auto br = content.at("br");
        Point tlPoint = Point(tl.at("x").get<int>(), tl.at("y").get<int>());
        Point brPoint = Point(br.at("x").get<int>(), br.at("y").get<int>());

        app.createForbidenZone(mapID, tlPoint, brPoint);

        //send forbidenZoneCreated()...
    }
    catch (const std::exception& e){
        std::cerr << "Error parsing handleDeletePoi message: " << e.what() << "\n";
    }
}

void AppAdapter::handleCreateSlowZone(const nlohmann::json& content) {
    try {
        int mapID = content.at("mapID").get<int>();
        auto tl = content.at("tl");
        auto br = content.at("br");
        Point tlPoint = Point(tl.at("x").get<int>(), tl.at("y").get<int>());
        Point brPoint = Point(br.at("x").get<int>(), br.at("y").get<int>());

        app.createSlowZone(mapID, tlPoint, brPoint);

        //send slowZoneCreated()...
    }
    catch (const std::exception& e){
        std::cerr << "Error parsing handleDeletePoi message: " << e.what() << "\n";
    }
}

void AppAdapter::handleCreateCommonZone(const nlohmann::json& content) {
    try {
        int mapID = content.at("mapID").get<int>();
        auto tl = content.at("tl");
        auto br = content.at("br");
        Point tlPoint = Point(tl.at("x").get<int>(), tl.at("y").get<int>());
        Point brPoint = Point(br.at("x").get<int>(), br.at("y").get<int>());

        app.createCommonZone(mapID, tlPoint, brPoint);

        //send CommonZoneCreated()...
    }
    catch (const std::exception& e){
        std::cerr << "Error parsing handleDeletePoi message: " << e.what() << "\n";
    }
}

void AppAdapter::handleCreateMap(const nlohmann::json& content) {
    //not available
}

void AppAdapter::handleDeleteMap(const nlohmann::json& content) {
    //not available
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

        if (type == "createTask") handleCreateTask(msg.at("content"));
        else if (type == "endCurrentTask") handleEndCurrentTask(msg.at("content"));
        else if (type == "deleteTask") handleDeleteTask(msg.at("content"));
        else if (type == "updateRobotPosition") handleUpdateRobotPosition(msg.at("content"));
        else if (type == "createRobot") handleCreateRobot(msg.at("content"));
        else if (type == "deleteRobot") handleDeleteRobot(msg.at("content"));
        else if (type == "createCommonPoi") handleCreateCommonPoi(msg.at("content"));
        else if (type == "deletePoi") handleDeletePoi(msg.at("content"));
        else if (type == "createForbidenZone") handleCreateForbidenZone(msg.at("content"));
        else if (type == "createSlowZone") handleCreateSlowZone(msg.at("content"));
        else if (type == "createCommonZone") handleCreateCommonZone(msg.at("content"));
        else std::cerr << "Unknown message type: " << type << "\n";
        
    } catch (const std::exception& e) {
        std::cerr << "Error handling message: " << e.what() << "\n";
    }
}
