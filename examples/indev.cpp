#include "indev.h"

using namespace yagi;

class Foo {
public:
  std::string id;

  Foo() {
    id = msg::Bus::register_endpoint([this](const auto &msg){ process_msg(msg); });

    msg::Bus::subscribe(id, msg::Type::AEvent);
    msg::Bus::subscribe(id, msg::Type::BEvent);
    msg::Bus::subscribe(id, msg::Type::CEvent);
  }

  void poll_msgs() {
    msg::Bus::poll(id);
  }

  void process_msg(const msg::Msg &msg) {
    std::visit(overloaded {
        [&](const msg::AEvent &e) { fmt::print("A: {}, {}\n", e.a, e.b); },
        [&](const msg::BEvent &e) { fmt::print("B: {}, {}\n", e.a, e.s); },
        [&](const msg::CEvent &e) { fmt::print("C: {}, {}, {}\n", e.a, e.b, e.c); },
    }, msg.data);
  }
};

int main(int, char *[]) {
  auto f = Foo();

  msg::Bus::send<msg::Type::AEvent>(1, 2);
  msg::Bus::send<msg::Type::BEvent>(1, "Hello");
  msg::Bus::send<msg::Type::CEvent>(1.1, 2.2, 3.3);

  f.poll_msgs();
}

