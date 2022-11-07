#ifndef YAGI_UTIL_LOG_H
#define YAGI_UTIL_LOG_H

#define SPDLOG_HEADER_ONLY
#if !defined(SPDLOG_ACTIVE_LEVEL)
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_INFO
#endif
#include "spdlog/spdlog.h"
#include "spdlog/sinks/ansicolor_sink.h"
#include "spdlog/sinks/base_sink.h"
#include "yagi/msg/bus.h"
#include <memory>

namespace yagi {

template<typename Mutex>
class YagiImguiLogSink : public spdlog::sinks::base_sink<Mutex> {
protected:
  void sink_it_(const spdlog::details::log_msg& msg) override {
    spdlog::memory_buf_t formatted;
    spdlog::sinks::base_sink<Mutex>::formatter_->format(msg, formatted);
    yagi::Bus::send_nowait<yagi::MsgType::ImguiLog>(fmt::to_string(formatted), msg.level);
  }

  void flush_() override { /* do nothing */ }
};

std::shared_ptr<spdlog::logger> logger();

} // namespace yagi

#define YAGI_LOG_TRACE(...)    SPDLOG_LOGGER_TRACE(yagi::logger(), __VA_ARGS__)
#define YAGI_LOG_DEBUG(...)    SPDLOG_LOGGER_DEBUG(yagi::logger(), __VA_ARGS__)
#define YAGI_LOG_INFO(...)     SPDLOG_LOGGER_INFO(yagi::logger(), __VA_ARGS__)
#define YAGI_LOG_WARN(...)     SPDLOG_LOGGER_WARN(yagi::logger(), __VA_ARGS__)
#define YAGI_LOG_ERROR(...)    SPDLOG_LOGGER_ERROR(yagi::logger(), __VA_ARGS__)
#define YAGI_LOG_CRITICAL(...) SPDLOG_LOGGER_CRITICAL(yagi::logger(), __VA_ARGS__)

#endif //YAGI_UTIL_LOG_H
