file(GLOB SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/main/*.cpp"
)

set(HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_library(
    main
    ${SOURCES}
)

include(FetchContent)
# ======================= nlohmann/json =======================
FetchContent_Declare(json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.7.3)

FetchContent_GetProperties(json)
if(NOT json_POPULATED)
  FetchContent_Populate(json)
  add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()
# =============================================================

target_include_directories(main PUBLIC "${HEADERS}")
target_link_libraries(main PUBLIC graph utils nlohmann_json::nlohmann_json)