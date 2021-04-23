file(GLOB SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/main/*.cpp"
)

set(HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_library(
    main
    ${SOURCES}
)

target_include_directories(main PUBLIC "${HEADERS}")
target_link_libraries(main PUBLIC graph utils)