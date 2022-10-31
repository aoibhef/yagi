#ifndef YAGI_GFX_CONTEXT_H
#define YAGI_GFX_CONTEXT_H

#include "yagi/gfx/gl/framebuffer.h"
#include "yagi/gfx/gl/shader.h"
#include "yagi/gfx/gl/static_buffer.h"
#include "yagi/gfx/gl/vertex_array.h"
#include "yagi/gfx/color.h"
#include "yagi/util/enum_bitmask_ops.h"
#include "yagi/util/log.h"
#include "glad/gl.h"
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "imgui.h"
#include <concepts>
#include <memory>

namespace yagi {

enum class ClearBit {
  stencil = GL_STENCIL_BUFFER_BIT,
  color = GL_COLOR_BUFFER_BIT,
  depth = GL_DEPTH_BUFFER_BIT
};

} // namespace yagi

ENABLE_BITMASK_OPERATORS(yagi::ClearBit)

namespace yagi {

class Context {
public:
  explicit Context(GLFWwindow *glfw_window, const glm::ivec2 &gl_version);
  ~Context();

  // Copy constructors don't make sense for OpenGL objects
  Context(const Context &) = delete;
  Context &operator=(const Context &) = delete;

  Context(Context &&other) noexcept;
  Context &operator=(Context &&other) noexcept;
  
  void clear(const RGB &color, const ClearBit &bit = yagi::ClearBit::color | yagi::ClearBit::depth);

  ShaderBuilder shader();
  ShaderBuilder shader(const std::string &tag);

  FramebufferBuilder framebuffer(GLsizei width, GLsizei height);

  template <Numeric T>
  std::unique_ptr<StaticBuffer<T>> static_buffer(
      const BufTarget &target,
      const BufUsage &usage,
      const std::vector<T> &data
  );

  std::unique_ptr<VertexArray> vertex_array(
      Shader &shader,
      const std::vector<BufAttrs> &buf_attrs
  );

  template <std::invocable<std::unique_ptr<GladGLContext> &> T>
  void run_block(T &&func) { func(gl_); }

  std::unique_ptr<GladGLContext> &get_underlying_ctx();

private:
  std::unique_ptr<GladGLContext> gl_{nullptr};

  struct {
    ImGuiContext *ctx{nullptr};
    ImGuiIO *io{nullptr};
  } imgui_{};

  void initialize_imgui_(GLFWwindow *glfw_window);

  static void GLAPIENTRY gl_message_callback_(
      GLenum source,
      GLenum type,
      GLuint id,
      GLenum severity,
      GLsizei length,
      const GLchar *message,
      const void *userParam
  );
};

template <Numeric T>
std::unique_ptr<StaticBuffer<T>> Context::static_buffer(
    const BufTarget &target,
    const BufUsage &usage,
    const std::vector<T> &data
) {
  return std::make_unique<StaticBuffer<T>>(gl_, target, usage, data);
}

} // namespace yagi

#endif //YAGI_GFX_CONTEXT_H
