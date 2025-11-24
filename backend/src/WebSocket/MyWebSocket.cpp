#include "MyWebSocket.h"
#include <iostream>

// ----------------------------------------------------------------------------------------
// CONSTRUCTOR ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------

MyWebSocket::MyWebSocket(int port_, AppAdapter& adapter_)
    : port(port_), adapter(adapter_), server(port_) { setupComms(); }

// ----------------------------------------------------------------------------------------
// PRIVATE FUNCTIONS ----------------------------------------------------------------------
// ----------------------------------------------------------------------------------------

void MyWebSocket::setupComms() {

    //setup callback
    adapter.setSendMessageCallback([this](const json& msg) {
        for (auto& client : server.getClients()) {
            auto ws = client.get();
            if (ws) ws->send(msg.dump());
        }
    });

    //setup entry handle messages
    server.setOnClientMessageCallback([this](std::shared_ptr<ix::ConnectionState> connectionState, const ix::WebSocket& webSocket, const ix::WebSocketMessagePtr& msg) {
        if (msg->type == ix::WebSocketMessageType::Message) {
            try {
                json received = json::parse(msg->str);
                adapter.handleMessage(received);
            } catch (const std::exception& e) {
                std::cerr << "Error parsing message: " << e.what() << std::endl;
            }
        } else if (msg->type == ix::WebSocketMessageType::Error) {
            std::cerr << "WebSocket error: " << msg->errorInfo.reason << std::endl;
        }
    });
}

// ----------------------------------------------------------------------------------------
// PUBLIC FUNCTIONS -----------------------------------------------------------------------
// ----------------------------------------------------------------------------------------

void MyWebSocket::run() {
    ix::initNetSystem();

    auto res = server.listen();
    if (!res.first) {
        std::cerr << "Error al iniciar el servidor: " << res.second << std::endl;
        return;
    }

    server.start();
    std::cout << "Servidor WebSocket escuchando en ws://localhost:" << port << std::endl;

    // Bloquea hasta Ctrl+C o server.stop()
    server.wait();
}
