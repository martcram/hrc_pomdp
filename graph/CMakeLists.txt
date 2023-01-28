set(HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_library(graph INTERFACE)

target_include_directories(graph INTERFACE "${HEADERS}")
target_link_libraries(graph INTERFACE plot utils)