#ifndef YAGI_MSG_MSG_H
#define YAGI_MSG_MSG_H

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "fmt/ostream.h"
#include <variant>

namespace yagi {

enum class MsgType {
  WindowClose,
  WindowSize,
  FramebufferSize,
  WindowContentScale,
  WindowPos,
  WindowIconify,
  WindowMaximize,
  WindowFocus,
  WindowRefresh,

  Monitor,

  Key,
  Character,
  CursorPos,
  CursorEnter,
  MouseButton,
  Scroll,
  Joystick,
  Drop,

  ImguiLog
};

template<MsgType>
struct Map;

#define DEFINE_MSG(t, ...)            \
  struct t {                          \
    __VA_ARGS__                       \
  };                                  \
  template<> struct Map<MsgType::t> { \
    using type = t;                   \
  };

DEFINE_MSG(WindowClose,
    GLFWwindow *window;
)

DEFINE_MSG(WindowSize,
    GLFWwindow *window;
    int width;
    int height;
)

DEFINE_MSG(FramebufferSize,
    GLFWwindow *window;
    int width;
    int height;
)

DEFINE_MSG(WindowContentScale,
    GLFWwindow *window;
    float xscale;
    float yscale;
)

DEFINE_MSG(WindowPos,
    GLFWwindow *window;
    int xpos;
    int ypos;
)

DEFINE_MSG(WindowIconify,
    GLFWwindow *window;
    int iconified;
)

DEFINE_MSG(WindowMaximize,
    GLFWwindow *window;
    int maximized;
)

DEFINE_MSG(WindowFocus,
    GLFWwindow *window;
    int focused;
)

DEFINE_MSG(WindowRefresh,
    GLFWwindow *window;
)

DEFINE_MSG(Monitor,
    GLFWmonitor *monitor;
    int event;
)

DEFINE_MSG(Key,
    GLFWwindow *window;
    int key;
    int scancode;
    int action;
    int mods;
)

DEFINE_MSG(Character,
    GLFWwindow *window;
    unsigned int codepoint;
)

DEFINE_MSG(CursorPos,
    GLFWwindow *window;
    double xpos;
    double ypos;
)

DEFINE_MSG(CursorEnter,
    GLFWwindow *window;
    int entered;
)

DEFINE_MSG(MouseButton,
    GLFWwindow *window;
    int button;
    int action;
    int mods;
)

DEFINE_MSG(Scroll,
    GLFWwindow *window;
    double xoffset;
    double yoffset;
)

DEFINE_MSG(Joystick,
    int jid;
    int event;
)

DEFINE_MSG(Drop,
    GLFWwindow *window;
    int count;
    const char **paths;
)

DEFINE_MSG(ImguiLog,
    std::string msg;
)

#undef DEFINE_MSG

struct Msg {
  MsgType type;
  std::variant<
      WindowClose,
      WindowSize,
      FramebufferSize,
      WindowContentScale,
      WindowPos,
      WindowIconify,
      WindowMaximize,
      WindowFocus,
      WindowRefresh,

      Monitor,

      Key,
      Character,
      CursorPos,
      CursorEnter,
      MouseButton,
      Scroll,
      Joystick,
      Drop,

      ImguiLog
  > data;
};

} // namespace yagi

inline std::ostream &operator<<(std::ostream &out, yagi::MsgType value) {
#define STRINGIFY(p) case (p): return out << #p;
  switch (value) {
    using enum yagi::MsgType;
    STRINGIFY(WindowClose)
    STRINGIFY(WindowSize)
    STRINGIFY(FramebufferSize)
    STRINGIFY(WindowContentScale)
    STRINGIFY(WindowPos)
    STRINGIFY(WindowIconify)
    STRINGIFY(WindowMaximize)
    STRINGIFY(WindowFocus)
    STRINGIFY(WindowRefresh)
    STRINGIFY(Monitor)
    STRINGIFY(Key)
    STRINGIFY(Character)
    STRINGIFY(CursorPos)
    STRINGIFY(CursorEnter)
    STRINGIFY(MouseButton)
    STRINGIFY(Scroll)
    STRINGIFY(Joystick)
    STRINGIFY(Drop)
    STRINGIFY(ImguiLog)
  }
#undef STRINGIFY
}

#endif //YAGI_MSG_MSG_H
