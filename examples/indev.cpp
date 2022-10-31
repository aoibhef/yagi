#include "indev.h"

class Indev : public yagi::Application {
public:
  void initialize() override {
    window->open({.title = "Indev",
                  .size = {800, 600},
                  .flags = yagi::WindowFlags::centered});
    ctx = window->create_ctx();
  }

  void update(double dt) override {
    clear_color.h = std::fmod(clear_color.h + (60.0 * dt), 360.0);
    clear_color.v = std::fmod(clear_color.v + (0.1 * dt), 1.0);
  }

  void draw() override {
    ctx->clear(clear_color.to_rgb());
  }

private:
  yagi::HSV clear_color{yagi::hsv(0, 1.0, 1.0)};
};

int main(int, char *[]) {
  yagi::Oracle().run_application<Indev>();
}
