file(GLOB SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/pomdp/*.cpp"
)

set(HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_library(
    pomdp
    ${SOURCES}
)

target_include_directories(pomdp PUBLIC "${HEADERS}")
target_link_libraries(pomdp PUBLIC graph main utils)

#-----------------------------------------------------------------------------#

# Find TinyXML2
find_package(tinyxml2 CONFIG REQUIRED)
if (NOT tinyxml2_FOUND)
    message(FATAL_ERROR "TinyXML2 not found.")
else ()
    target_link_libraries(pomdp PRIVATE tinyxml2::tinyxml2)
endif ()

#-----------------------------------------------------------------------------#