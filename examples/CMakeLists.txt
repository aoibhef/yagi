add_executable(indev indev.h indev.cpp)
target_compile_definitions(indev PUBLIC SPDLOG_ACTIVE_LEVEL=${YAGI_SPDLOG_LEVEL})
target_link_libraries(indev PUBLIC yagi)
