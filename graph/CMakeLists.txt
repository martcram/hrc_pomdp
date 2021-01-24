file(GLOB SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/graph/*.cpp"
)

set(HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_library(
    graph
    ${SOURCES}
)

target_include_directories(graph PUBLIC "${HEADERS}")

#-----------------------------------------------------------------------------#

# Find Boost
find_package(Boost REQUIRED)
if (NOT Boost_FOUND)
    message(FATAL_ERROR "Boost not found.")
else ()
    target_include_directories(graph PUBLIC ${BOOST_INCLUDE_DIRS})
endif ()

#-----------------------------------------------------------------------------#