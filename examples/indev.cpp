#include "yagi/yagi.h"

class Indev : public yagi::Application {
public:
  std::unique_ptr<yagi::Shader> shader{nullptr};
  std::unique_ptr<yagi::StaticBuffer<float>> vbo{nullptr};
  std::unique_ptr<yagi::VertexArray> vao{nullptr};

  void initialize() override {
    window->open({.title = "Indev",
                  .size = {1600, 900},
                  .flags = yagi::WindowFlags::centered});
    app_debug_overlay_enabled(true);
    input->bind_func("escape", [&]{ app_shutdown(); });

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

    vbo = ctx->static_buffer<float>(yagi::BufTarget::array, yagi::BufUsage::static_draw, {
         0.0,  0.3, 0.0,   1.0, 0.0, 0.0, 1.0,
        -0.3, -0.3, 0.0,   0.0, 1.0, 0.0, 1.0,
         0.3, -0.3, 0.0,   0.0, 0.0, 1.0, 1.0
    });

    vao = ctx->vertex_array(*shader, {
        {*vbo, {{"in_pos", 3, yagi::AttrType::f, false, 7, 0},
                {"in_color", 4, yagi::AttrType::f, false, 7, 3}}}
    });
  }

  void update(double dt) override {
    if (input->pressed("1"))
      window->set_decorated(!window->decorated);

    if (input->pressed("2"))
      window->set_vsync(!window->vsync);

    if (input->pressed("3"))
      window->set_resizable(!window->resizable);

    if (input->down("a", 0.1)) {
      int log_level = rnd::get<int>(0, 5);

      std::string s(16, 0);
      std::generate_n(s.begin(), 16, []() -> char { return rnd::get<char>('A', 'Z'); });

      switch (log_level) {
        case 0: YAGI_LOG_TRACE("{}", s); break;
        case 1: YAGI_LOG_DEBUG("{}", s); break;
        case 2: YAGI_LOG_INFO("{}", s); break;
        case 3: YAGI_LOG_WARN("{}", s); break;
        case 4: YAGI_LOG_ERROR("{}", s); break;
        case 5: YAGI_LOG_CRITICAL("{}", s); break;
      }
    }
  }

  void draw() override {
    ctx->clear(yagi::rgb(0x000000));

    shader->use();
    shader->uniform_mat4f("mvp", glm::mat4(1.0f));
    vao->draw_arrays(yagi::DrawMode::triangles, 0, 3);
  }
};

YAGI_CONSULT_ORACLE(Indev)
