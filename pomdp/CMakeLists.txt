file(GLOB SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/pomdp/*.cpp"
)

set(HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_library(
    pomdp
    ${SOURCES}
)

target_include_directories(pomdp PUBLIC "${HEADERS}")
target_link_libraries(pomdp PUBLIC graph main utils tinyxml2)