import WebSocket, { WebSocketServer } from 'ws';

// Crear servidor WebSocket en el puerto 8080
const wss = new WebSocketServer({ port: 8080 });

console.log('WebSocket server running on ws://localhost:8080');

wss.on('connection', (ws) => {
  console.log('Client connected');

  // Enviar mensaje de inicialización al conectarse
  setTimeout(() => {
    const iniMessage = {
      type: 'ini',
      content: {
        grid: [
          ['254', '254', '254', '254', '254'],
          ['254', '252', '254', '0', '254'],
          ['254', '253', '254', '254', '254'],
          ['254', '251', '254', '254', '254'],
          ['254', '254', '255', '254', '254']
        ],
        zones: {
          '254': 'EMPTY',
          '255': 'OUT_OF_BOUNDS',
          '253': 'WALL',
          '0': 'POI',
          '252': 'LowSpeedZone',
          '251': 'ForbiddenZone'
        },
        tasks: {
          'robot1': ['tarea2', 'tarea3', 'tarea4'],
          'robot2': ['tarea5', 'tarea6', 'tarea7'],
          'pendientes': ['tarea8', 'tarea9']
        }
      }
    };

    ws.send(JSON.stringify(iniMessage));
    console.log('Sent ini message');
  }, 1000);

  // Manejar mensajes del cliente
  ws.on('message', (message) => {
    console.log('Received:', message.toString());
  });

  // Limpiar cuando el cliente se desconecta
  ws.on('close', () => {
    console.log('Client disconnected');
    
  });

  ws.on('error', (error) => {
    console.error('WebSocket error:', error);
  });
});

console.log('\nReady to accept connections!');

