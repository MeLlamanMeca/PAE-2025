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
          <div class="order-3 lg:order-2 lg:col-span-6">
            <Map
              :grid="grid"
              :zones="zones"
              :robotPaths="robotPaths"
              :robotPositions="robotPositions"
              :robotInitialPositions="robotInitialPositions"
              :colorMap="colorMap"
            />
          </div>
          
          <!-- Zones on the right (hidden on mobile) -->
          <div class="order-2 lg:order-3 lg:col-span-3 hidden lg:block">
            <Zones :zones="zones" :colorMap="colorMap" />
          </div>
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
          <Panel :allTasksList="allTasksList" />
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
import Zones from './components/principal/zones.vue'
import Panel from './components/panelControl/panel.vue'

const connected = ref(false)
const grid = ref<string[][]>([])
const zones = ref<Record<string, string> | null>(null)
const tasks = ref<Record<string, string[]> | null>(null)
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
}

const allTasksList = ref<Task[]>([])
const robotsList = ref<string[]>([])

let ws: WebSocket | null = null
const animationIntervals: Record<string, number> = {}
const cleanupTimeouts: Record<string, number> = {}

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
  }
  
  ws.onmessage = (event) => {
    try {
      const data = JSON.parse(event.data)
      
      // Switch para manejar diferentes tipos de mensajes
      switch (data.type) {
        case 'ini':
          initialize(data.content)
          break
        
        case 'updateTask':
         updateTask(data.content)
          break
        
        case 'task':
         handleTaskPath(data.content)
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
  grid.value = content.grid
  zones.value = content.zones
  tasks.value = content.tasks
  console.log('Grid, zones, and tasks set:', grid.value, zones.value, tasks.value);

  // Inicializar la lista de tareas desde el servidor
  if (content.allTasks) {
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
  }

  // Guardar posiciones iniciales de los robots si vienen en el mensaje
  if (content.robots) {
    // content.robots debe ser un objeto tipo { [robotId]: [fila, columna] }
    robotInitialPositions.value = { ...content.robots }
    // Inicializar robotPositions con las posiciones iniciales si no hay ruta activa
    for (const [robotId, pos] of Object.entries(content.robots)) {
      if (!robotPositions.value[robotId]) {
        robotPositions.value[robotId] = pos as [number, number]
      }
    }
  }

  // No llamar a drawGrid, el canvas lo gestiona Map.vue
}

// Actualizar tareas
function updateTask(content: any) {
  console.log('Updating tasks:', content)
  
  if (content.tasks) {
    tasks.value = content.tasks
  }
  
  // Si el mensaje incluye cambios en el grid, actualizar
  if (content.grid) {
    grid.value = content.grid
    // No llamar a drawGrid
  }
}

// Manejar rutas de robots
function handleTaskPath(content: any) {
  console.log('Received task path:', content)
  console.log('Path coordinates:', content.path)
  
  if (content.robotId && content.path) {
    const robotId = content.robotId
    const path = content.path
    const taskName = content.taskName || 'Tarea sin nombre'
    
    console.log(`${robotId} will follow path with ${path.length} points`)
    
    // Detener animación previa si existe
    if (animationIntervals[robotId]) {
      clearInterval(animationIntervals[robotId])
      delete animationIntervals[robotId]
    }
    
    // Cancelar cualquier limpieza pendiente
    if (cleanupTimeouts[robotId]) {
      clearTimeout(cleanupTimeouts[robotId])
      delete cleanupTimeouts[robotId]
    }
    
    // Guardar la ruta completa
    robotPaths.value[robotId] = path
    
    // Guardar el nombre de la tarea activa
    activeTasks.value[robotId] = taskName
    
    // Actualizar el estado de la tarea en la lista
    updateTaskStatus(taskName, 'active')
    
    // Iniciar la animación del robot
    startRobotAnimation(robotId, path, taskName)
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

// Animar el movimiento del robot a través de la ruta
function startRobotAnimation(robotId: string, path: [number, number][], taskName: string) {
  if (path.length === 0) return
  let currentStep = 0
  robotAnimating.value[robotId] = true
  // Establecer posición inicial
  robotPositions.value[robotId] = path[0]
  // No llamar a drawGrid
  // Animar cada paso (1 segundo por cuadrado)
  animationIntervals[robotId] = window.setInterval(() => {
    currentStep++
    if (currentStep < path.length) {
      // Mover el robot a la siguiente posición
  robotPositions.value[robotId] = path[currentStep]
  // No llamar a drawGrid
    } else {
      // Terminar la animación
      clearInterval(animationIntervals[robotId])
      delete animationIntervals[robotId]
      robotAnimating.value[robotId] = false
      // Marcar la tarea como completada
      updateTaskStatus(taskName, 'completed')
      // Notificar al servidor que terminó
      if (ws && ws.readyState === WebSocket.OPEN) {
        const message = {
          type: 'terminado',
          robotId: robotId
        }
        ws.send(JSON.stringify(message))
        console.log(`Robot ${robotId} finished path, sent terminado message`)
      }
      // Limpiar la ruta visual después de un momento
      cleanupTimeouts[robotId] = window.setTimeout(() => {
        // Solo limpiar la ruta si no hay una nueva animación en curso
        if (!animationIntervals[robotId]) {
          // Limpiar solo el path, no la posición del robot
          delete robotPaths.value[robotId]
          // Mantener al robot en su última posición
          // Si hay posición inicial definida, usarla, si no, mantener la última
          if (robotInitialPositions.value[robotId]) {
            robotPositions.value[robotId] = robotInitialPositions.value[robotId]
          }
          // NO eliminar robotPositions para que el robot siga visible
        }
        delete cleanupTimeouts[robotId]
      }, 2000)
    }
  }, 1000) // 1 segundo por cuadrado
}

onMounted(() => {
  connectWebSocket()
})

onUnmounted(() => {
  // Limpiar todos los intervalos de animación
  Object.values(animationIntervals).forEach(interval => {
    clearInterval(interval)
  })
  
  // Limpiar todos los timeouts de limpieza
  Object.values(cleanupTimeouts).forEach(timeout => {
    clearTimeout(timeout)
  })
  
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