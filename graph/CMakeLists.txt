file(GLOB SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/graph/*.cpp"
)

set(HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_library(
    graph
    ${SOURCES}
)

target_include_directories(graph PUBLIC "${HEADERS}")

find_package(Boost REQUIRED)
if(Boost_FOUND)
    target_include_directories(graph PUBLIC ${BOOST_INCLUDE_DIRS})
endif()