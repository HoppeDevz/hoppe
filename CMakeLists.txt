cmake_minimum_required(VERSION 3.10)

project(hoppe VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(hoppe 
    src/main.cpp
    src/http_request_buffer_parser.cpp
    src/tcp_server.cpp
)

target_include_directories(hoppe PUBLIC ${CMAKE_SOURCE_DIR}/src/include)
