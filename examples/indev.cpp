#include "indev.h"

class Indev : public yagi::Application {
public:
  yagi::HSV clear_color{yagi::hsv(0, 1.0, 1.0)};

  std::unique_ptr<yagi::Shader> shader{nullptr};

  void initialize() override {
    window->open({.title = "Indev",
                  .size = {800, 600},
                  .flags = yagi::WindowFlags::centered});
    ctx = window->create_ctx();

    shader = ctx->shader()
        .vert_from_src(R"glsl(
#version 330 core
layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec4 in_color;

out vec4 out_color;

uniform mat4 mvp;

void main() {
  out_color = in_color;
  gl_Position = mvp * vec4(in_pos, 1.0);
}
)glsl")
        .frag_from_src(R"glsl(
#version 330 core
in vec4 out_color;

out vec4 FragColor;

void main() {
  FragColor = out_color;
}
)glsl")
        .link();
  }

  void update(double dt) override {
    clear_color.h = std::fmod(clear_color.h + (20.0 * dt), 360.0);
  }

  void draw() override {
    ctx->clear(clear_color.to_rgb());

    shader->use();
  }
};

int main(int, char *[]) {
  yagi::Oracle().run_application<Indev>();
}
