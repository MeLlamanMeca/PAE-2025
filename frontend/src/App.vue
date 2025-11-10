<template>
  <div id="app" class="font-sans p-5">
    <h1 class="text-center text-gray-800 mb-5">SARA - Robot Management System</h1>
    
    <div class="text-center mb-5">
      <span :class="[
        'inline-block px-4 py-2 rounded-full font-bold text-sm',
        connected ? 'bg-green-100 text-green-800' : 'bg-red-100 text-red-800'
      ]">
        {{ connected ? '🟢 Conectado' : '🔴 Desconectado' }}
      </span>
    </div>

    <div class="flex gap-8 justify-center flex-wrap">
      <!-- Panel de tareas a la izquierda -->
      <div class="min-w-[280px] max-w-[320px] bg-gray-100 p-5 rounded-lg shadow-md">
        <h3 class="mt-0 text-gray-800 border-b-2 border-gray-300 pb-2.5 text-xl">Lista de Tareas</h3>
        <div class="flex flex-col gap-3">
          <div v-if="allTasksList.length === 0" class="text-center text-gray-400 py-5 italic">
            Cargando tareas...
          </div>
          <div 
            v-for="task in allTasksList" 
            :key="task.id"
            :class="[
              'p-4 rounded-lg border-2 transition-all duration-300',
              task.status === 'completed' ? 'bg-gray-500 border-gray-600 text-white' :
              task.status === 'active' ? 'bg-green-500 border-green-600 text-white shadow-lg scale-105 animate-pulse' :
              'bg-white border-gray-300 text-gray-800'
            ]"
          >
            <div class="flex justify-between items-center mb-2">
              <div class="font-bold text-sm">{{ task.robotId.toUpperCase() }}</div>
              <div class="text-xs font-semibold px-2 py-1 rounded-xl bg-white/30">{{ task.statusLabel }}</div>
            </div>
            <div class="text-base font-medium">{{ task.name }}</div>
          </div>
        </div>
      </div>

      <div class="flex-shrink-0">
        <canvas ref="canvas" width="800" height="800" class="border-4 border-gray-800 block shadow-lg" />
      </div>

      <div class="min-w-[300px] max-w-[400px]">
        <div v-if="zones" class="bg-gray-100 p-5 rounded-lg mb-5">
          <h3 class="mt-0 text-gray-800 border-b-2 border-gray-300 pb-2.5">Zones</h3>
          <div v-for="(name, id) in zones" :key="id" class="flex items-center gap-2.5 my-2.5 text-base">
            <span class="w-8 h-8 rounded border-2 border-gray-800 inline-block" :style="{ backgroundColor: getColor(id) }"></span>
            <span>{{ id }}: {{ name }}</span>
          </div>
        </div>
      </div>
    </div>

    <!-- Sección de robots debajo del mapa -->
    <div v-if="robotsList.length > 0" class="mt-5 p-4 bg-gray-100 rounded-lg flex flex-col items-center">
      <h3 class="text-gray-800 mb-4 text-lg font-semibold text-center">Tareas por Robot</h3>
      <div class="flex gap-4 justify-center">
        <div 
          v-for="robotId in robotsList" 
          :key="robotId"
          class="bg-white rounded-md p-8 shadow-sm border border-gray-200"
        >
          <h4 class="text-xs font-semibold text-gray-600 mb-2 pb-1.5 border-b border-gray-300 whitespace-nowrap">{{ robotId.toUpperCase() }}</h4>
          <div class=" flex flex-col gap-3">
            <div 
              v-for="task in getRobotTasks(robotId)" 
              :key="task.id"
              :class="[
                'px-2 py-1.5 rounded border transition-all duration-300 text-xs whitespace-nowrap',
                task.status === 'completed' ? 'bg-gray-500 border-gray-600 text-white' :
                task.status === 'active' ? 'bg-green-500 border-green-600 text-white font-medium' :
                'bg-white border-gray-300 text-gray-800'
              ]"
            >
              <div>{{ task.name }}</div>
            </div>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<script setup lang="ts">
import { onMounted, onUnmounted, ref } from 'vue'

const canvas = ref<HTMLCanvasElement | null>(null)
const connected = ref(false)
const grid = ref<string[][]>([])
const zones = ref<Record<string, string> | null>(null)
const tasks = ref<Record<string, string[]> | null>(null)
const robotPaths = ref<Record<string, [number, number][]>>({})
const robotPositions = ref<Record<string, [number, number]>>({})
const robotAnimating = ref<Record<string, boolean>>({})
const activeTasks = ref<Record<string, string>>({})

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

function getColor(id: string): string {
  return colorMap[id] || '#' + Math.floor(Math.random()*16777215).toString(16)
}

// Filtrar tareas por robot
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
  console.log('Grid, zones, and tasks set:', grid.value, zones.value, tasks.value)
  
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
  
  drawGrid()
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
    drawGrid()
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
  drawGrid()
  
  // Animar cada paso (1 segundo por cuadrado)
  animationIntervals[robotId] = window.setInterval(() => {
    currentStep++
    
    if (currentStep < path.length) {
      // Mover el robot a la siguiente posición
      robotPositions.value[robotId] = path[currentStep]
      drawGrid()
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
        // Solo limpiar si no hay una nueva animación en curso
        if (!animationIntervals[robotId]) {
          delete robotPaths.value[robotId]
          delete robotPositions.value[robotId]
          drawGrid()
        }
        delete cleanupTimeouts[robotId]
      }, 2000)
    }
  }, 1000) // 1 segundo por cuadrado
}

