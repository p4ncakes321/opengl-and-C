cmake_minimum_required(VERSION 3.10)
project(game C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/built)

file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.c)
add_executable(game ${SOURCES})

target_include_directories(game PRIVATE ${PROJECT_SOURCE_DIR}/include)

find_package(glfw3 3.3 REQUIRED)
target_link_libraries(game PRIVATE glfw m)

find_path(CGLM_INCLUDE_DIR cglm/cglm.h)
if(CGLM_INCLUDE_DIR)
    target_include_directories(game PRIVATE ${CGLM_INCLUDE_DIR})
endif()

