#include "yagi/util/log.h"
#include "yagi/core/glfw_callbacks.h"
#include "yagi/msg/bus.h"

namespace yagi::internal {

void register_glfw_callbacks(GLFWwindow *window) {
  glfwSetErrorCallback(error_callback);

  glfwSetWindowSizeCallback(window, window_size_callback);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetWindowContentScaleCallback(window, window_content_scale_callback);
  glfwSetWindowPosCallback(window, window_pos_callback);
  glfwSetWindowIconifyCallback(window, window_iconify_callback);
  glfwSetWindowMaximizeCallback(window, window_maximize_callback);
  glfwSetWindowFocusCallback(window, window_focus_callback);
  glfwSetWindowRefreshCallback(window, window_refresh_callback);

  glfwSetKeyCallback(window, key_callback);
  glfwSetCharCallback(window, character_callback);
  glfwSetCursorPosCallback(window, cursor_position_callback);
  glfwSetCursorEnterCallback(window, cursor_enter_callback);
  glfwSetMouseButtonCallback(window, mouse_button_callback);
  glfwSetScrollCallback(window, scroll_callback);
  glfwSetJoystickCallback(joystick_callback);
  glfwSetDropCallback(window, drop_callback);

  glfwSetMonitorCallback(monitor_callback);

  YAGI_LOG_DEBUG("Registered GLFW callbacks");
}

void error_callback(int code, const char *description) {
  YAGI_LOG_ERROR("GLFW ({}): {}", code, description);
}

void window_size_callback(GLFWwindow *window, int width, int height) {
  Bus::send_nowait<MsgType::WindowSize>(window, width, height);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  Bus::send_nowait<MsgType::FramebufferSize>(window, width, height);
}

void window_content_scale_callback(GLFWwindow *window, float xscale, float yscale) {
  Bus::send_nowait<MsgType::WindowContentScale>(window, xscale, yscale);
}

void window_pos_callback(GLFWwindow *window, int xpos, int ypos) {
  Bus::send_nowait<MsgType::WindowPos>(window, xpos, ypos);
}

void window_iconify_callback(GLFWwindow *window, int iconified) {
  Bus::send_nowait<MsgType::WindowIconify>(window, iconified);
}

void window_maximize_callback(GLFWwindow *window, int maximized) {
  Bus::send_nowait<MsgType::WindowMaximize>(window, maximized);
}

void window_focus_callback(GLFWwindow *window, int focused) {
  Bus::send_nowait<MsgType::WindowFocus>(window, focused);
}

void window_refresh_callback(GLFWwindow *window) {
  Bus::send_nowait<MsgType::WindowRefresh>(window);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
  Bus::send_nowait<MsgType::Key>(window, key, scancode, action, mods);
}

void character_callback(GLFWwindow *window, unsigned int codepoint) {
  Bus::send_nowait<MsgType::Character>(window, codepoint);
}

void cursor_position_callback(GLFWwindow *window, double xpos, double ypos) {
  Bus::send_nowait<MsgType::CursorPos>(window, xpos, ypos);
}

void cursor_enter_callback(GLFWwindow *window, int entered) {
  Bus::send_nowait<MsgType::CursorEnter>(window, entered);
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
  Bus::send_nowait<MsgType::MouseButton>(window, button, action, mods);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
  Bus::send_nowait<MsgType::Scroll>(window, xoffset, yoffset);
}

void joystick_callback(int jid, int event) {
  Bus::send_nowait<MsgType::Joystick>(jid, event);
}

void drop_callback(GLFWwindow *window, int count, const char** paths) {
  Bus::send_nowait<MsgType::Drop>(window, count, paths);
}

void monitor_callback(GLFWmonitor *monitor, int event) {
  Bus::send_nowait<MsgType::Monitor>(monitor, event);
}

} // namespace yagi::internal
