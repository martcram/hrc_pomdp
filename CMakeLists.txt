cmake_minimum_required(VERSION 3.1.0)
project(hrc_pomdp VERSION 0.1.0)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set (CMAKE_CXX_STANDARD 17)

add_subdirectory(graph)
add_subdirectory(main)
add_subdirectory(pomdp)
add_subdirectory(plot)
add_subdirectory(utils)

add_library(${PROJECT_NAME}::graph ALIAS graph)
add_library(${PROJECT_NAME}::main  ALIAS main)
add_library(${PROJECT_NAME}::pomdp ALIAS pomdp)
add_library(${PROJECT_NAME}::plot  ALIAS plot)
add_library(${PROJECT_NAME}::utils ALIAS utils)

# add_executable(
#     hrc_pomdp
#     main.cpp
# )

# target_link_libraries(hrc_pomdp PUBLIC graph main pomdp plot utils)