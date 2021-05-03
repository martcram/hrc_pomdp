cmake_minimum_required(VERSION 3.1.0)
project(hrc_pomdp VERSION 0.1.0)

set (CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
set (CMAKE_CXX_STANDARD 17)

add_subdirectory(graph)
add_subdirectory(main)
add_subdirectory(pomdp)
add_subdirectory(plot)
add_subdirectory(utils)

add_executable(
    hrc_pomdp
    main.cpp
)

target_link_libraries(hrc_pomdp PUBLIC graph main pomdp plot utils)