<template>
  <div class="w-full">
    <!-- Navegación con tabs modernos -->
    <div class="flex border-b border-gray-200 bg-gray-50">
      <button
        @click="activePanel = 'tasks'"
        :class="[
          'flex-1 px-6 py-4 text-sm sm:text-base font-semibold transition-all duration-300 relative',
          activePanel === 'tasks' 
            ? 'bg-white text-blue-600 shadow-sm' 
            : 'text-gray-600 hover:text-gray-800 hover:bg-gray-100'
        ]"
      >
        <span class="flex items-center justify-center gap-2">
          <svg class="w-5 h-5" fill="none" stroke="currentColor" viewBox="0 0 24 24">
            <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M9 5H7a2 2 0 00-2 2v12a2 2 0 002 2h10a2 2 0 002-2V7a2 2 0 00-2-2h-2M9 5a2 2 0 002 2h2a2 2 0 002-2M9 5a2 2 0 012-2h2a2 2 0 012 2m-6 9l2 2 4-4"></path>
          </svg>
          Panel de Tareas
        </span>
        <div 
          v-if="activePanel === 'tasks'"
          class="absolute bottom-0 left-0 right-0 h-1 bg-gradient-to-r from-blue-600 to-indigo-600 rounded-t-full"
        ></div>
      </button>
      <button
        @click="activePanel = 'poi'"
        :class="[
          'flex-1 px-6 py-4 text-sm sm:text-base font-semibold transition-all duration-300 relative',
          activePanel === 'poi' 
            ? 'bg-white text-purple-600 shadow-sm' 
            : 'text-gray-600 hover:text-gray-800 hover:bg-gray-100'
        ]"
      >
        <span class="flex items-center justify-center gap-2">
          <svg class="w-5 h-5" fill="none" stroke="currentColor" viewBox="0 0 24 24">
            <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M17.657 16.657L13.414 20.9a1.998 1.998 0 01-2.827 0l-4.244-4.243a8 8 0 1111.314 0z"></path>
            <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M15 11a3 3 0 11-6 0 3 3 0 016 0z"></path>
          </svg>
          Panel de POI
        </span>
        <div 
          v-if="activePanel === 'poi'"
          class="absolute bottom-0 left-0 right-0 h-1 bg-gradient-to-r from-purple-600 to-pink-600 rounded-t-full"
        ></div>
      </button>
    </div>

    <!-- Contenido del panel activo -->
    <div class="p-5 sm:p-6 bg-white">
      <Transition name="fade" mode="out-in">
        <PanelTasks v-if="activePanel === 'tasks'" :allTasksList="allTasksList" />
        <PanelPOI v-else />
      </Transition>
    </div>
  </div>
</template>

<script setup lang="ts">
import { ref } from 'vue'
import PanelTasks from './panelTasks.vue'
import PanelPOI from './panelPOI.vue'

interface Task {
  id: string
  robotId: string
  name: string
  status: 'completed' | 'active' | 'pending'
  statusLabel: string
}

// Props
defineProps<{
  allTasksList: Task[]
}>()

// Estado del panel activo
const activePanel = ref<'tasks' | 'poi'>('tasks')
</script>

<style scoped>
.fade-enter-active, .fade-leave-active {
  transition: opacity 0.2s ease;
}

.fade-enter-from, .fade-leave-to {
  opacity: 0;
}
</style>