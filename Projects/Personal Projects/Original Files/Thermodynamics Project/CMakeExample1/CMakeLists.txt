cmake_minimum_required(VERSION 3.24)
project(hello_world)

set(SOURCE_FILES
    main.cpp
    hello.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})