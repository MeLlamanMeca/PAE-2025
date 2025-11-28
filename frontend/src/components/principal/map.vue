<template>
<div class="flex justify-center items-start w-full overflow-hidden">
  <div class="bg-white rounded-xl shadow-lg border-2 border-gray-200 p-4 max-w-full">
    <canvas 
      ref="canvas"
      class="border-2 border-gray-300 rounded-lg block shadow-inner max-w-full h-auto" 
    />
  </div>
</div>
</template>

<script setup lang="ts">
import { ref, onMounted, watch, nextTick } from 'vue'

// Props que recibe el componente
const props = defineProps<{
  grid: string[][]
  zones: Record<string, string> | null
  robotPaths: Record<string, [number, number][]>
  robotPositions: Record<string, [number, number]>
  robotInitialPositions?: Record<string, [number, number]>
  colorMap: Record<string, string>
}>()

const canvas = ref<HTMLCanvasElement | null>(null)

// Tamaño del canvas y escala
const CANVAS_SIZE = 800 // Tamaño fijo del canvas en píxeles
const CELL_SIZE = 20 // Tamaño de cada celda del grid en píxeles
const ROBOT_SIZE = 12 // Tamaño del robot en píxeles

function getColor(id: string): string {
  return props.colorMap[id] || '#' + Math.floor(Math.random()*16777215).toString(16)
}

function drawGrid() {
  console.log('Dibujando grid y rutas de robots')
  const c = canvas.value
  if (!c || !props.grid.length) return
  const ctx = c.getContext('2d')!
  const rows = props.grid.length
  const cols = props.grid[0].length
  
  // Establecer tamaño del canvas
  c.width = CANVAS_SIZE
  c.height = CANVAS_SIZE
  
  // Calcular el tamaño de cada celda para que quepa todo el grid
  const cellWidth = CANVAS_SIZE / cols
  const cellHeight = CANVAS_SIZE / rows
  
  ctx.clearRect(0, 0, c.width, c.height)
  
  // Dibujar el grid
  for (let row = 0; row < rows; row++) {
    for (let col = 0; col < cols; col++) {
      const cellValue = props.grid[row][col]
      const color = getColor(cellValue)
      ctx.fillStyle = color
      ctx.fillRect(col * cellWidth, row * cellHeight, cellWidth, cellHeight)
      
      // Borde de las celdas
      ctx.strokeStyle = '#333'
      ctx.lineWidth = 0.5
      ctx.strokeRect(col * cellWidth, row * cellHeight, cellWidth, cellHeight)
    }
  }
  
  // Dibujar rutas de robots con líneas fluidas
  const robotColors: Record<string, string> = {
    'robot1': '#FF00FF',
    'robot2': '#00FF00',
    'robot3': '#0000FF',
  }
  
  Object.entries(props.robotPaths).forEach(([robotId, path]) => {
    const color = robotColors[robotId] || '#FF0000'
    if (path.length === 0) return
    
    ctx.globalAlpha = 0.5
    ctx.strokeStyle = color
    ctx.lineWidth = 4
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
    
    // Marcar puntos de inicio y fin de la ruta
    path.forEach(([row, col], index) => {
      const x = col * cellWidth + cellWidth / 2
      const y = row * cellHeight + cellHeight / 2
      
      if (index === 0) {
        // Punto de inicio
        ctx.fillStyle = color
        ctx.globalAlpha = 0.6
        ctx.beginPath()
        ctx.arc(x, y, Math.min(cellWidth, cellHeight) * 0.3, 0, 2 * Math.PI)
        ctx.fill()
        ctx.globalAlpha = 1.0
      } else if (index === path.length - 1) {
        // Punto de fin
        ctx.fillStyle = color
        ctx.globalAlpha = 0.8
        ctx.beginPath()
        ctx.arc(x, y, Math.min(cellWidth, cellHeight) * 0.35, 0, 2 * Math.PI)
        ctx.fill()
        ctx.globalAlpha = 1.0
      }
    })
  })
  
  // Dibujar robots
  const allRobotIds = new Set([
    ...Object.keys(props.robotInitialPositions || {}),
    ...Object.keys(props.robotPositions)
  ])
  
  allRobotIds.forEach(robotId => {
    const position = props.robotPositions[robotId] || (props.robotInitialPositions && props.robotInitialPositions[robotId])
    if (!Array.isArray(position) || position.length !== 2) return
    
    const color = robotColors[robotId] || '#FF0000'
    const [row, col] = position
    
    // Posición centrada en la celda
    const x = col * cellWidth + cellWidth / 2
    const y = row * cellHeight + cellHeight / 2
    
    // Tamaño del robot proporcional al tamaño de celda
    const robotRadius = Math.min(cellWidth, cellHeight) * 0.4
    
    // Dibujar sombra
    ctx.shadowColor = 'rgba(0, 0, 0, 0.5)'
    ctx.shadowBlur = 8
    ctx.shadowOffsetX = 2
    ctx.shadowOffsetY = 2
    
    // Cuerpo del robot
    ctx.fillStyle = color
    ctx.beginPath()
    ctx.arc(x, y, robotRadius, 0, 2 * Math.PI)
    ctx.fill()
    
    ctx.shadowColor = 'transparent'
    
    // Borde blanco
    ctx.strokeStyle = '#FFFFFF'
    ctx.lineWidth = 3
    ctx.stroke()
    
    // Borde negro fino
    ctx.strokeStyle = '#000000'
    ctx.lineWidth = 1.5
    ctx.stroke()
    
    // Número del robot
    ctx.fillStyle = '#FFFFFF'
    ctx.font = `bold ${robotRadius * 0.8}px Arial`
    ctx.textAlign = 'center'
    ctx.textBaseline = 'middle'
    ctx.shadowColor = 'rgba(0, 0, 0, 0.8)'
    ctx.shadowBlur = 2
    
    const robotNumber = robotId.replace('robot', '')
    ctx.fillText(robotNumber, x, y)
    
    // Limpiar sombras
    ctx.shadowColor = 'transparent'
    ctx.shadowBlur = 0
    ctx.shadowOffsetX = 0
    ctx.shadowOffsetY = 0
  })
}

onMounted(() => {
  nextTick(drawGrid)
})

watch(() => [props.grid, props.robotPaths, props.robotPositions], () => {
  nextTick(drawGrid)
}, { deep: true })
</script>