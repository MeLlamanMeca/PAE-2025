#include "./app/App.h"
#include "./app/AppAdapter.h"
#include "./services/impl/ServiceFactoryImpl.h"
#include "./controllers/impl/ControllerFactoryImpl.h"
#include "./WebSocket/MyWebSocket.h"

int main() {

    ControllerFactoryImpl cf;
    ServiceFactoryImpl sf(cf);
    App app(sf);

    /*
    //Crear almacen de prueba
    app.createMap(map)
    app.createPoi()...
    app.createPoI()...
    */
   
    AppAdapter adapter(app);

    // Crear el servidor WebSocket
    MyWebSocket wsServer(8080, adapter);

    // Arrancar la aplicación
    wsServer.run();

    return 0;
}
