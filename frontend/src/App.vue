<template>
  <div id="app" class="min-h-screen bg-gradient-to-br from-slate-50 via-blue-50 to-indigo-50 font-sans">
    <!-- Header -->
    <header class="bg-white shadow-sm border-b border-gray-200">
      <div class="max-w-7xl mx-auto px-4 sm:px-6 lg:px-8 py-4 sm:py-6">
        <div class="flex flex-col sm:flex-row sm:items-center sm:justify-between gap-4">
          <div class="text-center sm:text-left">
            <h1 class="text-2xl sm:text-3xl lg:text-4xl font-bold bg-gradient-to-r from-blue-600 to-indigo-600 bg-clip-text text-transparent">
              SARA
            </h1>
            <p class="text-sm sm:text-base text-gray-600 mt-1">Robot Management System</p>
          </div>
          
          <div class="flex justify-center sm:justify-end items-center gap-3">
            <span :class="[
              'inline-flex items-center gap-2 px-4 py-2 rounded-full font-semibold text-xs sm:text-sm shadow-sm border-2 transition-all duration-300',
              connected 
                ? 'bg-green-50 text-green-700 border-green-200 shadow-green-100' 
                : 'bg-red-50 text-red-700 border-red-200 shadow-red-100 animate-pulse'
            ]">
              <span :class="[
                'w-2 h-2 rounded-full',
                connected ? 'bg-green-500' : 'bg-red-500'
              ]"></span>
              {{ connected ? 'Conectado' : 'Desconectado' }}
            </span>
            
            <button
              @click="showPanel = true"
              class="inline-flex items-center gap-2 px-4 py-2 bg-gradient-to-r from-blue-600 to-indigo-600 hover:from-blue-700 hover:to-indigo-700 text-white font-semibold rounded-lg shadow-lg hover:shadow-xl transition-all duration-300 text-xs sm:text-sm transform hover:scale-105"
            >
              <svg class="w-4 h-4" fill="none" stroke="currentColor" viewBox="0 0 24 24">
                <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M9 5H7a2 2 0 00-2 2v12a2 2 0 002 2h10a2 2 0 002-2V7a2 2 0 00-2-2h-2M9 5a2 2 0 002 2h2a2 2 0 002-2M9 5a2 2 0 012-2h2a2 2 0 012 2"></path>
              </svg>
              Panel de Control
            </button>
          </div>
        </div>
      </div>
    </header>

    <!-- Main Content -->
    <main class="max-w-full mx-auto px-4 sm:px-6 lg:px-8 py-6 sm:py-8 overflow-x-hidden">
      <div class="flex flex-col gap-6 lg:gap-8">
        <!-- Three column layout: Tasks, Map, Zones -->
        <div class="grid grid-cols-1 lg:grid-cols-12 gap-4 lg:gap-6 items-start">
          <!-- Tasks on the left -->
          <div class="order-1 lg:col-span-3">
            <Tasks :allTasksList="allTasksList" />
          </div>
          
          <!-- Map in the center -->
          <div class="order-3 lg:order-2 lg:col-span-9">
            <Map
              :grid="grid"
              :pois="pois"
              :robotPaths="robotPaths"
              :robotPositions="robotPositions"
              :robotInitialPositions="robotInitialPositions"
              :colorMap="colorMap"
            />
          </div>
          
          <!-- Zones removed as per request -->
        </div>

        <!-- Robots section below -->
        <Robots :allTasksList="allTasksList" :robotsList="robotsList" />
      </div>
    </main>
  
    <!-- Modal del Panel de Control -->
    <div 
      v-if="showPanel"
      class="fixed inset-0 bg-black/60 backdrop-blur-sm flex items-center justify-center z-50 p-4 animate-fadeIn"
      @click.self="showPanel = false"
    >
      <div class="bg-white rounded-2xl shadow-2xl max-w-4xl w-full max-h-[90vh] overflow-hidden relative transform transition-all animate-slideUp">
        <button
          @click="showPanel = false"
          class="absolute top-4 right-4 text-gray-400 hover:text-gray-600 hover:bg-gray-100 rounded-full p-2 transition-all duration-200 z-10"
        >
          <svg class="w-6 h-6" fill="none" stroke="currentColor" viewBox="0 0 24 24">
            <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M6 18L18 6M6 6l12 12"></path>
          </svg>
        </button>
        
        <div class="overflow-y-auto max-h-[90vh]">
          <Panel 
            :allTasksList="allTasksList" 
            :pois="pois"
            @create-task="sendCreateTask"
            @delete-task="sendDeleteTask"
            @create-poi="handleCreatePoi"
            @delete-poi="sendDeletePoi"
          />
        </div>
      </div>
    </div>
  </div>
