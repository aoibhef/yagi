#ifndef YAGI_GFX_CONTEXT_H
#define YAGI_GFX_CONTEXT_H

#include "yagi/gfx/gl/framebuffer.h"
#include "yagi/gfx/gl/shader.h"
#include "yagi/gfx/color.h"
#include "yagi/util/enum_bitmask_ops.h"
#include "glad/gl.h"
#include "glm/glm.hpp"
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
  explicit Context(const glm::ivec2 &gl_version);

  void clear(const RGB &color, const ClearBit &bit = yagi::ClearBit::color | yagi::ClearBit::depth);

  ShaderBuilder shader();
  ShaderBuilder shader(const std::string &tag);

  FramebufferBuilder framebuffer(GLsizei width, GLsizei height);

private:
  std::unique_ptr<GladGLContext> gl_{nullptr};
};

} // namespace yagi

#endif //YAGI_GFX_CONTEXT_H
