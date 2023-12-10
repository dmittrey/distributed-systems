cmake_minimum_required(VERSION 3.16)
project(dist-systems-1 C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -std=c99 -W -Wall -pedantic")

add_subdirectory(pa1-starter)
add_subdirectory(pa1-lib)

add_executable(pa1-app EXCLUDE_FROM_ALL pa1-lib/main.c)
target_link_libraries(pa1-app pa1-lib)

file(GLOB_RECURSE PA1_LIB_PACK_C_FILES pa1-lib/src/*.c)
file(GLOB_RECURSE PA1_LIB_PACK_H_FILES pa1-lib/include/*.h)

file(GLOB_RECURSE PA1_STARTER_PACK_H_FILES pa1-starter/*.h)

add_custom_target(pa1
    BYPRODUCTS ${CMAKE_BINARY_DIR}/pa1 ${CMAKE_SOURCE_DIR}/pa1.tar.gz
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/pa1
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${PA1_LIB_PACK_C_FILES} ${PA1_LIB_PACK_H_FILES} ${CMAKE_BINARY_DIR}/pa1
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${PA1_STARTER_PACK_H_FILES} ${CMAKE_BINARY_DIR}/pa1
    COMMAND ${CMAKE_COMMAND} -E tar czf ${CMAKE_SOURCE_DIR}/pa1.tar.gz ${CMAKE_BINARY_DIR}/pa1
    DEPENDS pa1-app
    COMMENT "Generate pa1 archive"
    VERBATIM
)