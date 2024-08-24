cmake_minimum_required(VERSION 3.20)
project(PlantStarBrawl)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(INC ${CMAKE_CURRENT_SOURCE_DIR}/inc)
set(ICON_DIR ${CMAKE_CURRENT_SOURCE_DIR}/assets/icon)
set(SCENE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/inc/scene)
set(PLAYER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/inc/player)
set(BULLET_DIR ${CMAKE_CURRENT_SOURCE_DIR}/inc/bullet)

set(MEDIA_PATH_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/assets/)

configure_file(
    ${INC}/MediaSource.h.in
    ${INC}/MediaSource.h
)

include_directories(${INC})
include_directories(${SCENE_DIR})
include_directories(${PLAYER_DIR})
include_directories(${BULLET_DIR})

file(GLOB_RECURSE SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_executable(${PROJECT_NAME} ${SRC} ${ICON_DIR}/icon.rc)
target_link_libraries(${PROJECT_NAME} Winmm easyx MSIMG32)
