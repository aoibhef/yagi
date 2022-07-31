cmake_minimum_required(VERSION 3.12...3.23)
project(yagi VERSION 0.1
        DESCRIPTION "WIP"
        LANGUAGES CXX C)

if(MSVC)
    set(CMAKE_CXX_STANDARD 23)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
else()
    set(CMAKE_CXX_STANDARD 20)
endif()

add_library(yagi STATIC
        include/yagi/msg/bus.h

        src/yagi/msg/bus.cpp)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
add_subdirectory(external/fmt)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(external/glfw)

add_subdirectory(external/glm)

set(SPDLOG_BUILD_EXAMPLE OFF CACHE BOOL "" FORCE)
set(SPDLOG_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(SPDLOG_FMT_EXTERNAL ON CACHE BOOL "" FORCE)
add_subdirectory(external/spdlog)

target_include_directories(yagi PUBLIC
        include)

target_link_libraries(yagi PUBLIC
        fmt::fmt
        glfw
        glm::glm
        spdlog::spdlog)

add_subdirectory(examples)
