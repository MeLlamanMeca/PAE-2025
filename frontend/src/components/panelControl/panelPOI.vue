<template>
  <div>
    <div class="flex justify-between items-center mb-5">
      <h3 class="text-xl sm:text-2xl font-bold text-gray-800 flex items-center gap-2">
        <svg class="w-6 h-6 text-purple-600" fill="none" stroke="currentColor" viewBox="0 0 24 24">
          <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M17.657 16.657L13.414 20.9a1.998 1.998 0 01-2.827 0l-4.244-4.243a8 8 0 1111.314 0z"></path>
          <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M15 11a3 3 0 11-6 0 3 3 0 016 0z"></path>
        </svg>
        Lista de POI
      </h3>
      <button
        @click="openCreatePOIModal"
        class="inline-flex items-center gap-2 px-4 py-2.5 bg-gradient-to-r from-purple-600 to-pink-600 hover:from-purple-700 hover:to-pink-700 text-white font-semibold rounded-lg shadow-lg hover:shadow-xl transition-all duration-300 text-xs sm:text-sm transform hover:scale-105"
      >
        <svg class="w-4 h-4" fill="none" stroke="currentColor" viewBox="0 0 24 24">
          <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M12 4v16m8-8H4"></path>
        </svg>
        Añadir POI
      </button>
    </div>

    <!-- Tabla de POIs -->
    <div class="overflow-x-auto rounded-xl border border-gray-200 shadow-md">
      <table class="w-full border-collapse bg-white">
        <thead>
          <tr class="bg-gradient-to-r from-gray-50 to-gray-100 border-b-2 border-gray-200">
            <th class="px-4 py-3 text-left text-xs sm:text-sm font-bold text-gray-700 uppercase tracking-wider">ID</th>
            <th class="px-4 py-3 text-left text-xs sm:text-sm font-bold text-gray-700 uppercase tracking-wider">Nombre</th>
            <th class="px-4 py-3 text-left text-xs sm:text-sm font-bold text-gray-700 uppercase tracking-wider">Ubicación</th>
            <th class="px-4 py-3 text-left text-xs sm:text-sm font-bold text-gray-700 uppercase tracking-wider">Color</th>
            <th class="px-4 py-3 text-center text-xs sm:text-sm font-bold text-gray-700 uppercase tracking-wider">Acciones</th>
          </tr>
        </thead>
        <tbody>
          <tr v-if="poiList.length === 0">
            <td colspan="5" class="px-4 py-12 text-center">
              <div class="flex flex-col items-center justify-center">
                <svg class="w-16 h-16 text-gray-300 mb-3" fill="none" stroke="currentColor" viewBox="0 0 24 24">
                  <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M17.657 16.657L13.414 20.9a1.998 1.998 0 01-2.827 0l-4.244-4.243a8 8 0 1111.314 0z"></path>
                  <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M15 11a3 3 0 11-6 0 3 3 0 016 0z"></path>
                </svg>
                <p class="text-gray-500 text-sm font-medium">No hay POIs registrados</p>
              </div>
            </td>
          </tr>
          <tr 
            v-for="poi in poiList" 
            :key="poi.id"
            class="border-b border-gray-100 hover:bg-purple-50 transition-all duration-200"
          >
            <td class="px-4 py-3 text-xs sm:text-sm font-bold text-purple-600">{{ poi.id }}</td>
            <td class="px-4 py-3 text-xs sm:text-sm font-medium text-gray-900">{{ poi.name }}</td>
            <td class="px-4 py-3 text-xs sm:text-sm text-gray-600 font-mono">
              {{ poi.location ? `(${poi.location.row}, ${poi.location.col})` : '-' }}
            </td>
            <td class="px-4 py-3">
              <div class="flex items-center gap-2.5">
                <span 
                  class="w-8 h-8 rounded-lg border-2 border-gray-800 inline-block shadow-sm"
                  :style="{ backgroundColor: poi.color }"
                ></span>
                <span class="text-xs font-mono text-gray-600">{{ poi.color }}</span>
              </div>
            </td>
            <td class="px-4 py-3 text-center">
              <div class="flex items-center justify-center gap-2">
                <button
                  @click="openEditPOIModal(poi)"
                  class="inline-flex items-center px-3 py-1.5 bg-yellow-500 hover:bg-yellow-600 text-white rounded-lg text-xs font-semibold transition-all duration-200 shadow hover:shadow-md transform hover:scale-105"
                >
                  <svg class="w-3 h-3 mr-1" fill="none" stroke="currentColor" viewBox="0 0 24 24">
                    <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M11 5H6a2 2 0 00-2 2v11a2 2 0 002 2h11a2 2 0 002-2v-5m-1.414-9.414a2 2 0 112.828 2.828L11.828 15H9v-2.828l8.586-8.586z"></path>
                  </svg>
                  Editar
                </button>
                <button
                  @click="deletePOI(poi.id)"
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

    <!-- Modal para crear/editar POI -->
    <div 
      v-if="showPOIModal"
      class="fixed inset-0 bg-black/60 backdrop-blur-sm flex items-center justify-center z-[60] p-4 animate-fadeIn"
      @click.self="closePOIModal"
    >
      <div class="bg-white rounded-2xl shadow-2xl max-w-lg w-full p-6 sm:p-8 relative transform transition-all animate-slideUp">
        <!-- Botón cerrar -->
        <button
          @click="closePOIModal"
          class="absolute top-4 right-4 text-gray-400 hover:text-gray-600 hover:bg-gray-100 rounded-full p-2 transition-all duration-200"
        >
          <svg class="w-6 h-6" fill="none" stroke="currentColor" viewBox="0 0 24 24">
            <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M6 18L18 6M6 6l12 12"></path>
          </svg>
        </button>
        
        <!-- Título del modal -->
        <div class="mb-6">
          <h3 class="text-xl sm:text-2xl font-bold text-gray-800 flex items-center gap-2">
            <svg class="w-6 h-6 text-purple-600" fill="none" stroke="currentColor" viewBox="0 0 24 24">
              <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M17.657 16.657L13.414 20.9a1.998 1.998 0 01-2.827 0l-4.244-4.243a8 8 0 1111.314 0z"></path>
              <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M15 11a3 3 0 11-6 0 3 3 0 016 0z"></path>
            </svg>
            {{ editingPOI ? 'Editar POI' : 'Añadir Nuevo POI' }}
          </h3>
        </div>

        <!-- Formulario -->
        <form @submit.prevent="savePOI" class="space-y-5">
          <!-- ID -->
          <div>
            <label class="block text-sm font-bold text-gray-700 mb-2">ID *</label>
            <input
              v-model="poiForm.id"
              type="text"
              required
              :disabled="!!editingPOI"
              :class="[
                'w-full px-4 py-2.5 border-2 rounded-lg focus:outline-none focus:ring-2 focus:ring-purple-500 focus:border-transparent text-sm transition-all duration-200',
                editingPOI ? 'bg-gray-100 border-gray-300 cursor-not-allowed' : 'border-gray-200'
              ]"
              :placeholder="editingPOI ? editingPOI.id : 'ID del POI (ej: 11)'"
            />
          </div>

          <!-- Nombre -->
          <div>
            <label class="block text-sm font-bold text-gray-700 mb-2">Nombre *</label>
            <input
              v-model="poiForm.name"
              type="text"
              required
              class="w-full px-4 py-2.5 border-2 border-gray-200 rounded-lg focus:outline-none focus:ring-2 focus:ring-purple-500 focus:border-transparent text-sm transition-all duration-200"
              :placeholder="editingPOI ? editingPOI.name : 'Nombre del POI'"
            />
          </div>

          <!-- Ubicación -->
          <div>
            <label class="block text-sm font-bold text-gray-700 mb-2">Ubicación (Coordenadas) *</label>
            <div class="grid grid-cols-2 gap-3">
              <div>
                <label class="block text-xs font-semibold text-gray-600 mb-1.5">Fila</label>
                <input
                  v-model.number="poiForm.location.row"
                  type="number"
                  required
                  min="0"
                  class="w-full px-4 py-2.5 border-2 border-gray-200 rounded-lg focus:outline-none focus:ring-2 focus:ring-purple-500 focus:border-transparent text-sm transition-all duration-200"
                  :placeholder="editingPOI ? String(editingPOI.location?.row) : '0'"
                />
              </div>
              <div>
                <label class="block text-xs font-semibold text-gray-600 mb-1.5">Columna</label>
                <input
                  v-model.number="poiForm.location.col"
                  type="number"
                  required
                  min="0"
                  class="w-full px-4 py-2.5 border-2 border-gray-200 rounded-lg focus:outline-none focus:ring-2 focus:ring-purple-500 focus:border-transparent text-sm transition-all duration-200"
                  :placeholder="editingPOI ? String(editingPOI.location?.col) : '0'"
                />
              </div>
            </div>
            <div class="mt-2 flex items-center gap-2 text-xs text-gray-500">
              <svg class="w-4 h-4" fill="none" stroke="currentColor" viewBox="0 0 24 24">
                <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M13 16h-1v-4h-1m1-4h.01M21 12a9 9 0 11-18 0 9 9 0 0118 0z"></path>
              </svg>
              <span>Tip: Click en el mapa para seleccionar coordenadas</span>
            </div>
          </div>

          <!-- Color -->
          <div>
            <label class="block text-sm font-bold text-gray-700 mb-2">Color *</label>
            <div class="flex items-center gap-3">
              <input
                v-model="poiForm.color"
                type="color"
                required
                class="w-16 h-12 border-2 border-gray-300 rounded-lg cursor-pointer shadow-sm"
              />
              <input
                v-model="poiForm.color"
                type="text"
                required
                pattern="^#[0-9A-Fa-f]{6}$"
                class="flex-1 px-4 py-2.5 border-2 border-gray-200 rounded-lg focus:outline-none focus:ring-2 focus:ring-purple-500 focus:border-transparent text-sm font-mono transition-all duration-200"
                placeholder="#00CED1"
              />
            </div>
            <div class="mt-2 flex items-center gap-2 text-xs text-gray-500">
              <svg class="w-4 h-4" fill="none" stroke="currentColor" viewBox="0 0 24 24">
                <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M13 16h-1v-4h-1m1-4h.01M21 12a9 9 0 11-18 0 9 9 0 0118 0z"></path>
              </svg>
              <span>Color en formato hexadecimal (ej: #00CED1)</span>
            </div>
          </div>

          <!-- Botones -->
          <div class="flex justify-end gap-3 mt-8 pt-4 border-t border-gray-200">
            <button
              type="button"
              @click="closePOIModal"
              class="px-5 py-2.5 bg-gray-200 hover:bg-gray-300 text-gray-800 font-semibold rounded-lg transition-all duration-200 text-sm"
            >
              Cancelar
            </button>
            <button
              type="submit"
              class="px-5 py-2.5 bg-gradient-to-r from-purple-600 to-pink-600 hover:from-purple-700 hover:to-pink-700 text-white font-semibold rounded-lg shadow-lg hover:shadow-xl transition-all duration-300 text-sm transform hover:scale-105"
            >
              {{ editingPOI ? 'Guardar Cambios' : 'Crear POI' }}
            </button>
          </div>
        </form>
      </div>
    </div>
  </div>
</template>

<script setup lang="ts">
import { ref } from 'vue'

interface POI {
  id: string
  name: string
  location: {
    row: number
    col: number
  }
  color: string
}

// Estado del modal
const showPOIModal = ref(false)
const editingPOI = ref<POI | null>(null)

// Formulario de POI
const poiForm = ref({
  id: '',
  name: '',
  location: {
    row: 0,
    col: 0
  },
  color: '#00CED1'
})

// Lista de POIs (debería venir de props o del servidor)
const poiList = ref<POI[]>([
  { id: '0', name: 'POI 0 - Turquesa', location: { row: 5, col: 5 }, color: '#00CED1' },
  { id: '1', name: 'POI 1 - Tomate', location: { row: 10, col: 10 }, color: '#FF6347' },
  { id: '2', name: 'POI 2 - Azul real', location: { row: 15, col: 15 }, color: '#4169E1' },
])

// Abrir modal para crear POI
function openCreatePOIModal() {
  editingPOI.value = null
  poiForm.value = {
    id: '',
    name: '',
    location: {
      row: 0,
      col: 0
    },
    color: '#00CED1'
  }
  showPOIModal.value = true
}

// Abrir modal para editar POI
function openEditPOIModal(poi: POI) {
  editingPOI.value = poi
  poiForm.value = {
    id: poi.id,
    name: poi.name,
    location: { ...poi.location },
    color: poi.color
  }
  showPOIModal.value = true
}

// Cerrar modal
function closePOIModal() {
  showPOIModal.value = false
  editingPOI.value = null
  poiForm.value = {
    id: '',
    name: '',
    location: {
      row: 0,
      col: 0
    },
    color: '#00CED1'
  }
}

// Guardar POI (crear o editar)
function savePOI() {
  if (editingPOI.value) {
    // Editar POI existente
    console.log('Editando POI:', editingPOI.value.id, poiForm.value)
    // TODO: Enviar al servidor para actualizar
  } else {
    // Crear nuevo POI
    console.log('Creando nuevo POI:', poiForm.value)
    // TODO: Enviar al servidor para crear
  }
  closePOIModal()
}

// Eliminar POI
function deletePOI(poiId: string) {
  if (confirm('¿Estás seguro de que quieres eliminar este POI?')) {
    console.log('Eliminando POI:', poiId)
    // TODO: Enviar al servidor para eliminar
  }
}
</script>