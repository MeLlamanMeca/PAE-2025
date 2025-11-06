// Luego compilar con:
// g++ -std=c++17 server.cpp -I./external/json/include -I./external/uWebSockets/src -I./external/uWebSockets/uSockets/src -lpthread -o server

#include "external/uWebSockets/src/App.h"
#include "external/json/include/nlohmann/json.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <mutex>


AppController ac;




using json = nlohmann::json;

// Estructura básica para una tarea
struct Task {
    std::string description;
};

// Lista global de tareas y su mutex
std::vector<Task> tasks;
std::mutex tasksMutex;

int main() {
    using namespace std;

    cout << "🧠 Iniciando servidor de tareas..." << endl;

    uWS::App app;

    struct PerSocketData {
    };

    uWS::TemplatedApp<false>::WebSocketBehavior<PerSocketData> behavior;
    
    behavior.open = [](auto *ws) {
        cout << "🧍 Cliente conectado" << endl;

        // Enviar mensaje inicial
        json msg = {
            {"type", "info"},
            {"message", "Conectado al servidor C++"}
        };
        ws->send(msg.dump());
    };
    
    behavior.message = [](auto *ws, std::string_view data, uWS::OpCode opCode) {
            try {
                json received = json::parse(data);

                if (received["action"] == "add_task") {
                    std::string taskDescription = received["task"];

                    {
                        std::lock_guard<std::mutex> lock(tasksMutex);
                        tasks.push_back({taskDescription});
                    }

                    cout << "📝 Nueva tarea añadida: " << taskDescription << endl;

                    json response = {
                        {"type", "task_added"},
                        {"task", taskDescription},
                        {"total_tasks", tasks.size()}
                    };
                    ws->send(response.dump(), opCode);
                }
                else if (received["action"] == "list_tasks") {
                    std::lock_guard<std::mutex> lock(tasksMutex);
                    std::vector<std::string> list;
                    for (auto &t : tasks) list.push_back(t.description);

                    json response = {
                        {"type", "tasks_list"},
                        {"tasks", list}
                    };
                    ws->send(response.dump(), opCode);
                }
                else {
                    ws->send(R"({"type":"error","message":"Acción desconocida"})", opCode);
                }
            } catch (...) {
                ws->send(R"({"type":"error","message":"JSON inválido"})", opCode);
            }
    };
    
    behavior.close = [](auto *ws, int, std::string_view) {
        cout << "👋 Cliente desconectado" << endl;
    };

    app.ws<PerSocketData>("/*", behavior).listen(8080, [](auto *token) {
        if (token) std::cout << "✅ Servidor WebSocket en ws://localhost:8080" << std::endl;
    }).run();

    return 0;
}
