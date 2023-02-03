cmake_minimum_required(VERSION 3.10)
project(WasmEdge-tool VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED False)

set(SOURCES
    src/tool.c
)

add_executable(tool ${SOURCES})
target_link_libraries(tool PRIVATE wasmedge)
