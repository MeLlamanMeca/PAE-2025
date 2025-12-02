<template>
  <div>
    <div class="flex justify-between items-center mb-5">
      <h3 class="text-xl sm:text-2xl font-bold text-gray-800 flex items-center gap-2">
        <svg class="w-6 h-6 text-blue-600" fill="none" stroke="currentColor" viewBox="0 0 24 24">
          <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M9 5H7a2 2 0 00-2 2v12a2 2 0 002 2h10a2 2 0 002-2V7a2 2 0 00-2-2h-2M9 5a2 2 0 002 2h2a2 2 0 002-2M9 5a2 2 0 012-2h2a2 2 0 012 2m-6 9l2 2 4-4"></path>
        </svg>
        Lista de Tareas
      </h3>
      <button
        @click="openCreateTaskModal"
        class="inline-flex items-center gap-2 px-4 py-2.5 bg-gradient-to-r from-blue-600 to-indigo-600 hover:from-blue-700 hover:to-indigo-700 text-white font-semibold rounded-lg shadow-lg hover:shadow-xl transition-all duration-300 text-xs sm:text-sm transform hover:scale-105"
      >
        <svg class="w-4 h-4" fill="none" stroke="currentColor" viewBox="0 0 24 24">
          <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M12 4v16m8-8H4"></path>
        </svg>
        Añadir Tarea
      </button>
    </div>

    <!-- Tabla de tareas pendientes -->
    <div class="overflow-x-auto rounded-xl border border-gray-200 shadow-md">
      <table class="w-full border-collapse bg-white">
        <thead>
          <tr class="bg-gradient-to-r from-gray-50 to-gray-100 border-b-2 border-gray-200">
            <th class="px-4 py-3 text-left text-xs sm:text-sm font-bold text-gray-700 uppercase tracking-wider">Nombre</th>
            <th class="px-4 py-3 text-left text-xs sm:text-sm font-bold text-gray-700 uppercase tracking-wider">Descripción</th>
            <th class="px-4 py-3 text-left text-xs sm:text-sm font-bold text-gray-700 uppercase tracking-wider">Estado</th>
            <th class="px-4 py-3 text-center text-xs sm:text-sm font-bold text-gray-700 uppercase tracking-wider">Acciones</th>
          </tr>
        </thead>
        <tbody>
          <tr v-if="pendingTasks.length === 0">
            <td colspan="4" class="px-4 py-12 text-center">
              <div class="flex flex-col items-center justify-center">
                <svg class="w-16 h-16 text-gray-300 mb-3" fill="none" stroke="currentColor" viewBox="0 0 24 24">
                  <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M9 12h6m-6 4h6m2 5H7a2 2 0 01-2-2V5a2 2 0 012-2h5.586a1 1 0 01.707.293l5.414 5.414a1 1 0 01.293.707V19a2 2 0 01-2 2z"></path>
                </svg>
                <p class="text-gray-500 text-sm font-medium">No hay tareas pendientes</p>
              </div>
            </td>
          </tr>
          <tr 
            v-for="task in pendingTasks" 
            :key="task.id"
            class="border-b border-gray-100 hover:bg-blue-50 transition-all duration-200"
          >
            <td class="px-4 py-3 text-xs sm:text-sm font-medium text-gray-900">{{ task.name }}</td>
            <td class="px-4 py-3 text-xs sm:text-sm text-gray-600">{{ task.description || '-' }}</td>
            <td class="px-4 py-3">
              <span :class="[
                'inline-flex items-center px-2.5 py-1 rounded-full text-xs font-bold',
                task.status === 'pending' ? 'bg-yellow-100 text-yellow-800 border border-yellow-200' :
                task.status === 'active' ? 'bg-green-100 text-green-800 border border-green-200' :
                'bg-gray-100 text-gray-800 border border-gray-200'
              ]">
                {{ task.statusLabel }}
              </span>
            </td>
            <td class="px-4 py-3 text-center">
              <div class="flex items-center justify-center gap-2">
                <button
                  @click="openEditTaskModal(task)"
                  class="inline-flex items-center px-3 py-1.5 bg-yellow-500 hover:bg-yellow-600 text-white rounded-lg text-xs font-semibold transition-all duration-200 shadow hover:shadow-md transform hover:scale-105"
                >
                  <svg class="w-3 h-3 mr-1" fill="none" stroke="currentColor" viewBox="0 0 24 24">
                    <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M11 5H6a2 2 0 00-2 2v11a2 2 0 002 2h11a2 2 0 002-2v-5m-1.414-9.414a2 2 0 112.828 2.828L11.828 15H9v-2.828l8.586-8.586z"></path>
                  </svg>
                  Editar
                </button>
                <button
                  @click="deleteTask(task.id)"
                  class="inline-flex items-center px-3 py-1.5 bg-red-500 hover:bg-red-600 text-white rounded-lg text-xs font-semibold transition-all duration-200 shadow hover:shadow-md transform hover:scale-105"
                >
                  <svg class="w-3 h-3 mr-1" fill="none" stroke="currentColor" viewBox="0 0 24 24">
                    <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M19 7l-.867 12.142A2 2 0 0116.138 21H7.862a2 2 0 01-1.995-1.858L5 7m5 4v6m4-6v6m1-10V4a1 1 0 00-1-1h-4a1 1 0 00-1 1v3M4 7h16"></path>
                  </svg>
                  Eliminar
                </button>
              </div>
            </td>
          </tr>
        </tbody>
      </table>
    </div>

    <!-- Modal para crear/editar tarea -->
    <Teleport to="body">
      <div 
        v-if="showTaskModal"
        class="fixed inset-0 bg-black/60 backdrop-blur-sm flex items-center justify-center z-[60] p-4 animate-fadeIn"
        @click.self="closeTaskModal"
      >
        <div class="bg-white rounded-2xl shadow-2xl max-w-lg w-full p-6 sm:p-8 relative transform transition-all animate-slideUp">
          <!-- Botón cerrar -->
          <button
            @click="closeTaskModal"
            class="absolute top-4 right-4 text-gray-400 hover:text-gray-600 hover:bg-gray-100 rounded-full p-2 transition-all duration-200"
          >
            <svg class="w-6 h-6" fill="none" stroke="currentColor" viewBox="0 0 24 24">
              <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M6 18L18 6M6 6l12 12"></path>
            </svg>
          </button>
          
          <!-- Título del modal -->
          <div class="mb-6">
            <h3 class="text-xl sm:text-2xl font-bold text-gray-800 flex items-center gap-2">
              <svg class="w-6 h-6 text-blue-600" fill="none" stroke="currentColor" viewBox="0 0 24 24">
                <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M9 5H7a2 2 0 00-2 2v12a2 2 0 002 2h10a2 2 0 002-2V7a2 2 0 00-2-2h-2M9 5a2 2 0 002 2h2a2 2 0 002-2M9 5a2 2 0 012-2h2a2 2 0 012 2"></path>
              </svg>
              {{ editingTask ? 'Editar Tarea' : 'Añadir Nueva Tarea' }}
            </h3>
          </div>

          <!-- Formulario -->
          <form @submit.prevent="saveTask" class="space-y-5">
            <!-- Nombre -->
            <div>
              <label class="block text-sm font-bold text-gray-700 mb-2">Nombre *</label>
              <input
                v-model="taskForm.name"
                type="text"
                required
                class="w-full px-4 py-2.5 border-2 border-gray-200 rounded-lg focus:outline-none focus:ring-2 focus:ring-blue-500 focus:border-transparent text-sm transition-all duration-200"
                :placeholder="editingTask ? editingTask.name : 'Nombre de la tarea'"
              />
            </div>

            <!-- Descripción -->
            <div>
              <label class="block text-sm font-bold text-gray-700 mb-2">Descripción</label>
              <textarea
                v-model="taskForm.description"
                rows="3"
                class="w-full px-4 py-2.5 border-2 border-gray-200 rounded-lg focus:outline-none focus:ring-2 focus:ring-blue-500 focus:border-transparent text-sm transition-all duration-200 resize-none"
                :placeholder="editingTask ? editingTask.description : 'Descripción de la tarea'"
              ></textarea>
            </div>

            <!-- POI Inicio -->
            <div>
              <label class="block text-sm font-bold text-gray-700 mb-2">POI Inicio *</label>
              <select
                v-model="taskForm.poiStart"
                required
                class="w-full px-4 py-2.5 border-2 border-gray-200 rounded-lg focus:outline-none focus:ring-2 focus:ring-blue-500 focus:border-transparent text-sm transition-all duration-200 bg-white cursor-pointer"
              >
                <option value="" disabled>Seleccionar POI</option>
                <option v-for="poi in availablePOIs" :key="poi.id" :value="poi.id">
                  {{ poi.id }} - {{ poi.name }}
                </option>
              </select>
            </div>

            <!-- POI Final -->
            <div>
              <label class="block text-sm font-bold text-gray-700 mb-2">POI Final *</label>
              <select
                v-model="taskForm.poiEnd"
                required
                class="w-full px-4 py-2.5 border-2 border-gray-200 rounded-lg focus:outline-none focus:ring-2 focus:ring-blue-500 focus:border-transparent text-sm transition-all duration-200 bg-white cursor-pointer"
              >
                <option value="" disabled>Seleccionar POI</option>
                <option v-for="poi in availablePOIs" :key="poi.id" :value="poi.id">
                  {{ poi.id }} - {{ poi.name }}
                </option>
              </select>
            </div>

            <!-- Peso -->
            <div>
              <label class="block text-sm font-bold text-gray-700 mb-2">Peso (kg) *</label>
              <input
                v-model.number="taskForm.weight"
                type="number"
                min="0"
                required
                class="w-full px-4 py-2.5 border-2 border-gray-200 rounded-lg focus:outline-none focus:ring-2 focus:ring-blue-500 focus:border-transparent text-sm transition-all duration-200"
                placeholder="Peso de la carga"
              />
            </div>

            <!-- Botones -->
            <div class="flex justify-end gap-3 mt-8 pt-4 border-t border-gray-200">
              <button
                type="button"
                @click="closeTaskModal"
                class="px-5 py-2.5 bg-gray-200 hover:bg-gray-300 text-gray-800 font-semibold rounded-lg transition-all duration-200 text-sm"
              >
                Cancelar
              </button>
              <button
                type="submit"
                class="px-5 py-2.5 bg-gradient-to-r from-blue-600 to-indigo-600 hover:from-blue-700 hover:to-indigo-700 text-white font-semibold rounded-lg shadow-lg hover:shadow-xl transition-all duration-300 text-sm transform hover:scale-105"
              >
                {{ editingTask ? 'Guardar Cambios' : 'Crear Tarea' }}
              </button>
            </div>
          </form>
        </div>
      </div>
    </Teleport>
  </div>
