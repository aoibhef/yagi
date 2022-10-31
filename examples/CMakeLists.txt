add_executable(indev indev.cpp)
target_compile_definitions(indev PUBLIC SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_DEBUG)
target_link_libraries(indev PUBLIC yagi)
