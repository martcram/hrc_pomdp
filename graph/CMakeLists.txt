set(HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_library(graph INTERFACE)

target_include_directories(graph INTERFACE "${HEADERS}")
target_link_libraries(graph INTERFACE plot utils)

#-----------------------------------------------------------------------------#

# Find Boost
find_package(Boost REQUIRED)
if (NOT Boost_FOUND)
    message(FATAL_ERROR "Boost not found.")
else ()
    target_include_directories(graph PUBLIC ${BOOST_INCLUDE_DIRS})
endif ()

#-----------------------------------------------------------------------------#