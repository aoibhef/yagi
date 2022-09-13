#include "yagi/gfx/context.h"
#include "yagi/util/log.h"
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

namespace yagi {

Context::Context(const glm::ivec2 &gl_version) {
  gl_ = std::make_unique<GladGLContext>();
  auto glad_version = gladLoadGLContext(gl_.get(), glfwGetProcAddress);
  if (glad_version == 0) {
    YAGI_LOG_CRITICAL("Failed to initialize OpenGL");
    std::exit(EXIT_FAILURE);
  }

  auto glad_major = GLAD_VERSION_MAJOR(glad_version);
  auto glad_minor = GLAD_VERSION_MINOR(glad_version);
  YAGI_LOG_DEBUG("Initialized OpenGL v{}.{}", glad_major, glad_minor);

  if (glad_major != gl_version.x || glad_minor != gl_version.y)
    YAGI_LOG_WARN("Requested OpenGL v{}.{}", gl_version.x, gl_version.y);
}

void Context::clear(const RGB &color, const ClearBit &bit) {
  gl_->ClearColor(
      static_cast<float>(color.r) / 255.0f,
      static_cast<float>(color.g) / 255.0f,
      static_cast<float>(color.b) / 255.0f,
      static_cast<float>(color.a) / 255.0f);
  gl_->Clear(unwrap(bit));
}

} // namespace yagi