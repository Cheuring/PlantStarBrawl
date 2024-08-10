cmake_minimum_required(VERSION 3.20)
project(PlantStarBrawl)


set(INC ${CMAKE_CURRENT_SOURCE_DIR}/inc)
set(SCENE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/inc/scene)

include_directories(${INC})
include_directories(${SCENE_DIR})

file(GLOB SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

link_libraries(libeasyx.a)
add_executable(${PROJECT_NAME} ${SRC})