</template>

<script setup lang="ts">
import { onMounted, onUnmounted, ref } from 'vue'
import Tasks from './components/principal/tasks.vue'
import Robots from './components/principal/robots.vue'
import Map from './components/principal/map.vue'
import Panel from './components/panelControl/panel.vue'

const connected = ref(false)
const grid = ref<string[][]>([])
const tasks = ref<Record<string, string[]> | null>(null)
const pois = ref<any[]>([])
const robotPaths = ref<Record<string, [number, number][]>>({});
const robotPositions = ref<Record<string, [number, number]>>({});
const robotInitialPositions = ref<Record<string, [number, number]>>({});
const robotAnimating = ref<Record<string, boolean>>({});
const activeTasks = ref<Record<string, string>>({});
const showPanel = ref(false);

// Lista de todas las tareas definidas
interface Task {
  id: string
  robotId: string
  name: string
  status: 'completed' | 'active' | 'pending'
  statusLabel: string
  pointIni?: { x: number, y: number }
  pointFin?: { x: number, y: number }
  weight?: number
}

const allTasksList = ref<Task[]>([])
const robotsList = ref<string[]>([])

let ws: WebSocket | null = null

// Mapa de colores para cada número/zona
const colorMap: Record<string, string> = {
 // Caracteres especiales según la imagen
  '254': '#FFFFFF',   // EMPTY - Blanco
  '255': '#808080',   // OUT_OF_BOUNDS - Gris (fuera de límites)
  '253': '#2C2C2C',   // WALL - Negro/Gris oscuro
  '252': '#FFD700',   // LowSpeedZone - Amarillo dorado
  '251': '#DC143C',   // ForbiddenZone - Rojo carmesí
  
  // POI (Points of Interest) - Números del 0 al 250
  '0': '#00CED1',     // POI 0 - Turquesa
  '1': '#FF6347',     // POI 1 - Tomate
  '2': '#4169E1',     // POI 2 - Azul real
  '3': '#FF8C00',     // POI 3 - Naranja oscuro
  '4': '#9370DB',     // POI 4 - Púrpura medio
  '5': '#32CD32',     // POI 5 - Verde lima
  '6': '#FF1493',     // POI 6 - Rosa profundo
  '7': '#00BFFF',     // POI 7 - Azul cielo profundo
  '8': '#FFD700',     // POI 8 - Oro
  '9': '#BA55D3',     // POI 9 - Orquídea medio
  '10': '#20B2AA',    // POI 10 - Verde mar claro
}

// getColor ya no se usa aquí, se usa en Map.vue

// Filtrar tareas por robot (ya no se usa aquí, está en Robots.vue)
function getRobotTasks(robotId: string): Task[] {
  return allTasksList.value.filter(task => task.robotId === robotId)
}

