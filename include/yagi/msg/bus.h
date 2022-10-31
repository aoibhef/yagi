#ifndef YAGI_MSG_BUS_H
#define YAGI_MSG_BUS_H

#include "yagi/msg/msg_def.h"
#include <functional>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace yagi {

using MsgFunc = std::function<void(const Msg &)>;

class Bus {
public:
  static std::string register_endpoint(const MsgFunc &&f);

  static void subscribe(const std::string &id, const MsgType &type);

  template<MsgType T, typename... Args>
  static void send(const Args &...args);

  static void poll(const std::string &id);
  static void poll_all();

private:
  static std::unordered_map<std::string, MsgFunc> funcs_;
  static std::unordered_map<MsgType, std::unordered_set<std::string>> subscriptions_;
  static std::unordered_map<std::string, std::queue<Msg>> queues_;
};

template<MsgType T, typename... Args>
void Bus::send(const Args &...args) {
  for (const auto &id : subscriptions_[T])
    queues_[id].push(Msg{T, typename Map<T>::type{args...}});
}

} // namespace yagi

#endif //YAGI_MSG_BUS_H
