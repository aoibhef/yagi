#include "indev.h"

class Indev : public yagi::Application {
public:
  std::unique_ptr<yagi::Shader> shader{nullptr};
  std::unique_ptr<yagi::StaticBuffer<float>> vbo{nullptr};
  GLuint vao;

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

    const std::vector<float> vertices = {
         0.0,  0.5, 0.0,   1.0, 0.0, 0.0, 1.0,
        -0.5, -0.5, 0.0,   0.0, 1.0, 0.0, 1.0,
         0.5, -0.5, 0.0,   0.0, 0.0, 1.0, 1.0
    };
    vbo = ctx->static_buffer<float>(vertices, yagi::BufTarget::array, yagi::BufUsage::static_draw);

    ctx->run_block([&](const std::unique_ptr<GladGLContext> &gl) {
      gl->GenVertexArrays(1, &vao);
      gl->BindVertexArray(vao);

      vbo->bind(yagi::BufTarget::array);
      gl->VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), nullptr);
      gl->EnableVertexAttribArray(0);
      gl->VertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *)(sizeof(float) * 3));
      gl->EnableVertexAttribArray(1);
      vbo->unbind(yagi::BufTarget::array);
    });
  }

  void update(double dt) override {}

  void draw() override {
    ctx->clear(yagi::rgb(0x000000));

    shader->use();
    shader->uniform_mat4f("mvp", glm::mat4(1.0f));
    ctx->run_block([&](const std::unique_ptr<GladGLContext> &gl) {
      gl->BindVertexArray(vao);
      gl->DrawArrays(GL_TRIANGLES, 0, 3);
    });
  }
};

int main(int, char *[]) {
  yagi::Oracle().run_application<Indev>();
}