function connectWebSocket() {
  // Cambia esta URL por la de tu servidor WebSocket
  ws = new WebSocket('ws://localhost:8080')
  
  ws.onopen = () => {
    connected.value = true
    console.log('WebSocket connected')
    
    // Solicitar inicialización
    const iniRequest = {
      type: 'ini',
      content: {
        mapID: 1
      }
    }
    ws.send(JSON.stringify(iniRequest))
    console.log('Sent ini request:', iniRequest)
  }
  
  ws.onmessage = (event) => {
    try {
      const data = JSON.parse(event.data)
      console.log('Received message type:', data.type, data.content)
      
      // Switch para manejar diferentes tipos de mensajes
      switch (data.type) {
        case 'ini':
          initialize(data.content)
          break
        
        case 'assignedTask':
          handleAssignedTask(data.content)
          break
        
        case 'deletedTask':
          handleDeletedTask(data.content)
          break

        case 'endCurrentTask':
          handleEndCurrentTask(data.content)
          break

        case 'setActiveTask':
          handleSetActiveTask(data.content)
          break
        
        case 'moveRobot':
          handleMoveRobot(data.content)
          break

        case 'createdPoi':
          handleCreatedPoi(data.content)
          break

        case 'deletedPoi':
          handleDeletedPoi(data.content)
          break
        
        case 'createdZone':
          console.log('Zone created:', data.content)
          break
        
        default:
          console.warn('Unknown message type:', data.type)
      }
    } catch (error) {
      console.error('Error parsing WebSocket message:', error)
    }
  }
  
  ws.onerror = (error) => {
    console.error('WebSocket error:', error)
  }
  
  ws.onclose = () => {
    connected.value = false
    console.log('WebSocket disconnected')
    
    // Intentar reconectar después de 3 segundos
    setTimeout(connectWebSocket, 3000)
  }
}

// Inicializar el grid, zones y tasks
function initialize(content: any) {
  console.log('Initializing with content:', content)
  
  // Handle Map (support both 'map' and 'grid' properties)
  if (content.map) {
    grid.value = content.map.map((row: any[]) => row.map(cell => String(cell)))
  } else if (content.grid) {
    grid.value = content.grid
  }

  // Handle POIs
  if (content.pois) {
    pois.value = content.pois
  }
  
  if (content.tasks) tasks.value = content.tasks
  
  console.log('Grid and tasks set');

  // Handle Robots and Tasks from new format
  if (content.robots) {
    const newRobotsList: string[] = []
    const newRobotPositions: Record<string, [number, number]> = {}
    const newAllTasks: Task[] = []

    content.robots.forEach((robot: any) => {
      const robotId = String(robot.ID)
      newRobotsList.push(robotId)
      

      if (robot.position) {
        newRobotPositions[robotId] = [robot.position.x, robot.position.y]
        robotInitialPositions.value[robotId] = [robot.position.x, robot.position.y]
      }

      // Tasks
      if (robot.tasks) {
        robot.tasks.forEach((t: any) => {
          newAllTasks.push({
            id: String(t.ID),
            robotId: robotId,
            name: `Tarea ${t.ID}`,
            status: 'pending',
            statusLabel: 'Pendiente',
            pointIni: t.ini,
            pointFin: t.fin,
            weight: t.weight
          })
        })
      }

      // State
      if (robot.state) {
        // Check if state is object and has task
        if (typeof robot.state === 'object' && robot.state.task) {
           const activeTaskId = String(robot.state.task.ID)
           const task = newAllTasks.find(t => t.id === activeTaskId && t.robotId === robotId)
           if (task) {
             task.status = 'active'
             task.statusLabel = 'En Progreso'
           }
           
           // If there is a route in state, show it
           if (robot.state.route) {
             robotPaths.value[robotId] = robot.state.route.map((p: any) => [p.y, p.x])
           }
        }
      }
    })
    
    robotsList.value = newRobotsList.sort()
    robotPositions.value = newRobotPositions
    allTasksList.value = newAllTasks
  } 
  // Fallback for old format
  else if (content.allTasks) {
    allTasksList.value = content.allTasks.map((task: any) => ({
      ...task,
      statusLabel: task.status === 'completed' ? 'Completada' : 
                   task.status === 'active' ? 'En Progreso' : 
                   'Pendiente'
    }))
    console.log('Initialized tasks list:', allTasksList.value)

    // Extraer la lista única de robots
    const uniqueRobots = new Set<string>()
    content.allTasks.forEach((task: any) => {
      uniqueRobots.add(task.robotId)
    })
    robotsList.value = Array.from(uniqueRobots).sort()
    console.log('Robots list:', robotsList.value)
    
    // Guardar posiciones iniciales de los robots si vienen en el mensaje (old format)
    if (content.robots) {
        robotInitialPositions.value = { ...content.robots }
        for (const [robotId, pos] of Object.entries(content.robots)) {
        if (!robotPositions.value[robotId]) {
            robotPositions.value[robotId] = pos as [number, number]
        }
        }
    }
  }
}

