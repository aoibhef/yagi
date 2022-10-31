#pragma once

#define SPDLOG_HEADER_ONLY
#if !defined(SPDLOG_ACTIVE_LEVEL)
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_INFO
#endif
#include "spdlog/spdlog.h"
#include "spdlog/sinks/ansicolor_sink.h"
#include <memory>

namespace yagi {

std::shared_ptr<spdlog::logger> logger();

} // namespace yagi

#define LOG_TRACE(...)    SPDLOG_LOGGER_TRACE(yagi::logger(), __VA_ARGS__)
#define LOG_DEBUG(...)    SPDLOG_LOGGER_DEBUG(yagi::logger(), __VA_ARGS__)
#define LOG_INFO(...)     SPDLOG_LOGGER_INFO(yagi::logger(), __VA_ARGS__)
#define LOG_ERROR(...)    SPDLOG_LOGGER_ERROR(yagi::logger(), __VA_ARGS__)
#define LOG_CRITICAL(...) SPDLOG_LOGGER_CRITICAL(yagi::logger(), __VA_ARGS__)
