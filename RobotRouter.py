import asyncio
import websockets
import json

# -----------------------------------------------------
# FUNCIÓN DE ENVÍO (Implementación de mensajes cada 1s)
# -----------------------------------------------------
async def sender(websocket):
    counter = 0
    while True:
        await asyncio.sleep(1) 
        
        counter += 1
        message_data = {
            "type": "heartbeat",
            "id": 1,
            "timestamp": asyncio.get_event_loop().time(),
            "sequence": counter
        }
        
        try:
            await websocket.send(json.dumps(message_data))
            # Opcional: print(f"⬆️ Mensaje enviado (Seq: {counter})")
        except websockets.ConnectionClosed:
            break

# -----------------------------------------------------
# FUNCIÓN DE ESCUCHA (Listener)
# -----------------------------------------------------
async def listener(websocket):
    while True:
        try:
            raw = await websocket.recv()
        except websockets.ConnectionClosed:
            print("❌ Conexión cerrada por el servidor.")
            break

        try:
            msg = json.loads(raw)
        except json.JSONDecodeError:
            print("⚠️ Mensaje no es JSON válido:", raw)
            continue

        print("📩 Mensaje recibido:", msg)

# -----------------------------------------------------
# FUNCIÓN PRINCIPAL (Main)
# -----------------------------------------------------
async def main():
    uri = "ws://localhost:8080"

    try:
        async with websockets.connect(uri) as websocket:
            print("🔌 Conectado al WebSocket.")
            await websocket.send(json.dumps({"type": "ini", "content": {"mapID": 1}}))

            # 1. Tarea para ESCUCHAR mensajes (recibirá el 'Hola cliente')
            listener_task = asyncio.create_task(listener(websocket))
            
            # 2. Tarea para ENVIAR mensajes periódicamente (heartbeats)
            sender_task = asyncio.create_task(sender(websocket))

            # Espera a que AMBAS tareas terminen (lo que sólo pasa si la conexión se cierra)
            await asyncio.gather(listener_task, sender_task)
            
    except ConnectionRefusedError:
        print("🛑 Error: No se pudo conectar al servidor. Asegúrate de que el servidor esté corriendo.")
    
    print("🤖 Conexión finalizada.") 


asyncio.run(main())