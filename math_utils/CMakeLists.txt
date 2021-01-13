add_library(
    math_utils
    "src/math_utils/math_utils.cpp"
    "include/math_utils/math_utils.hpp"
)

target_include_directories(
    math_utils PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)