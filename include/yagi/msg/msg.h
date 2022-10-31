#ifndef YAGI_MSG_MSG_H
#define YAGI_MSG_MSG_H

#include "yagi/util/overloaded.h"
#include "fmt/ostream.h"
#include <variant>

namespace yagi::msg {

enum class Type {
  AEvent,
  BEvent,
  CEvent
};

template<Type>
struct Map;

#define DEFINE_MSG(t, ...)               \
  struct t {                             \
    __VA_ARGS__                          \
  };                                     \
  template<> struct Map<Type::t> { \
    using type = t;                      \
  };

DEFINE_MSG(AEvent,
  int a;
  int b;
)

DEFINE_MSG(BEvent,
  int a;
  std::string s;
)

DEFINE_MSG(CEvent,
  double a;
  double b;
  double c;
)

#undef DEFINE_MSG

struct Msg {
  Type type;
  std::variant<AEvent, BEvent, CEvent> data;
};

} // namespace yagi::msg

inline std::ostream &operator<<(std::ostream &out, yagi::msg::Type value) {
  std::string s;
#define STRINGIFY(p) case (p): s = #p; break;
  switch (value) {
    using enum yagi::msg::Type;
    STRINGIFY(AEvent)
    STRINGIFY(BEvent)
    STRINGIFY(CEvent)
  }
#undef STRINGIFY
  return out << s;
}
template<> struct fmt::formatter<yagi::msg::Type> : ostream_formatter {};

#endif //YAGI_MSG_MSG_H
