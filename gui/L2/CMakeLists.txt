cmake_minimum_required(VERSION 3.20)
project(TransitNetworkVisualization)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Include Directories & Libs ---

set(INCLUDE_DIRS include include/Exceptions)

target_include_directories(Exceptions PUBLIC ../../include/Exceptions)

find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS})

add_executable(cli main.cpp VisualSorters.cpp SortView.cpp Transaction.cpp Generator.cpp Comparators.cpp)
target_link_libraries(cli Exceptions ${SDL2_LIBRARIES})

target_compile_definitions(cli PRIVATE EXECUTABLE_PATH="${CMAKE_CURRENT_SOURCE_DIR}")

