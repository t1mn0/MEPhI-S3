cmake_minimum_required(VERSION 3.20)
project(TransitNetworkVisualization)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Include Directories & Libs ---

set(INCLUDE_DIRS include include/Exceptions)

target_include_directories(Exceptions PUBLIC ../../include/Exceptions)

add_executable(cli main.cpp GraphView.cpp Help.cpp Commands.cpp)
target_link_libraries(cli Exceptions)

target_compile_definitions(cli PRIVATE EXECUTABLE_PATH="${CMAKE_CURRENT_SOURCE_DIR}")