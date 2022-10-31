#ifndef YAGI_APP_WINDOW_H
#define YAGI_APP_WINDOW_H

#include "yagi/msg/bus.h"
#include "yagi/util/enum_bitmask_ops.h"
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include <string>

namespace yagi {

enum class WindowFlags {
  none       = 1 << 0,
  vsync      = 1 << 1,
  resizable  = 1 << 2,
  hidden     = 1 << 3,
  centered   = 1 << 4,
  fullscreen = 1 << 5,
  borderless = 1 << 6
};

struct WindowOpenParams {
  std::string title{"Yagi Window"};

  glm::ivec2 size{0, 0};
  glm::ivec2 pos{0, 0};

  int monitor_num{0};

  WindowFlags flags{WindowFlags::none};

  glm::ivec2 glversion{3, 3};
};

class Window {
public:
  GLFWwindow *glfw_handle{nullptr};

  Window() = default;

  void open(const WindowOpenParams &params);

  bool should_close() const;

  void poll_msgs();

private:
  std::string msg_endpoint_id_{};
  void received_msg_(const msg::Msg &msg);

  struct {
    bool borderless{false};
    bool vsync{false};
  } wm_info_;

  GLFWmonitor *get_monitor_(int monitor_num);
  void open_fullscreen_(const WindowOpenParams &params);
  void open_windowed_(const WindowOpenParams &params);
};

} // namespace yagi

ENABLE_BITMASK_OPERATORS(yagi::WindowFlags)

#endif //YAGI_APP_WINDOW_H
