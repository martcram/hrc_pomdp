file(GLOB SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/utils/*.cpp"
)

set(HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_library(
    utils
    ${SOURCES}
)

target_include_directories(utils PUBLIC "${HEADERS}")