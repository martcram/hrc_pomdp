set(HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_library(math_utils INTERFACE)

target_include_directories(math_utils INTERFACE "${HEADERS}")