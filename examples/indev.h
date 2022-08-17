#ifndef YAGI_EXAMPLES_INDEV_H
#define YAGI_EXAMPLES_INDEV_H

#include "yagi/util/log.h"
#include "yagi/util/rnd.h"
#include <any>
#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <queue>

template<class ...Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class ...Ts> overloaded(Ts...) -> overloaded<Ts...>;

enum class MsgType {
  AEvent,
  BEvent,
  CEvent
};

template<MsgType>
struct PayloadMap;

#define DEFINE_MSG_PAYLOAD(msg, ...)           \
  template<> struct PayloadMap<MsgType::msg> { \
    using type = struct msg ## Pay {           \
      __VA_ARGS__                              \
    };                                         \
  };

DEFINE_MSG_PAYLOAD(AEvent,
  int a;
  int b;
)

DEFINE_MSG_PAYLOAD(BEvent,
  int a;
  std::string s;
)

DEFINE_MSG_PAYLOAD(CEvent,
  double a;
  double b;
  double c;
)

#undef DEFINE_MSG_PAYLOAD

struct Msg {
  const MsgType type;
  std::any payload;
};

using MsgFunc = std::function<void(const Msg &)>;

class Messenger {
public:
  static Messenger &get_instance();

  std::string create_id(const MsgFunc &&f);

  void subscribe(const std::string &id, const MsgType &type);

  template <MsgType T, typename... Args>
  void send(const Args &...args);

  void poll(const std::string &id);

  template<MsgType T>
  static auto extract_msg_payload(const std::any &payload);

private:
  Messenger() = default;

  std::unordered_map<std::string, MsgFunc> funcs_{};
  std::unordered_map<MsgType, std::unordered_set<std::string>> subscriptions_{};
  std::unordered_map<std::string, std::queue<Msg>> queues_{};
};

Messenger &Messenger::get_instance() {
  static auto instance = Messenger();

  return instance;
}

std::string Messenger::create_id(const MsgFunc &&f) {
  std::string id;
  do {
    id = rnd::base58(11);
  } while (funcs_.contains(id));

  funcs_[id] = f;
  queues_[id] = std::queue<Msg>();

  return id;
}

void Messenger::subscribe(const std::string &id, const MsgType &type) {
  subscriptions_[type].insert(id);
}

template <MsgType T, typename... Args>
void Messenger::send(const Args &...args) {
  for (const auto &id : subscriptions_[T])
    queues_[id].push(Msg{T, typename PayloadMap<T>::type{args...}});
}

void Messenger::poll(const std::string &id) {
  while (!queues_[id].empty()) {
    funcs_[id](queues_[id].front());
    queues_[id].pop();
  }
}

template<MsgType T>
auto Messenger::extract_msg_payload(const std::any &payload) {
  return std::any_cast<typename PayloadMap<T>::type>(payload);
}

#endif //YAGI_EXAMPLES_INDEV_H
