#ifndef YAGI_APP_GLFW_CALLBACKS_H
#define YAGI_APP_GLFW_CALLBACKS_H

#include "GLFW/glfw3.h"

namespace yagi::internal {

void register_glfw_callbacks(GLFWwindow *window);

void error_callback(int code, const char *description);

void window_size_callback(GLFWwindow *window, int width, int height);

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void window_content_scale_callback(GLFWwindow *window, float xscale, float yscale);

void window_pos_callback(GLFWwindow *window, int xpos, int ypos);

void window_iconify_callback(GLFWwindow *window, int iconified);

void window_maximize_callback(GLFWwindow *window, int maximized);

void window_focus_callback(GLFWwindow *window, int focused);

void window_refresh_callback(GLFWwindow *window);

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

void character_callback(GLFWwindow *window, unsigned int codepoint, int mods);

void cursor_position_callback(GLFWwindow *window, double xpos, double ypos);

void cursor_enter_callback(GLFWwindow *window, int entered);

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

void joystick_callback(int jid, int event);

void drop_callback(GLFWwindow *window, int count, const char** paths);

void monitor_callback(GLFWmonitor *monitor, int event);

} // namespace yagi::internal

#endif //YAGI_APP_GLFW_CALLBACKS_H