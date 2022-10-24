#include "yagi/msg/bus.h"
#include "yagi/util/log.h"
#include "yagi/util/rnd.h"

namespace yagi {

std::unordered_map<std::string, MsgFunc> Bus::funcs_{};
std::unordered_map<MsgType, std::unordered_set<std::string>> Bus::subscriptions_{};
std::unordered_map<std::string, std::queue<Msg>> Bus::queues_{};

std::string Bus::register_endpoint(const MsgFunc &&f) {
  std::string id;
  do {
    id = rnd::base58(11);
  } while (funcs_.contains(id));

  funcs_[id] = f;
  queues_[id] = std::queue<Msg>();

  return id;
}

void Bus::subscribe(const std::string &id, const MsgType &type) {
  subscriptions_[type].insert(id);
}

void Bus::poll(const std::string &id) {
  while (!queues_[id].empty()) {
    funcs_[id](queues_[id].front());
    queues_[id].pop();
  }
}

void Bus::poll_all() {
  for (auto &q : queues_)
    poll(q.first);
}

} // namespace yagi
