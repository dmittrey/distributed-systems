cmake_minimum_required(VERSION 3.16)
project(pa1-starter C)

file(GLOB_RECURSE H_FILES *.h)

add_library(${PROJECT_NAME} OBJECT ${H_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
