cmake_minimum_required(VERSION 3.16)
project(pa2345-starter C)

file(GLOB_RECURSE H_FILES *.h)
file(GLOB_RECURSE C_FILES *.c)

add_library(${PROJECT_NAME} EXCLUDE_FROM_ALL OBJECT ${H_FILES} ${C_FILES})
target_link_libraries(${PROJECT_NAME} pa1-starter)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_library(runtime SHARED IMPORTED)
set_target_properties(runtime PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/lib64/libruntime.so)