#include "./app/App.h"
#include "./app/AppAdapter.h"
#include "./services/impl/ServiceFactoryImpl.h"
#include "./controllers/impl/ControllerFactoryImpl.h"
#include "./WebSocket/MyWebSocket.h"

#define W 0xFD   // Wall
#define E 0xFE   // Empty
#define O 0xFF   // OutOfBounds

int main() {

    ControllerFactoryImpl cf;
    ServiceFactoryImpl sf(cf);
    App app(sf);

    std::vector<std::vector<unsigned char>> map = {
    { W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W },
    { W, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, W },
    { W, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, W },
    { W, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, W },
    { W, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, W },
    { W, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, W },
    { W, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, W },
    { W, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, W },
    { W, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, W },
    { W, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, W },
    { W, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, W },
    { W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W }
    };  
    app.createMap(1, map);
    app.createRobot(1, Point(2,2), 1500);
    app.createRobot(1, Point(5,5), 1500);
   
    AppAdapter adapter(app);

    // Crear el servidor WebSocket
    MyWebSocket wsServer(8080, adapter);

    // Arrancar la aplicación
    wsServer.run();

    return 0;
}
