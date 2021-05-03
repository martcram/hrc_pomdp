file(GLOB SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/plot/*.cpp"
)

set(HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_library(
    plot
    ${SOURCES}
)

target_include_directories(plot PUBLIC "${HEADERS}")
target_link_libraries(plot PUBLIC utils)

#-----------------------------------------------------------------------------#

# Find GraphViz
find_package(Graphviz REQUIRED)
if (NOT GRAPHVIZ_FOUND)
    message(FATAL_ERROR "Graphviz not found.")
else ()
    target_include_directories(plot PUBLIC ${GRAPHVIZ_INCLUDE_DIRS})
    target_link_libraries(plot PUBLIC  ${GRAPHVIZ_CDT_LIBRARY} 
                                       ${GRAPHVIZ_GVC_LIBRARY} 
                                       ${GRAPHVIZ_CGRAPH_LIBRARY} 
                                       ${GRAPHVIZ_PATHPLAN_LIBRARY}
    )
endif ()

#-----------------------------------------------------------------------------#