</template>

<script setup lang="ts">
import { ref, computed } from 'vue'

interface Task {
  id: string
  robotId: string
  name: string
  description?: string
  poiStart?: string
  poiEnd?: string
  pointIni?: { x: number, y: number }
  pointFin?: { x: number, y: number }
  weight?: number
  status: 'completed' | 'active' | 'pending'
  statusLabel: string
}

interface POI {
  id: string
  name: string
  position?: { x: number, y: number }
}

const props = defineProps<{
  allTasksList: Task[],
  pois: any[]
}>()

const emit = defineEmits(['create-task', 'delete-task'])

// Estado del modal
const showTaskModal = ref(false)
const editingTask = ref<Task | null>(null)

// Formulario de tarea
const taskForm = ref({
  name: '',
  description: '',
  poiStart: '',
  poiEnd: '',
  weight: 0
})

// Lista de POIs disponibles
const availablePOIs = computed(() => {
  return props.pois
    .filter(poi => {
      const id = String(poi.id)
      // Filtrar IDs estructurales (253=WALL, 254=EMPTY, 255=OUT_OF_BOUNDS)
      return !['253', '254', '255','252','251'].includes(id)
    })
    .map(poi => ({
      id: String(poi.id),
      name: poi.name || `POI ${poi.id}`,
      position: poi.position
    }))
})

