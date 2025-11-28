cmake_minimum_required(VERSION 3.10)
project(mi_websocket_cpp)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_subdirectory(third_party/IXWebSocket)

add_library(nlohmann_json INTERFACE)
target_include_directories(nlohmann_json INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/third_party/nlohmann)

add_executable(server 
src/main.cpp 
src/WebSocket/MyWebSocket.cpp
src/app/App.cpp
src/app/AppAdapter.cpp
src/services/impl/MapServiceImpl.cpp
src/services/impl/RobotServiceImpl.cpp
src/services/impl/TaskServiceImpl.cpp
src/common/Map.cpp
)

target_link_libraries(server PRIVATE ixwebsocket::ixwebsocket nlohmann_json)
