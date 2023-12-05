cmake_minimum_required(VERSION 3.16)
project(dist-systems-1 C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -std=c99 -W -Wall -pedantic")

add_subdirectory(pa1-starter)
add_subdirectory(pa1-lib)

add_executable(pa1 pa1-lib/main.c)
target_link_libraries(pa1 pa1-lib)

add_custom_target(pack 
    COMMAND
        ${CMAKE_COMMAND} -E echo Hi this is a custom target
    VERBATIM
)