function handleAssignedTask(content: any) {
  console.log('Handling assignedTask:', content)
  // content: { robotID, position, task: { ini, fin, ID } }
  
  const robotId = String(content.robotID)
  const taskData = content.task
  const taskId = String(taskData.ID)
  const position = content.position // Posición en la lista donde debe ir la tarea
  
  const newTask: Task = {
    id: taskId,
    robotId: robotId,
    name: `${taskData.ini.x},${taskData.ini.y} -> ${taskData.fin.x},${taskData.fin.y}`,
    status: 'pending',
    statusLabel: 'Pendiente',
    pointIni: taskData.ini,
    pointFin: taskData.fin
  }
  
  // Si la posición está dentro del rango del array, reemplazar usando splice para reactividad
  // Si no, agregar al final
  if (position >= 0 && position < allTasksList.value.length) {
    allTasksList.value.splice(position, 1, newTask)
    console.log('Replaced task at position:', position)
  } else {
    allTasksList.value.push(newTask)
    console.log('Added new task at the end')
  }
  
  // Asegurar que el robot esté en la lista
  if (!robotsList.value.includes(robotId)) {
    robotsList.value.push(robotId)
    robotsList.value.sort()
  }
}

function handleDeletedTask(content: any) {
  console.log('Handling deletedTask:', content)
  // content: { robotID, taskID }
  const rId = String(content.robotID)
  const tId = String(content.taskID)
  
  const index = allTasksList.value.findIndex(t => t.id === tId && t.robotId === rId)
  if (index !== -1) {
    allTasksList.value.splice(index, 1)
  }
}

function handleEndCurrentTask(content: any) {
  console.log('Handling endCurrentTask:', content)
  // content: { robotID }
  const robotId = String(content.robotID)
  
  // Buscar la tarea activa de este robot y marcarla como completada o eliminarla
  // Si queremos mantener historial, la marcamos completada. Si no, la borramos.
  // Asumiremos que se completa.
  const activeTaskName = activeTasks.value[robotId]
  if (activeTaskName) {
    updateTaskStatus(activeTaskName, 'completed')
    delete activeTasks.value[robotId]
    delete robotPaths.value[robotId]
  }
}

function handleSetActiveTask(content: any) {
  console.log('Handling setActiveTask:', content)
  // content: { robotID, task: { ini, fin, ID }, route: [...] }
  const robotId = String(content.robotID)
  const taskData = content.task
  const route = content.route
  
  // Actualizar ruta
  if (route) {
    robotPaths.value[robotId] = route.map((p: any) => [p.y, p.x])
  }
  
  // Actualizar estado de la tarea
  const taskId = String(taskData.ID)
  const taskName = `Tarea ${taskId}`
  
  // Si la tarea no existe en la lista, añadirla (por si acaso)
  let task = allTasksList.value.find(t => t.id === taskId && t.robotId === robotId)
  if (!task) {
    task = {
      id: taskId,
      robotId: robotId,
      name: taskName,
      status: 'active',
      statusLabel: 'En Progreso',
      pointIni: taskData.ini,
      pointFin: taskData.fin
    }
    allTasksList.value.push(task)
  } else {
    task.status = 'active'
    task.statusLabel = 'En Progreso'
  }
  
  activeTasks.value[robotId] = taskName
}

function handleMoveRobot(content: any) {
  console.log('Handling moveRobot:', content)
  // content: { robotID, position: {x, y} }
  // x es fila, y es columna
  const robotId = String(content.robotID)
  if (content.position) {
    robotPositions.value[robotId] = [content.position.x, content.position.y]
  }
}

