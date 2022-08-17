#ifndef YAGI_EXAMPLES_INDEV_H
#define YAGI_EXAMPLES_INDEV_H

#include "yagi/util/log.h"
#include "yagi/util/rnd.h"
#include "fmt/format.h"
#include "fmt/ostream.h"
#include <any>
#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <queue>

template<class ...Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class ...Ts> overloaded(Ts...) -> overloaded<Ts...>;

namespace yagi::msg {

enum class MsgType {
  AEvent,
  BEvent,
  CEvent
};

std::ostream &operator<<(std::ostream &out, MsgType value) {
  std::string s;
#define STRINGIFY(p) case (p): s = #p; break;
  switch (value) {
    using
    enum MsgType;
    STRINGIFY(AEvent)
    STRINGIFY(BEvent)
    STRINGIFY(CEvent)
  }
#undef STRINGIFY
  return out << s;
}

template<MsgType>
struct MsgMap;

#define DEFINE_MSG(t, ...)               \
  struct t {                             \
    __VA_ARGS__                          \
  };                                     \
  template<> struct MsgMap<MsgType::t> { \
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
  MsgType type;
  std::variant<AEvent, BEvent, CEvent> data;
};

using MsgFunc = std::function<void(const Msg &)>;

class Bus {
public:
  static std::string create_id(const MsgFunc &&f);

  static void subscribe(const std::string &id, const MsgType &type);

  template<MsgType T, typename... Args>
  static void send(const Args &...args);

  static void poll(const std::string &id);

private:
  Bus() = default;

  static Bus &get_instance_();

  std::unordered_map<std::string, MsgFunc> funcs_{};
  std::unordered_map<MsgType, std::unordered_set<std::string>> subscriptions_{};
  std::unordered_map<std::string, std::queue<Msg>> queues_{};
};

std::string Bus::create_id(const MsgFunc &&f) {
  std::string id;
  do {
    id = rnd::base58(11);
  } while (get_instance_().funcs_.contains(id));

  get_instance_().funcs_[id] = f;
  get_instance_().queues_[id] = std::queue<Msg>();

  return id;
}

void Bus::subscribe(const std::string &id, const MsgType &type) {
  get_instance_().subscriptions_[type].insert(id);
}

template<MsgType T, typename... Args>
void Bus::send(const Args &...args) {
  for (const auto &id: get_instance_().subscriptions_[T])
    get_instance_().queues_[id].push(Msg{T, typename MsgMap<T>::type{args...}});
}

void Bus::poll(const std::string &id) {
  while (!get_instance_().queues_[id].empty()) {
    get_instance_().funcs_[id](get_instance_().queues_[id].front());
    get_instance_().queues_[id].pop();
  }
}

Bus &Bus::get_instance_() {
  static auto instance = Bus();

  return instance;
}

} // namespace yagi::msg

template<> struct fmt::formatter<yagi::msg::MsgType> : ostream_formatter {};

#endif //YAGI_EXAMPLES_INDEV_H
