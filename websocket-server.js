import WebSocket, { WebSocketServer } from 'ws';

// Crear servidor WebSocket en el puerto 8080
const wss = new WebSocketServer({ port: 8080 });

console.log('WebSocket server running on ws://localhost:8080');

// Rutas predefinidas para los robots (simulación)
// Robot1 opera en la parte superior del grid (filas 1-4)
// Robot2 opera en la parte inferior del grid (filas 6-9)
const robotRoutes = {
  robot1: [
    {
      name: 'Cargando Caja 1',
      path: [
        [1, 1], [1, 2], [1, 3], [1, 4], [1, 5], [1, 6], [1, 7]
      ]
    },
    {
      name: 'Descargando Caja 1',
      path: [
        [1, 8], [2, 8], [3, 8], [3, 7], [3, 6], [3, 5], [3, 4], [3, 3], [3, 2], [3, 1]
      ]
    },
    {
      name: 'Cargando Caja 2',
      path: [
        [3, 1], [2, 1], [2, 2], [2, 3], [2, 4], [2, 5], [2, 6], [2, 7], [2, 8], [1, 8], [1, 7]
      ]
    },
  ],
  robot2: [
    {
      name: 'Descargando Caja 2',
      path: [
        [8, 1], [8, 2], [8, 3], [8, 4], [8, 5], [8, 6], [8, 7], [8, 7]
      ]
    },
    {
      name: 'Cargando Caja 3',
      path: [
        [8, 8], [7, 8], [6, 8], [6, 7], [6, 6], [6, 5], [6, 4], [6, 3], [6, 2], [6, 1]
      ]
    },
    {
      name: 'Descargando Caja 3',
      path: [
        [6, 1], [7, 1], [7, 2], [7, 3], [7, 4], [7, 5], [7, 6], [7, 7], [7, 8], [8, 8], [8, 7]
      ]
    },
  ]
};

const robotTaskIndex = {
  robot1: 0,
  robot2: 0
};

wss.on('connection', (ws) => {
  console.log('Client connected');

  // Enviar mensaje de inicialización al conectarse
  setTimeout(() => {
    const iniMessage = {
      type: 'ini',
      content: {
        grid: [
          ['255', '255', '255', '255', '255', '255', '255', '255', '255', '255'],
          ['255', '252', '254', '254', '254', '254', '254', '0', '254', '255'],
          ['255', '254', '254', '254', '254', '254', '254', '254', '254', '255'],
          ['255', '251', '254', '254', '254', '254', '254', '254', '254', '255'],
          ['255', '254', '253', '253', '253', '254', '253', '253', '253', '255'],
          ['255', '254', '254', '254', '254', '254', '254', '254', '254', '255'],
          ['255', '254', '254', '254', '254', '254', '254', '252', '254', '255'],
          ['255', '1', '254', '254', '254', '254', '254', '254', '254', '255'],
          ['255', '254', '254', '254', '254', '254', '254', '2', '254', '255'],
          ['255', '255', '255', '255', '255', '255', '255', '255', '255', '255']
        ],
        zones: {
          '254': 'EMPTY',
          '255': 'OUT_OF_BOUNDS',
          '253': 'WALL',
          '0': 'POI',
          '1': 'POI',
          '2': 'POI',
          '252': 'LowSpeedZone',
          '251': 'ForbiddenZone'
        },
        allTasks: [
          { id: 'task1', robotId: 'robot1', name: 'Cargando Caja 1', status: 'pending' },
          { id: 'task2', robotId: 'robot1', name: 'Descargando Caja 1', status: 'pending' },
          { id: 'task3', robotId: 'robot1', name: 'Cargando Caja 2', status: 'pending' },
          { id: 'task4', robotId: 'robot2', name: 'Descargando Caja 2', status: 'pending' },
          { id: 'task5', robotId: 'robot2', name: 'Cargando Caja 3', status: 'pending' },
          { id: 'task6', robotId: 'robot2', name: 'Descargando Caja 3', status: 'pending' }
        ]
      }
    };

    ws.send(JSON.stringify(iniMessage));
    console.log('Sent ini message');

    // Enviar primera ruta para robot1 después del ini
    setTimeout(() => {
      sendNextTask(ws, 'robot1');
    }, 1000);

    // Enviar primera ruta para robot2 después del ini
    setTimeout(() => {
      sendNextTask(ws, 'robot2');
    }, 1500);
  }, 1000);

  // Manejar mensajes del cliente
  ws.on('message', (message) => {
    console.log('Received:', message.toString());
    
    try {
      const data = JSON.parse(message.toString());
      
      // Si el robot terminó su ruta, enviar la siguiente
      if (data.type === 'terminado') {
        console.log(`Robot ${data.robotId} finished task`);
        
        // Esperar un poco antes de enviar la siguiente tarea
        setTimeout(() => {
          sendNextTask(ws, data.robotId);
        }, 1000);
      }
    } catch (error) {
      console.error('Error parsing message:', error);
    }
  });

  // Limpiar cuando el cliente se desconecta
  ws.on('close', () => {
    console.log('Client disconnected');
    
  });

  ws.on('error', (error) => {
    console.error('WebSocket error:', error);
  });
});

// Función para enviar la siguiente tarea a un robot
function sendNextTask(ws, robotId) {
  const routes = robotRoutes[robotId];
  
  if (!routes || routes.length === 0) {
    console.log(`No routes available for ${robotId}`);
    return;
  }

  const taskIndex = robotTaskIndex[robotId];
  
  // Verificar si ya completó todas sus tareas
  if (taskIndex >= routes.length) {
    console.log(`${robotId} has completed all tasks!`);
    return;
  }
  
  const taskData = routes[taskIndex];
  
  const taskMessage = {
    type: 'task',
    content: {
      robotId: robotId,
      taskName: taskData.name,
      path: taskData.path
    }
  };

  ws.send(JSON.stringify(taskMessage));
  console.log(`Sent task "${taskData.name}" to ${robotId}`);
  
  // Incrementar el índice para la próxima vez
  robotTaskIndex[robotId]++;
}

console.log('\nReady to accept connections!');

