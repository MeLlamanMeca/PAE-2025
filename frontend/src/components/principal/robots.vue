<template>
  <div v-if="robotsList.length > 0" class="mt-6 sm:mt-8">
    <div class="bg-white rounded-xl shadow-lg border border-gray-200 overflow-hidden">
      <!-- Header -->
      <div class="bg-gradient-to-r from-indigo-600 to-purple-600 px-5 py-4">
        <h3 class="text-white font-bold text-lg sm:text-xl flex items-center gap-2">
          <svg class="w-5 h-5" fill="none" stroke="currentColor" viewBox="0 0 24 24">
            <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M9 3v2m6-2v2M9 19v2m6-2v2M5 9H3m2 6H3m18-6h-2m2 6h-2M7 19h10a2 2 0 002-2V7a2 2 0 00-2-2H7a2 2 0 00-2 2v10a2 2 0 002 2zM9 9h6v6H9V9z"></path>
          </svg>
          Tareas por Robot
        </h3>
      </div>
      
      <!-- Content -->
      <div class="p-5 sm:p-6">
        <div class="grid grid-cols-1 sm:grid-cols-2 lg:grid-cols-3 gap-4 sm:gap-5">
          <div 
            v-for="robotId in robotsList" 
            :key="robotId"
            class="bg-gradient-to-br from-gray-50 to-blue-50 rounded-lg p-5 border-2 border-gray-200 hover:border-indigo-300 transition-all duration-300 hover:shadow-lg"
          >
            <div class="flex items-center gap-3 mb-4 pb-3 border-b-2 border-gray-300">
              <div class="w-10 h-10 rounded-lg bg-gradient-to-br from-indigo-600 to-purple-600 flex items-center justify-center text-white font-bold shadow-md">
                {{ robotId.replace('robot', 'R') }}
              </div>
              <h4 class="text-sm sm:text-base font-bold text-gray-800">
                {{ robotId.toUpperCase() }}
              </h4>
            </div>
            
            <div class="flex flex-col gap-2">
              <div 
                v-for="task in getRobotTasks(robotId)" 
                :key="task.id"
                :class="[
                  'px-3 py-2 rounded-lg transition-all duration-300 text-xs sm:text-sm font-medium text-center shadow-sm transform hover:scale-105',
                  task.status === 'completed' 
                    ? 'bg-gradient-to-r from-gray-500 to-gray-600 text-white' :
                  task.status === 'active' 
                    ? 'bg-gradient-to-r from-green-500 to-emerald-600 text-white shadow-md shadow-green-200 animate-pulse' :
                    'bg-white text-gray-800 border-2 border-gray-300 hover:border-blue-400'
                ]"
              >
                {{ task.name }}
              </div>
            </div>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<script setup lang="ts">

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
  robotsList: string[]
}>()

function getRobotTasks(robotId: string): Task[] {
  return props.allTasksList.filter(task => task.robotId === robotId)
}

</script>