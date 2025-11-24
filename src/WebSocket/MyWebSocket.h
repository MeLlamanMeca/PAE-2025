#pragma once

#include <ixwebsocket/IXWebSocketServer.h>
#include <nlohmann/json.hpp>
#include <memory>
#include "../app/AppAdapter.h"
#include <functional>

using json = nlohmann::json;

class MyWebSocket {
    public:
        MyWebSocket(int port_, AppAdapter& adapter_);

        void run();

    private:
        int port;
        AppAdapter adapter;
        ix::WebSocketServer server;

        void setupComms();
};