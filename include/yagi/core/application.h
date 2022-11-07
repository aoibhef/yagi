#ifndef YAGI_APP_APPLICATION_H
#define YAGI_APP_APPLICATION_H

#include "yagi/core/inputmgr.h"
#include "yagi/core/window.h"
#include "yagi/msg/bus.h"
#include "yagi/util/framecounter.h"
#include <memory>

namespace yagi {

class ImguiLogWindow {
public:
  bool docked{true};

  ImguiLogWindow();

  void clear();

  void add(spdlog::level::level_enum level, const char* fmt, ...) IM_FMTARGS(2);

  void draw(const char* title, bool *p_open = nullptr);

private:
  ImGuiTextBuffer buf_;
  ImGuiTextFilter filter_;
  ImVector<int> line_offsets_;
  ImVector<spdlog::level::level_enum> line_levels_;

  bool autoscroll_{true};
  bool wrapping_{true};

  static std::unordered_map<spdlog::level::level_enum, ImU32> color_map_;
};

class Application {
  friend class Oracle;

public:
  std::unique_ptr<Window> window{nullptr};
  std::unique_ptr<Context> ctx{nullptr};
  std::unique_ptr<InputMgr> input{nullptr};

  Application();

  virtual void initialize();
  virtual void deinitialize();
  virtual void update(double dt);
  virtual void draw();

  void app_shutdown();
  void app_debug_overlay_enabled(bool enabled);

private:
  std::string msg_endpoint_id_{};
  void received_msg_(const Msg &msg);

  FrameCounter framecounter_{};

  struct {
    bool enabled{false};

    ImguiLogWindow log{};
  } overlay{};

  void update_();

  void start_frame_();
  void draw_();
  void draw_debug_overlay_();
  void end_frame_();

  void post_draw_();

  void imgui_start_frame_();
  void imgui_end_frame_();
};

} // namespace yagi

#endif //YAGI_APP_APPLICATION_H
