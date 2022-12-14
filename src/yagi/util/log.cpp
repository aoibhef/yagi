#include "yagi/util/log.h"

namespace yagi {

std::shared_ptr<spdlog::logger> logger() {
  static auto logger = std::invoke([] {
    auto console_sink = std::make_shared<spdlog::sinks::ansicolor_stdout_sink_mt>();
    console_sink->set_color_mode(spdlog::color_mode::always);

    auto msg_sink = std::make_shared<YagiImguiLogSink<spdlog::details::null_mutex>>();
    msg_sink->set_pattern("[%H:%M:%S %t %L] %v");

    auto l = std::make_shared<spdlog::logger>("", console_sink);
    l->sinks().emplace_back(msg_sink);
    l->sinks()[0]->set_level(spdlog::level::off);

    l->set_level(spdlog::level::trace);

    return l;
  });

  return logger;
}

} // namespace yagi