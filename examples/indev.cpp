#include "indev.h"

class Indev : public yagi::Application {
public:
  void initialize() override {
    window->open({.title = "Indev", .size = {800, 600}, .flags = yagi::WindowFlags::centered});

    fmt::print("Initialize indev!\n");
  }

  void update(double dt) override {

  }

  void draw() override {

  }
};

int main(int, char *[]) {
  yagi::Oracle().run_application<Indev>();
}
