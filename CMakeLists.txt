cmake_minimum_required(VERSION 3.12...3.23)
project(yagi VERSION 0.1
        DESCRIPTION "WIP"
        LANGUAGES CXX C)

option(YAGI_SPDLOG_LEVEL "spdlog log level" SPDLOG_LEVEL_DEBUG)
option(YAGI_BUILD_EXAMPLES "Build example programs" ON)

if(MSVC)
    set(CMAKE_CXX_STANDARD 23)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
else()
    set(CMAKE_CXX_STANDARD 20)
endif()

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

include(include/CMakeLists.txt)
include(src/CMakeLists.txt)
include(external/CMakeLists.txt)
add_library(yagi STATIC
        ${YAGI_EXTERNAL_INCLUDE_FILES}
        ${YAGI_EXTERNAL_SRC_FILES}
        ${YAGI_INCLUDE_FILES}
        ${YAGI_SRC_FILES})

# Make ImGui use a different user config file so we don't have
# to touch the submodule files directly or make our own branch
target_compile_definitions(yagi PUBLIC IMGUI_USER_CONFIG="yagi/yagi_imconfig.h")

# spdlog log level
target_compile_definitions(yagi PRIVATE SPDLOG_ACTIVE_LEVEL=${YAGI_SPDLOG_LEVEL})

target_include_directories(yagi PUBLIC
        external/glad/include
        external/imgui/backends
        external/imgui
        external/pcg-cpp/include
        include)

target_link_libraries(yagi PUBLIC
        fmt::fmt
        glfw
        glm::glm
        libnyquist
        OpenAL::OpenAL
        spdlog::spdlog_header_only)

if(YAGI_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
