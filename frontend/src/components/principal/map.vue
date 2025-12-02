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
  pois?: any[]
  robotPaths: Record<string, [number, number][]>
  robotPositions: Record<string, [number, number]>
  robotInitialPositions?: Record<string, [number, number]>
  colorMap: Record<string, string>
}>()

const canvas = ref<HTMLCanvasElement | null>(null)

// Configuración visual
const MAX_CANVAS_SIZE = 800 // Tamaño máximo permitido
const MIN_CELL_SIZE = 30    // Tamaño mínimo de celda para visibilidad

function getColor(id: string): string {
  return props.colorMap[id] || '#' + Math.floor(Math.random()*16777215).toString(16)
}

function drawGrid() {
  const c = canvas.value
  if (!c || !props.grid.length) return
  const ctx = c.getContext('2d')!
  const rows = props.grid.length
  const cols = props.grid[0].length
  
  // 1. Calcular tamaño de celda óptimo
  // Queremos que sea lo más grande posible sin pasarnos del MAX_CANVAS_SIZE
  const maxCellWidth = MAX_CANVAS_SIZE / cols
  const maxCellHeight = MAX_CANVAS_SIZE / rows
  const cellSize = Math.floor(Math.min(maxCellWidth, maxCellHeight))
  
  // 2. Ajustar el tamaño del canvas exactamente a la matriz
  c.width = cols * cellSize
  c.height = rows * cellSize
  
  // Limpiar
  ctx.clearRect(0, 0, c.width, c.height)
  
  // 3. Dibujar el grid (fondo)
  for (let row = 0; row < rows; row++) {
    for (let col = 0; col < cols; col++) {
      const cellValue = props.grid[row][col]
      const x = col * cellSize
      const y = row * cellSize
      
      // Fondo de celda
      const color = getColor(cellValue)
      ctx.fillStyle = color
      ctx.fillRect(x, y, cellSize, cellSize)
      
      // Borde sutil
      ctx.strokeStyle = 'rgba(0,0,0,0.1)'
      ctx.lineWidth = 1
      ctx.strokeRect(x, y, cellSize, cellSize)
    }
  }

  // 4. Dibujar POIs (Puntos de Interés)
  if (props.pois) {
    props.pois.forEach(poi => {
      if (poi.position) {
        const col = poi.position.x
        const row = poi.position.y
        const x = col * cellSize
        const y = row * cellSize
        const color = getColor(String(poi.id))
        
        // Sombra para dar efecto de elevación
        
       
        
        // Dibujar POI con un margen interno
        const padding = cellSize * 0.1
        ctx.fillStyle = color
        ctx.fillRect(x + padding, y + padding, cellSize - padding*2, cellSize - padding*2)
        
        // Borde blanco para resaltar
       
       // ctx.shadowColor = 'transparent'
        //ctx.strokeStyle = '#fff'
      //  ctx.lineWidth = 2
       // ctx.strokeRect(x + padding, y + padding, cellSize - padding*2, cellSize - padding*2)
        
        // ID del POI
        ctx.fillStyle = '#000'
        ctx.font = `bold ${cellSize * 0.4}px Arial`
        ctx.textAlign = 'center'
        ctx.textBaseline = 'middle'
        ctx.fillText(String(poi.id), x + cellSize/2, y + cellSize/2)
      }
    })
  }
  
  // 5. Dibujar rutas de robots
  const robotColors: Record<string, string> = {
    'robot1': '#FF00FF',
    'robot2': '#00FF00',
    'robot3': '#0000FF',
  }
  
  Object.entries(props.robotPaths).forEach(([robotId, path]) => {
    const color = robotColors[robotId] || '#FF0000'
    if (path.length === 0) return
    
    ctx.lineCap = 'round'
    ctx.lineJoin = 'round'
    
    // Sombra suave para la ruta
    
    //ctx.shadowColor = color
    //ctx.shadowBlur = 5
    //ctx.globalAlpha = 0.6
    
    ctx.beginPath()
    ctx.strokeStyle = color
    ctx.lineWidth = cellSize * 0.15 // Grosor proporcional
    
    for (let i = 0; i < path.length; i++) {
      const [row, col] = path[i]
      const x = col * cellSize + cellSize / 2
      const y = row * cellSize + cellSize / 2
      if (i === 0) ctx.moveTo(x, y)
      else ctx.lineTo(x, y)
    }
    ctx.stroke()
    
    ctx.globalAlpha = 1.0
    ctx.shadowBlur = 0
  })
  
  // 6. Dibujar robots (con gradientes y efectos)
  const allRobotIds = new Set([
    ...Object.keys(props.robotInitialPositions || {}),
    ...Object.keys(props.robotPositions)
  ])
  
  allRobotIds.forEach(robotId => {
    const position = props.robotPositions[robotId] || (props.robotInitialPositions && props.robotInitialPositions[robotId])
    if (!Array.isArray(position) || position.length !== 2) return
    
    const baseColor = robotColors[robotId] || '#FF0000'
    const [row, col] = position
    
    const x = col * cellSize + cellSize / 2
    const y = row * cellSize + cellSize / 2
    const radius = cellSize * 0.35
    
    // Sombra del robot
    ctx.shadowColor = 'rgba(0, 0, 0, 0.5)'
    ctx.shadowBlur = 10
    ctx.shadowOffsetY = 4
    
    // Gradiente radial para efecto 3D
    const gradient = ctx.createRadialGradient(x - radius/3, y - radius/3, radius/10, x, y, radius)
    gradient.addColorStop(0, '#FFFFFF') // Brillo especular
    gradient.addColorStop(0.3, baseColor)
    gradient.addColorStop(1, '#000000') // Borde oscuro
    
    ctx.fillStyle = gradient
    ctx.beginPath()
    ctx.arc(x, y, radius, 0, 2 * Math.PI)
    ctx.fill()
    
    // Resetear sombras
    ctx.shadowColor = 'transparent'
    ctx.shadowBlur = 0
    ctx.shadowOffsetY = 0
    
    // Anillo exterior blanco
    ctx.strokeStyle = '#FFFFFF'
    ctx.lineWidth = 2
    ctx.stroke()
    
    // Número del robot
    ctx.fillStyle = '#FFFFFF'
    ctx.font = `bold ${radius}px Arial`
    ctx.textAlign = 'center'
    ctx.textBaseline = 'middle'
    // Sombra texto
    ctx.shadowColor = 'rgba(0,0,0,0.8)'
    ctx.shadowBlur = 2
    
    const robotNumber = robotId.replace('robot', '')
    ctx.fillText(robotNumber, x, y)
    
    ctx.shadowBlur = 0
  })
}

onMounted(() => {
  nextTick(drawGrid)
})

watch(() => [props.grid, props.robotPaths, props.robotPositions, props.pois], () => {
  nextTick(drawGrid)
}, { deep: true })
</script>