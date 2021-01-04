cmake_minimum_required(VERSION 3.1.0)

project(hrc_pomdp VERSION 0.1.0)

file(GLOB SOURCES
    "src/*.cpp"
)

include_directories(include)

add_executable(
    hrc_pomdp
    main.cpp

    ${SOURCES}
)
