file(GLOB SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/AndOrGraph/*.cpp"
)

file(GLOB HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/AndOrGraph/*.cpp"
)

add_library(
    AndOrGraph
    ${SOURCES}
    ${HEADERS}
)

target_include_directories(AndOrGraph PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")