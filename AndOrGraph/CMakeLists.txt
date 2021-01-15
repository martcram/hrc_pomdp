file(GLOB SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/AndOrGraph/*.cpp"
)

set(HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_library(
    AndOrGraph
    ${SOURCES}
)

target_include_directories(AndOrGraph PUBLIC "${HEADERS}")
target_link_libraries(AndOrGraph PUBLIC math_utils)