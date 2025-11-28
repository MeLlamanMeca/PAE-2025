BUILD_DIR = build

#ejecutar make all para compilar
all: build
	cmake --build $(BUILD_DIR)

build:
	mkdir -p $(BUILD_DIR)
	cmake -S . -B $(BUILD_DIR) -G "MinGW Makefiles"