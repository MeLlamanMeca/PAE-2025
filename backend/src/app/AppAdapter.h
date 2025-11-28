#pragma once

#include <functional>
#include <nlohmann/json.hpp>
#include "App.h"

class AppAdapter {
    private:
        App app;
        std::function<void(const nlohmann::json&)> sendMessageCallback;

        void broadcastMessage(const nlohmann::json& msg);

        void handleCreateTask(const nlohmann::json& msg);
        void handleDeleteTask(const nlohmann::json& msg);
        void handleEndCurrentTask(const nlohmann::json& msg);
        void handleStartTask(const nlohmann::json& msg);
        void handleUpdateRobotPosition(const nlohmann::json& msg);
        void handleCreateRobot(const nlohmann::json& msg);
        void handleDeleteRobot(const nlohmann::json& msg);
        void handleCreateCommonPoi(const nlohmann::json& msg);
        void handleDeletePoi(const nlohmann::json& msg);
        void handleCreateForbidenZone(const nlohmann::json& msg);
        void handleCreateSlowZone(const nlohmann::json& msg);
        void handleCreateCommonZone(const nlohmann::json& msg);
        void handleCreateMap(const nlohmann::json& msg);
        void handleDeleteMap(const nlohmann::json& msg);
        
        
    public:
        explicit AppAdapter(App& app) : app(app) {};

        void setSendMessageCallback(std::function<void(const nlohmann::json&)> cb);
        void handleMessage(const nlohmann::json& msg);
};
