#ifndef YAGI_MSG_MSG_H
#define YAGI_MSG_MSG_H

#include "yagi/util/overloaded.h"
#include "fmt/ostream.h"
#include <variant>

namespace yagi::msg {

enum class MsgType {
  AEvent,
  BEvent,
  CEvent
};

template<MsgType>
struct Map;

#define DEFINE_MSG(t, ...)         \
  struct t {                       \
    __VA_ARGS__                    \
  };                               \
  template<> struct Map<MsgType::t> { \
    using type = t;                \
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
  MsgType type;
  std::variant<AEvent, BEvent, CEvent> data;
};

} // namespace yagi::msg

inline std::ostream &operator<<(std::ostream &out, yagi::msg::MsgType value) {
  std::string s;
#define STRINGIFY(p) case (p): s = #p; break;
  switch (value) {
    using enum yagi::msg::MsgType;
    STRINGIFY(AEvent)
    STRINGIFY(BEvent)
    STRINGIFY(CEvent)
  }
#undef STRINGIFY
  return out << s;
}
template<> struct fmt::formatter<yagi::msg::MsgType> : ostream_formatter {};

#endif //YAGI_MSG_MSG_H
