#ifndef YAGI_MSG_MSG_H
#define YAGI_MSG_MSG_H

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "fmt/ostream.h"
#include <variant>

namespace yagi {

enum class MsgType {
  WindowPos
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

DEFINE_MSG(WindowPos,
  GLFWwindow *window;
  int xpos;
  int ypos;
)

#undef DEFINE_MSG

struct Msg {
  MsgType type;
  std::variant<WindowPos> data;
};

} // namespace yagi

inline std::ostream &operator<<(std::ostream &out, yagi::MsgType value) {
#define STRINGIFY(p) case (p): return out << #p;
  switch (value) {
    using enum yagi::MsgType;
    STRINGIFY(WindowPos)
  }
#undef STRINGIFY
}

#endif //YAGI_MSG_MSG_H
