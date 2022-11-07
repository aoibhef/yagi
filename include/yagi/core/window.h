#ifndef YAGI_APP_WINDOW_H
#define YAGI_APP_WINDOW_H

#include "yagi/gfx/context.h"
#include "yagi/msg/bus.h"
#include "yagi/util/enum_bitmask_ops.h"
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include <string>

namespace yagi {

enum class WindowFlags {
  none        = 1 << 0,
  vsync       = 1 << 1,
  resizable   = 1 << 2,
  hidden      = 1 << 3,
  centered    = 1 << 4,
  fullscreen  = 1 << 5,
  borderless  = 1 << 6,
  undecorated = 1 << 7,
};

struct WindowOpenParams {
  std::string title{"Yagi Window"};

  glm::ivec2 size{0, 0};
  glm::ivec2 pos{0, 0};

  int monitor_num{0};

  WindowFlags flags{WindowFlags::none};

  glm::ivec2 gl_version{3, 3};
};

class Window {
public:
  // Attributes about the window as member variables rather than functions
  // To change these, use the corresponding set_* call
  int x{0}, y{0};
  int w{0}, h{0};
  bool decorated{true};
  bool borderless{false};
  bool resizable{false};
  bool vsync{false};

  GLFWwindow *glfw_handle{nullptr};

  Window() = default;

  void open(const WindowOpenParams &params);
  std::unique_ptr<Context> create_ctx();

  void set_x(int xpos);
  void set_y(int ypos);

  void set_w(int width);
  void set_h(int height);

  void set_decorated(bool is_decorated);
  void set_resizable(bool is_resizable);
  void set_vsync(bool is_enabled);

  bool should_close() const;
  void set_should_close(bool should);

  void swap() const;

  void poll_msgs();

private:
  std::string msg_endpoint_id_{};
  void received_msg_(const Msg &msg);

  struct {
    glm::ivec2 gl_version{};
  } wm_info_;

  GLFWmonitor *get_monitor_(int monitor_num);
  void open_fullscreen_(const WindowOpenParams &params);
  void open_windowed_(const WindowOpenParams &params);
};

} // namespace yagi

ENABLE_BITMASK_OPERATORS(yagi::WindowFlags)

#endif //YAGI_APP_WINDOW_H
