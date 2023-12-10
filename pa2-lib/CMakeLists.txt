cmake_minimum_required(VERSION 3.16)
project(pa2-lib C)

file(GLOB_RECURSE C_FILES src/*.c)
file(GLOB_RECURSE H_FILES include/*.h)

add_library(${PROJECT_NAME} OBJECT ${C_FILES} ${H_FILES})
target_link_libraries(${PROJECT_NAME} pa2345-starter)
target_include_directories(${PROJECT_NAME} PUBLIC include)