#include "yagi/core/window.h"

#include "yagi/core/glfw_callbacks.h"
#include "yagi/util/log.h"
#include "yagi/util/overloaded.h"
#include "yagi/util/platform.h"

namespace yagi {

void Window::open(const WindowOpenParams &params) {
  msg_endpoint_id_ = msg::Bus::register_endpoint([&](const msg::Msg &msg) { received_msg_(msg); });
  msg::Bus::subscribe(msg_endpoint_id_, msg::MsgType::WindowPos);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, params.gl_version.x);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, params.gl_version.y);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  wm_info_.gl_version = params.gl_version;

  if (set(params.flags, WindowFlags::fullscreen) || set(params.flags, WindowFlags::borderless))
    open_fullscreen_(params);
  else
    open_windowed_(params);

  internal::register_glfw_callbacks(glfw_handle);

  glfwMakeContextCurrent(glfw_handle);

  if (!set(params.flags, WindowFlags::fullscreen) &&
      !set(params.flags, WindowFlags::borderless) &&
      !set(params.flags, WindowFlags::hidden))
    glfwShowWindow(glfw_handle);
}

std::unique_ptr<Context> Window::create_ctx() {
  return std::make_unique<Context>(wm_info_.gl_version);
}

bool Window::should_close() const {
  return glfwWindowShouldClose(glfw_handle);
}

void Window::swap() const {
  glfwSwapBuffers(glfw_handle);
}

void Window::poll_msgs() {
  msg::Bus::poll(msg_endpoint_id_);
}

void Window::received_msg_(const msg::Msg &msg) {
  std::visit(overloaded {
      [&](const msg::WindowPos &m) {},
      [&](const auto &m) { YAGI_LOG_WARN("Unhandled event {}", msg.type); }
  }, msg.data);
}

GLFWmonitor *Window::get_monitor_(int monitor_num) {
  int monitor_count = 0;
  auto monitors = glfwGetMonitors(&monitor_count);

  if (monitor_num >= monitor_count) {
    YAGI_LOG_WARN("Monitor {} out of range (only {} monitors available); defaulting to primary", monitor_num, monitor_count);
    return monitors[0];
  }
  return monitors[monitor_num];
}

void Window::open_fullscreen_(const WindowOpenParams &params) {
#if defined(YAGI_PLATFORM_WINDOWS)
  GLFWmonitor *monitor = get_monitor_(params.monitor_num);
  const GLFWvidmode *mode = glfwGetVideoMode(monitor);

  glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);  // Why is this necessary?
  glfwWindowHint(GLFW_RED_BITS, mode->redBits);
  glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
  glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
  glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

  if (set(params.flags, WindowFlags::borderless)) {
    wm_info_.borderless = true;

    glfwWindowHint(GLFW_AUTO_ICONIFY, GLFW_FALSE);
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);

    glfw_handle = glfwCreateWindow(mode->width, mode->height, params.title.c_str(), nullptr, nullptr);
  } else
    glfw_handle = glfwCreateWindow(mode->width, mode->height, params.title.c_str(), monitor, nullptr);

  if (!glfw_handle) {
    const char *description;
    int code = glfwGetError(&description);
    YAGI_LOG_CRITICAL("Failed to create GLFW window: ({}) {}", code, description);
    glfwTerminate();
    std::exit(EXIT_FAILURE);
  } else
    YAGI_LOG_DEBUG("Created GLFW window");

  if (set(params.flags, WindowFlags::borderless)) {
    int base_x, base_y;
    glfwGetMonitorPos(monitor, &base_x, &base_y);
    glfwSetWindowPos(glfw_handle, base_x, base_y);
  }
#elif defined(YAGI_PLATFORM_LINUX)
  /* We are making the assumption that the user is running a version of X11
   * that treats *all* fullscreen windows as borderless fullscreen windows.
   * This seems to generally be true for a good majority of systems. This may
   * also just act exactly like a normal fullscreen, there's not really any
   * way to tell ahead of time.
   */

  GLFWmonitor *monitor = get_monitor_(params.monitor_num);
  const GLFWvidmode *mode = glfwGetVideoMode(monitor);

  glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);  // Why is this necessary?
  glfwWindowHint(GLFW_RED_BITS, mode->redBits);
  glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
  glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
  glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

  if (set(params.flags, WindowFlags::borderless)) {
    wm_info_.borderless = true;

    glfwWindowHint(GLFW_AUTO_ICONIFY, GLFW_FALSE);
  }

  glfw_handle = glfwCreateWindow(mode->width, mode->height, params.title.c_str(), monitor, nullptr);
  if (!glfw_handle) {
    const char *description;
    int code = glfwGetError(&description);
    YAGI_LOG_CRITICAL("Failed to create GLFW window:\n* ({}) {}", code, description);
    glfwTerminate();
    std::exit(EXIT_FAILURE);
  } else
    YAGI_LOG_DEBUG("Created GLFW window");
#endif
}

void Window::open_windowed_(const WindowOpenParams &params) {
  GLFWmonitor *monitor = get_monitor_(params.monitor_num);
  const GLFWvidmode *mode = glfwGetVideoMode(monitor);

  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
  glfwWindowHint(GLFW_RESIZABLE, set(params.flags, WindowFlags::resizable) ? GLFW_TRUE : GLFW_FALSE);

  glfw_handle = glfwCreateWindow(params.size.x, params.size.y, params.title.c_str(), nullptr, nullptr);
  if (!glfw_handle) {
    const char *description;
    int code = glfwGetError(&description);
    YAGI_LOG_CRITICAL("Failed to create GLFW window:\n* ({}) {}", code, description);
    glfwTerminate();
    std::exit(EXIT_FAILURE);
  } else
    YAGI_LOG_DEBUG("Created GLFW window");

  int base_x, base_y;
  glfwGetMonitorPos(monitor, &base_x, &base_y);
  if (set(params.flags, WindowFlags::centered))
    glfwSetWindowPos(
        glfw_handle,
        base_x + (mode->width - params.size.x) / 2,
        base_y + (mode->height - params.size.y) / 2
    );
  else
    glfwSetWindowPos(
        glfw_handle,
        base_x + params.pos.x,
        base_y + params.pos.y
    );
}

} // namespace yagi
