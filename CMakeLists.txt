cmake_minimum_required(VERSION 3.1.0)
project(hrc_pomdp VERSION 0.1.0)

add_subdirectory(AndOrGraph)
add_subdirectory(graph)
add_subdirectory(math_utils)

add_executable(
    hrc_pomdp
    main.cpp
)

target_link_libraries(hrc_pomdp PUBLIC AndOrGraph graph math_utils)