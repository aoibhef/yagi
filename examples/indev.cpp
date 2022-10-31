#include "indev.h"

int main(int, char *[]) {
  using namespace yagi;

  auto id1 = msg::Bus::create_id([](const msg::Msg &msg) {
    std::visit(overloaded {
        [&](const msg::AEvent &e) { fmt::print("id1 | A: {}, {}\n", e.a, e.b); },
        [&](const msg::BEvent &e) { fmt::print("id1 | B: {}, {}\n", e.a, e.s); },
        [&](const auto &e)        { YAGI_LOG_WARN("Unhandled event {}", msg.type); },
    }, msg.data);
  });
  msg::Bus::subscribe(id1, msg::MsgType::AEvent);
  msg::Bus::subscribe(id1, msg::MsgType::BEvent);

  auto id2 = msg::Bus::create_id([](const msg::Msg &msg) {
    std::visit(overloaded {
        [&](const msg::AEvent &e) { fmt::print("id2 | A: {}, {}\n", e.a, e.b); },
        [&](const msg::CEvent &e) { fmt::print("id2 | C: {}, {}, {}\n", e.a, e.b, e.c); },
        [&](const auto &e)        { YAGI_LOG_WARN("Unhandled event {}", msg.type); },
    }, msg.data);
  });
  msg::Bus::subscribe(id2, msg::MsgType::AEvent);
  msg::Bus::subscribe(id2, msg::MsgType::CEvent);

  msg::Bus::send<msg::MsgType::AEvent>(1, 2);
  msg::Bus::send<msg::MsgType::BEvent>(1, "Hello");
  msg::Bus::send<msg::MsgType::CEvent>(1.1, 2.2, 3.3);

  msg::Bus::poll(id1);
  msg::Bus::poll(id2);
}

