cmake_minimum_required(VERSION 3.20)
project(PlantStarBrawl)

set(INC ${CMAKE_CURRENT_SOURCE_DIR}/inc)

include_directories(${INC})

file(GLOB SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

link_libraries(libeasyx.a)
add_executable(${PROJECT_NAME} ${SRC})

# target_link_libraries(${PROJECT_NAME} libeasyx.a)