function handleCreatedPoi(content: any) {
  console.log('Handling createdPoi:', content)
  // content: { type, name, position: {x, y} }
  
  // Generar un ID temporal si no viene (el mensaje de ejemplo no trae ID explícito fuera de 'ini')
  // Pero para el frontend necesitamos un ID único. Usaremos coordenadas o timestamp.
  const tempId = `poi-${Date.now()}`
  
  pois.value.push({
    id: tempId,
    name: content.name || content.type,
    position: content.position,
    type: content.type
  })
}

function handleDeletedPoi(content: any) {
  console.log('Handling deletedPoi:', content)
  // content: { position: {x, y} }
  
  if (content.position) {
    const { x, y } = content.position
    const index = pois.value.findIndex(p => p.position.x === x && p.position.y === y)
    if (index !== -1) {
      pois.value.splice(index, 1)
    }
  }
}

// Actualizar el estado de una tarea en la lista
function updateTaskStatus(taskName: string, status: 'completed' | 'active' | 'pending') {
  const task = allTasksList.value.find(t => t.name === taskName)
  if (task) {
    task.status = status
    task.statusLabel = status === 'completed' ? 'Completada' : 
                       status === 'active' ? 'En Progreso' : 
                       'Pendiente'
  }
}

function sendCreateTask(taskData: any) {
  if (ws && ws.readyState === WebSocket.OPEN) {
    const message = {
      type: 'createTask',
      content: {
        mapID: 1, // Asumimos mapID 1 por defecto o lo sacamos de algún lado
        pointIni: taskData.pointIni,
        pointFin: taskData.pointFin,
        weight: taskData.weight
      }
    }
    ws.send(JSON.stringify(message))
    console.log('Sent createTask:', message)
  } else {
    console.error('WebSocket not connected')
  }
}

function sendDeleteTask(taskData: any) {
  if (ws && ws.readyState === WebSocket.OPEN) {
    const message = {
      type: 'deleteTask',
      content: {
        robotID: taskData.robotId,
        taskID: taskData.taskId
      }
    }
    ws.send(JSON.stringify(message))
    console.log('Sent deleteTask:', message)
  } else {
    console.error('WebSocket not connected')
  }
}

function sendCreateCommonPoi(poiData: any) {
  if (ws && ws.readyState === WebSocket.OPEN) {
    const message = {
      type: 'createCommonPoi',
      content: {
        mapID: 1,
        name: poiData.name,
        position: poiData.position, // {x, y}
        color: poiData.color
      }
    }
    ws.send(JSON.stringify(message))
    console.log('Sent createCommonPoi:', message)
  } else {
    console.error('WebSocket not connected')
  }
}

function sendCreateChargingBayPoi(poiData: any) {
  if (ws && ws.readyState === WebSocket.OPEN) {
    const message = {
      type: 'createChargingBayPoi',
      content: {
        mapID: 1,
        name: poiData.name,
        position: poiData.position, // {x, y}
        color: poiData.color
      }
    }
    ws.send(JSON.stringify(message))
    console.log('Sent createChargingBayPoi:', message)
  } else {
    console.error('WebSocket not connected')
  }
}

function handleCreatePoi(poiData: any) {
  if (poiData.type === 'chargingBay') {
    sendCreateChargingBayPoi(poiData)
  } else {
    sendCreateCommonPoi(poiData)
  }
}

function sendDeletePoi(poiData: any) {
  if (ws && ws.readyState === WebSocket.OPEN) {
    const message = {
      type: 'deletePoi',
      content: {
        mapID: 1,
        position: poiData.position // {x, y}
      }
    }
    ws.send(JSON.stringify(message))
    console.log('Sent deletePoi:', message)
  } else {
    console.error('WebSocket not connected')
  }
}

onMounted(() => {
  connectWebSocket()
})

onUnmounted(() => {
  if (ws) {
    ws.close()
  }
})
</script>

<style scoped>
@keyframes fadeIn {
  from { opacity: 0; }
  to { opacity: 1; }
}

@keyframes slideUp {
  from {
    opacity: 0;
    transform: translateY(20px);
  }
  to {
    opacity: 1;
    transform: translateY(0);
  }
}

.animate-fadeIn {
  animation: fadeIn 0.2s ease-out;
}

.animate-slideUp {
  animation: slideUp 0.3s ease-out;
}
</style>