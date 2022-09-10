#include "yagi/app/glfw_callbacks.h"

#include "yagi/msg/bus.h"
#include "yagi/util/log.h"

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
  glfwSetCharModsCallback(window, character_callback);
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

void window_size_callback(GLFWwindow *window, int width, int height) {}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {}

void window_content_scale_callback(GLFWwindow *window, float xscale, float yscale) {}

void window_pos_callback(GLFWwindow *window, int xpos, int ypos) {
  msg::Bus::send<msg::MsgType::WindowPos>(window, xpos, ypos);
}

void window_iconify_callback(GLFWwindow *window, int iconified) {}

void window_maximize_callback(GLFWwindow *window, int maximized) {}

void window_focus_callback(GLFWwindow *window, int focused) {}

void window_refresh_callback(GLFWwindow *window) {}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {}

void character_callback(GLFWwindow *window, unsigned int codepoint, int mods) {}

void cursor_position_callback(GLFWwindow *window, double xpos, double ypos) {}

void cursor_enter_callback(GLFWwindow *window, int entered) {}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {}

void joystick_callback(int jid, int event) {}

void drop_callback(GLFWwindow *window, int count, const char** paths) {}

void monitor_callback(GLFWmonitor *monitor, int event) {}

} // namespace yagi::internal
