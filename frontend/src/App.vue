<template>
  <div id="app">
    <h1>SARA - Robot Management System</h1>
    
    <div class="status">
      <span :class="['connection-status', connected ? 'connected' : 'disconnected']">
        
      </span>
    </div>

    <div class="content">
      <div class="canvas-container">
        <canvas ref="canvas" width="800" height="800" />
      </div>

      <div class="info-panel">
        <div v-if="zones" class="zones-section">
          <h3>Zones</h3>
          <div v-for="(name, id) in zones" :key="id" class="zone-item">
            <span class="zone-color" :style="{ backgroundColor: getColor(id) }"></span>
            <span>{{ id }}: {{ name }}</span>
          </div>
        </div>
<!-- Panel de Nodos Seleccionados 
        <div v-if="tasks" class="tasks-section">
          <h3>Tasks</h3>
          <div v-for="(taskList, robot) in tasks" :key="robot" class="robot-tasks">
            <h4>{{ robot }}</h4>
            <ul>
              <li v-for="task in taskList" :key="task">{{ task }}</li>
            </ul>
          </div>
          
        </div>
-->
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

function getColor(id: string): string {
  return colorMap[id] || '#' + Math.floor(Math.random()*16777215).toString(16)
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
}

onMounted(() => {
  connectWebSocket()
  
  // Para pruebas, simular datos después de 1 segundo si no hay conexión
  setTimeout(() => {
    if (!connected.value) {
      console.log('Using mock data for testing...')
     
    }
  }, 1000)
})

onUnmounted(() => {
  if (ws) {
    ws.close()
  }
})
</script>

<style scoped>
#app {
  font-family: Arial, sans-serif;
  padding: 20px;
}

h1 {
  text-align: center;
  color: #333;
  margin-bottom: 20px;
}

.status {
  text-align: center;
  margin-bottom: 20px;
}

.connection-status {
  display: inline-block;
  padding: 8px 16px;
  border-radius: 20px;
  font-weight: bold;
  font-size: 14px;
}

.connection-status.connected {
  background-color: #e8f5e9;
  color: #2e7d32;
}

.connection-status.disconnected {
  background-color: #ffebee;
  color: #c62828;
}

.content {
  display: flex;
  gap: 30px;
  justify-content: center;
  flex-wrap: wrap;
}

.canvas-container {
  flex-shrink: 0;
}

canvas {
  border: 3px solid #333;
  display: block;
  box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
}

.info-panel {
  min-width: 300px;
  max-width: 400px;
}

.zones-section,
.tasks-section {
  background: #f5f5f5;
  padding: 20px;
  border-radius: 8px;
  margin-bottom: 20px;
}

.zones-section h3,
.tasks-section h3 {
  margin-top: 0;
  color: #333;
  border-bottom: 2px solid #ddd;
  padding-bottom: 10px;
}

.zone-item {
  display: flex;
  align-items: center;
  gap: 10px;
  margin: 10px 0;
  font-size: 16px;
}

.zone-color {
  width: 30px;
  height: 30px;
  border-radius: 4px;
  border: 2px solid #333;
  display: inline-block;
}

.robot-tasks {
  margin-bottom: 15px;
}

.robot-tasks h4 {
  margin: 10px 0 5px 0;
  color: #555;
  font-size: 16px;
}

.robot-tasks ul {
  margin: 5px 0;
  padding-left: 20px;
}

.robot-tasks li {
  margin: 5px 0;
  color: #666;
}
</style>