function drawGrid() {
  const c = canvas.value
  if (!c || !grid.value.length) return
  
  const ctx = c.getContext('2d')!
  const rows = grid.value.length
  const cols = grid.value[0].length
  
  const cellWidth = c.width / cols
  const cellHeight = c.height / rows
  
  // Limpiar el canvas
  ctx.clearRect(0, 0, c.width, c.height)
  
  // Dibujar cada celda
  for (let row = 0; row < rows; row++) {
    for (let col = 0; col < cols; col++) {
      const cellValue = grid.value[row][col]
      const color = getColor(cellValue)
      
      // Dibujar celda
      ctx.fillStyle = color
      ctx.fillRect(col * cellWidth, row * cellHeight, cellWidth, cellHeight)
      
      // Dibujar borde de la celda
      ctx.strokeStyle = '#333'
      ctx.lineWidth = 2
      ctx.strokeRect(col * cellWidth, row * cellHeight, cellWidth, cellHeight)
      
      // Dibujar el número si no es 255 (espacio vacío)
      if (cellValue !== '255') {
        ctx.fillStyle = '#000'
        ctx.font = `${cellWidth * 0.3}px Arial`
        ctx.textAlign = 'center'
        ctx.textBaseline = 'middle'
      
      }
    }
  }
  
  // Dibujar las rutas de los robots
  const robotColors: Record<string, string> = {
    'robot1': '#FF00FF',  // Magenta
    'robot2': '#00FF00',  // Verde brillante
    'robot3': '#0000FF',  // Azul
  }
  
  Object.entries(robotPaths.value).forEach(([robotId, path]) => {
    const color = robotColors[robotId] || '#FF0000'
    
    if (path.length === 0) return
    
    // Dibujar la línea de la ruta con transparencia para verla mejor
    ctx.globalAlpha = 0.6
    ctx.strokeStyle = color
    ctx.lineWidth = 6
    ctx.lineCap = 'round'
    ctx.lineJoin = 'round'
    
    ctx.beginPath()
    for (let i = 0; i < path.length; i++) {
      const [row, col] = path[i]
      const x = col * cellWidth + cellWidth / 2
      const y = row * cellHeight + cellHeight / 2
      
      if (i === 0) {
        ctx.moveTo(x, y)
      } else {
        ctx.lineTo(x, y)
      }
    }
    ctx.stroke()
    ctx.globalAlpha = 1.0
    
    // Dibujar círculos en cada punto de la ruta
    path.forEach(([row, col], index) => {
      const x = col * cellWidth + cellWidth / 2
      const y = row * cellHeight + cellHeight / 2
      
      // Círculo más grande para el inicio
      if (index === 0) {
        ctx.fillStyle = color
        ctx.beginPath()
        ctx.arc(x, y, cellWidth * 0.25, 0, 2 * Math.PI)
        ctx.fill()
        ctx.strokeStyle = '#FFFFFF'
        ctx.lineWidth = 3
        ctx.stroke()
      } 
      // Círculo grande para el final
      else if (index === path.length - 1) {
        ctx.fillStyle = color
        ctx.beginPath()
        ctx.arc(x, y, cellWidth * 0.3, 0, 2 * Math.PI)
        ctx.fill()
        ctx.strokeStyle = '#FFFFFF'
        ctx.lineWidth = 4
        ctx.stroke()
      }
      // Puntos pequeños para los puntos intermedios
      else {
        ctx.fillStyle = color
        ctx.beginPath()
        ctx.arc(x, y, cellWidth * 0.12, 0, 2 * Math.PI)
        ctx.fill()
      }
    })
  })
  
  // Dibujar la posición actual de cada robot (encima de la ruta)
  Object.entries(robotPositions.value).forEach(([robotId, position]) => {
    const color = robotColors[robotId] || '#FF0000'
    const [row, col] = position
    
    const x = col * cellWidth + cellWidth / 2
    const y = row * cellHeight + cellHeight / 2
    
    // Dibujar el robot como un círculo grande con borde y sombra
    ctx.shadowColor = 'rgba(0, 0, 0, 0.5)'
    ctx.shadowBlur = 10
    ctx.shadowOffsetX = 3
    ctx.shadowOffsetY = 3
    
    // Círculo exterior (robot)
    ctx.fillStyle = color
    ctx.beginPath()
    ctx.arc(x, y, cellWidth * 0.35, 0, 2 * Math.PI)
    ctx.fill()
    
    // Borde blanco grueso
    ctx.shadowColor = 'transparent'
    ctx.strokeStyle = '#FFFFFF'
    ctx.lineWidth = 4
    ctx.stroke()
    
    // Borde negro fino
    ctx.strokeStyle = '#000000'
    ctx.lineWidth = 2
    ctx.stroke()
    
    // Dibujar el ID del robot en el centro
    ctx.fillStyle = '#FFFFFF'
    ctx.font = `bold ${cellWidth * 0.2}px Arial`
    ctx.textAlign = 'center'
    ctx.textBaseline = 'middle'
    ctx.shadowColor = 'rgba(0, 0, 0, 0.8)'
    ctx.shadowBlur = 3
    const robotNumber = robotId.replace('robot', '')
    ctx.fillText(robotNumber, x, y)
    
    // Resetear sombra
    ctx.shadowColor = 'transparent'
    ctx.shadowBlur = 0
    ctx.shadowOffsetX = 0
    ctx.shadowOffsetY = 0
  })
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
/* Solo estilos necesarios que no se pueden hacer con Tailwind */
</style>

