#include "indev.h"

int main(int, char *[]) {
  auto id1 = Messenger::get_instance().create_id([](const Msg &msg) {
    switch (msg.type) {
      case MsgType::AEvent: {
        auto pay = Messenger::extract_msg_payload<MsgType::AEvent>(msg.payload);
        fmt::print("id1 | A: {}, {}\n", pay.a, pay.b);
      }
        break;

      case MsgType::BEvent: {
        auto pay = Messenger::extract_msg_payload<MsgType::BEvent>(msg.payload);
        fmt::print("id1 | B: {}, {}\n", pay.a, pay.s);
      }
        break;

      case MsgType::CEvent: {
        auto pay = Messenger::extract_msg_payload<MsgType::CEvent>(msg.payload);
        fmt::print("id1 | C: {}, {}, {}\n", pay.a, pay.b, pay.c);
      }
        break;
    }
  });
  Messenger::get_instance().subscribe(id1, MsgType::AEvent);
  Messenger::get_instance().subscribe(id1, MsgType::BEvent);

  auto id2 = Messenger::get_instance().create_id([](const Msg &msg) {
    switch (msg.type) {
      case MsgType::AEvent: {
        auto pay = Messenger::extract_msg_payload<MsgType::AEvent>(msg.payload);
        fmt::print("id2 | A: {}, {}\n", pay.a, pay.b);
      }
        break;

      case MsgType::BEvent: {
        auto pay = Messenger::extract_msg_payload<MsgType::BEvent>(msg.payload);
        fmt::print("id2 | B: {}, {}\n", pay.a, pay.s);
      }
        break;

      case MsgType::CEvent: {
        auto pay = Messenger::extract_msg_payload<MsgType::CEvent>(msg.payload);
        fmt::print("id2 | C: {}, {}, {}\n", pay.a, pay.b, pay.c);
      }
        break;
    }
  });
  Messenger::get_instance().subscribe(id2, MsgType::AEvent);
  Messenger::get_instance().subscribe(id2, MsgType::CEvent);

  Messenger::get_instance().send<MsgType::AEvent>(1, 2);
  Messenger::get_instance().send<MsgType::BEvent>(1, "Hello");
  Messenger::get_instance().send<MsgType::CEvent>(1.1, 2.2, 3.3);

  Messenger::get_instance().poll(id1);
  Messenger::get_instance().poll(id2);
}
