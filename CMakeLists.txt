cmake_minimum_required(VERSION 3.1.0)
project(hrc_pomdp VERSION 0.1.0)

set (CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

add_subdirectory(graph)
add_subdirectory(main)
add_subdirectory(math_utils)

add_executable(
    hrc_pomdp
    main.cpp
)

target_link_libraries(hrc_pomdp PUBLIC graph main math_utils)