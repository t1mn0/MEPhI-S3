cmake_minimum_required(VERSION 3.20)

project(mephi-3 VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



add_compile_options(
    -Werror

    -Wall
    -Wextra
)



add_executable(project 
                        main.cpp 
                        #src/Optional/Optional.cpp 
                        #src/SmartPtr/SharedPtr.cpp 
                        src/SmartPtr/UniquePtr.cpp 
                        src/tmn.cpp) 
