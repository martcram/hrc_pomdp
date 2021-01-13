file(GLOB SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/math_utils/*.cpp"
)

set(HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_library(
    math_utils
    ${SOURCES}
)

target_include_directories(math_utils PUBLIC "${HEADERS}")