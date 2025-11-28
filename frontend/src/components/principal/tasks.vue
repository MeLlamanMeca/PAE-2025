<template>
  <div class="w-full lg:w-auto lg:min-w-[300px] lg:max-w-[350px]">
    <div class="bg-white rounded-xl shadow-lg border border-gray-200 overflow-hidden">
      <!-- Header -->
      <div class="bg-gradient-to-r from-blue-600 to-indigo-600 px-5 py-4">
        <h3 class="text-white font-bold text-lg sm:text-xl flex items-center gap-2">
          <svg class="w-5 h-5" fill="none" stroke="currentColor" viewBox="0 0 24 24">
            <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M9 5H7a2 2 0 00-2 2v12a2 2 0 002 2h10a2 2 0 002-2V7a2 2 0 00-2-2h-2M9 5a2 2 0 002 2h2a2 2 0 002-2M9 5a2 2 0 012-2h2a2 2 0 012 2m-6 9l2 2 4-4"></path>
          </svg>
          Lista de Tareas
        </h3>
      </div>
      
      <!-- Content -->
      <div class="p-4 sm:p-5 max-h-[600px] overflow-y-auto">
        <div v-if="activeTasks.length === 0" class="text-center py-12">
          <div class="inline-flex items-center justify-center w-16 h-16 bg-gray-100 rounded-full mb-4">
            <svg class="w-8 h-8 text-gray-400" fill="none" stroke="currentColor" viewBox="0 0 24 24">
              <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M9 12l2 2 4-4m6 2a9 9 0 11-18 0 9 9 0 0118 0z"></path>
            </svg>
          </div>
          <p class="text-gray-500 text-sm">No hay tareas pendientes o activas</p>
        </div>
        
        <div class="flex flex-col gap-3">
          <div 
            v-for="task in activeTasks" 
            :key="task.id"
            :class="[
              'p-4 rounded-lg border-2 transition-all duration-300 transform hover:scale-[1.02]',
              task.status === 'active' 
                ? 'bg-gradient-to-br from-green-500 to-emerald-600 border-green-600 text-white shadow-lg shadow-green-200 animate-pulse' :
                'bg-white border-gray-200 text-gray-800 hover:border-blue-300 hover:shadow-md'
            ]"
          >
            <div class="flex justify-between items-start mb-2">
              <div class="flex items-center gap-2">
                <div :class="[
                  'w-8 h-8 rounded-lg flex items-center justify-center text-xs font-bold',
                  task.status === 'active' 
                    ? 'bg-white/20 text-white' 
                    : 'bg-blue-100 text-blue-700'
                ]">
                  {{ task.robotId.replace('robot', 'R') }}
                </div>
              </div>
              <span :class="[
                'text-xs font-semibold px-2.5 py-1 rounded-full',
                task.status === 'active' 
                  ? 'bg-white/25 text-white' 
                  : 'bg-blue-100 text-blue-700'
              ]">
                {{ task.statusLabel }}
              </span>
            </div>
            <div class="text-sm sm:text-base font-semibold mt-2">{{ task.name }}</div>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<script setup lang="ts">
import { computed } from 'vue'

// Definir la interfaz de la tarea
interface Task {
  id: string
  robotId: string
  name: string
  status: 'completed' | 'active' | 'pending'
  statusLabel: string
}

// Props que recibe el componente
const props = defineProps<{
  allTasksList: Task[]
}>()

// Computed para filtrar solo tareas activas y pendientes (excluir completadas)
const activeTasks = computed(() => {
  return props.allTasksList.filter(task => task.status !== 'completed')
})
</script>