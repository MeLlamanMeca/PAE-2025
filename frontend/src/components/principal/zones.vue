<template>
  <div class="w-full">
    <div v-if="zones" class="bg-white rounded-xl shadow-lg border border-gray-200 overflow-hidden">
      <!-- Header -->
      <div class="bg-gradient-to-r from-purple-600 to-pink-600 px-5 py-4">
        <h3 class="text-white font-bold text-lg sm:text-xl flex items-center gap-2">
          <svg class="w-5 h-5" fill="none" stroke="currentColor" viewBox="0 0 24 24">
            <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M7 21a4 4 0 01-4-4V5a2 2 0 012-2h4a2 2 0 012 2v12a4 4 0 01-4 4zm0 0h12a2 2 0 002-2v-4a2 2 0 00-2-2h-2.343M11 7.343l1.657-1.657a2 2 0 012.828 0l2.829 2.829a2 2 0 010 2.828l-8.486 8.485M7 17h.01"></path>
          </svg>
          Zonas del Mapa
        </h3>
      </div>
      
      <!-- Content -->
      <div class="p-4 sm:p-5 max-h-[600px] overflow-y-auto">
        <div class="space-y-2.5">
          <div 
            v-for="(name, id) in zones" 
            :key="id" 
            class="flex items-center gap-3 p-2.5 rounded-lg hover:bg-gray-50 transition-colors duration-200"
          >
            <div 
              class="w-8 h-8 rounded-lg border-2 border-gray-700 flex-shrink-0 shadow-sm" 
              :style="{ backgroundColor: getColor(id) }"
            ></div>
            <div class="flex-1 min-w-0">
              <div class="text-xs font-semibold text-gray-500">ID: {{ id }}</div>
              <div class="text-sm font-medium text-gray-800 truncate">{{ name }}</div>
            </div>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<script setup lang="ts">
const props = defineProps<{
  zones: Record<string, string> | null
  colorMap: Record<string, string>
}>()

function getColor(id: string): string {
  return props.colorMap[id] || '#' + Math.floor(Math.random()*16777215).toString(16)
}
</script>