// Filtrar solo tareas pendientes
const pendingTasks = computed(() => {
  return props.allTasksList.filter(task => task.status === 'pending')
})

// Abrir modal para crear tarea
function openCreateTaskModal() {
  editingTask.value = null
  taskForm.value = {
    name: '',
    description: '',
    poiStart: '',
    poiEnd: '',
    weight: 0
  }
  showTaskModal.value = true
}

// Abrir modal para editar tarea
function openEditTaskModal(task: Task) {
  editingTask.value = task
  
  // Buscar IDs de POIs basados en las coordenadas
  let startPoiId = ''
  let endPoiId = ''
  
  if (task.pointIni) {
    const p = props.pois.find(poi => poi.position && poi.position.x === task.pointIni?.x && poi.position.y === task.pointIni?.y)
    if (p) startPoiId = String(p.id)
  }
  
  if (task.pointFin) {
    const p = props.pois.find(poi => poi.position && poi.position.x === task.pointFin?.x && poi.position.y === task.pointFin?.y)
    if (p) endPoiId = String(p.id)
  }

  taskForm.value = {
    name: task.name,
    description: task.description || '',
    poiStart: startPoiId,
    poiEnd: endPoiId,
    weight: task.weight || 0
  }
  showTaskModal.value = true
}

// Cerrar modal
function closeTaskModal() {
  showTaskModal.value = false
  editingTask.value = null
  taskForm.value = {
    name: '',
    description: '',
    poiStart: '',
    poiEnd: '',
    weight: 0
  }
}

// Guardar tarea (crear o editar)
function saveTask() {
  // Buscar coordenadas de los POIs seleccionados
  const startPoi = props.pois.find(p => String(p.id) === taskForm.value.poiStart)
  const endPoi = props.pois.find(p => String(p.id) === taskForm.value.poiEnd)
  
  if (!startPoi || !endPoi) {
    console.error('POIs seleccionados no encontrados o sin posición')
    return
  }

  if (editingTask.value) {
    // Editar tarea existente: Eliminar y Crear nueva
    console.log('Editando tarea (Delete + Create):', editingTask.value.id)
    
    // 1. Eliminar tarea anterior
    emit('delete-task', {
      robotId: editingTask.value.robotId,
      taskId: editingTask.value.id
    })

    // 2. Crear nueva tarea con los datos actualizados
    emit('create-task', {
      pointIni: startPoi.position,
      pointFin: endPoi.position,
      weight: taskForm.value.weight
    })

  } else {
    // Crear nueva tarea
    console.log('Creando nueva tarea:', taskForm.value)
    
    emit('create-task', {
      pointIni: startPoi.position,
      pointFin: endPoi.position,
      weight: taskForm.value.weight
    })
  }
  closeTaskModal()
}

// Eliminar tarea
function deleteTask(taskId: string) {
  if (confirm('¿Estás seguro de que quieres eliminar esta tarea?')) {
    console.log('Eliminando tarea:', taskId)
    const task = props.allTasksList.find(t => t.id === taskId)
    if (task) {
      emit('delete-task', {
        robotId: task.robotId,
        taskId: taskId
      })
    }
  }
}
</script>