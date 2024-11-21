cmake_minimum_required(VERSION 3.20)
project(SEMESTER-3 VERSION 1.2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Include Directories & Libs ---
set(INCLUDE_DIRS include include/VFS include/Exceptions)

add_subdirectory(include/Exceptions)
add_subdirectory(script/PLOT_SFML)	
add_subdirectory(include/VFS)	

target_include_directories(Exceptions INTERFACE ${INCLUDE_DIRS})
target_include_directories(PLOT INTERFACE ${INCLUDE_DIRS})
target_include_directories(VFS INTERFACE ${INCLUDE_DIRS})

# --- Graphs (SFML) ---
set(SFML_STATIC_LIBRARIES FALSE)
find_package(SFML COMPONENTS window graphics system REQUIRED)
if(NOT SFML_FOUND)
    message(FATAL_ERROR "SFML not found. Please install the required dependencies for this project to work")
    return()
endif()

add_executable(graphs script/Graph.cpp script/L3.cpp)
target_link_libraries(graphs PLOT sfml-window sfml-system sfml-graphics)

# --- Tests (GTest) ---
find_package(GTest REQUIRED)
if(NOT GTEST_FOUND)
    message(FATAL_ERROR "GTest not found. Please install the required dependencies for this project to work")
    return()
else()
    option(BUILD_TESTS "Build tests" ON)

    if(BUILD_TESTS)
        enable_testing()
        add_subdirectory(test/Hash)
        add_subdirectory(test/VFS)
    endif()
endif()