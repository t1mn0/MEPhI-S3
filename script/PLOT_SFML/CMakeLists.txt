cmake_minimum_required(VERSION 3.20)
project(PlotLib)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(PLOT SHARED
    area.cpp
    axis.cpp
    curve.cpp
    grid.cpp
    line.cpp
    plot.cpp
)