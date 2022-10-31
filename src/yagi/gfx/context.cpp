#include "yagi/util/log.h"
#include "yagi/gfx/context.h"
#include "yagi/util/rnd.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace yagi {

Context::Context(GLFWwindow *glfw_window, const glm::ivec2 &gl_version) {
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

  gl_->Enable(GL_DEBUG_OUTPUT);
  gl_->DebugMessageCallback(gl_message_callback_, nullptr);

  initialize_imgui_(glfw_window);
}

Context::~Context() {
  ImGui::DestroyContext(imgui_.ctx);
  ImPlot::DestroyContext(implot_.ctx);
}

Context::Context(Context &&other) noexcept {
  *this = std::move(other);
}

Context &Context::operator=(Context &&other) noexcept {
  // TODO: This is definitely wrong, I think this will leave
  //   the 'other' context in an unusable state with a void context

  if (this != &other) {
    ImGui::DestroyContext(imgui_.ctx);
    ImPlot::DestroyContext(implot_.ctx);
    gl_.reset();

    gl_.swap(other.gl_);
    std::swap(imgui_, other.imgui_);
    std::swap(implot_, other.implot_);

    other.imgui_.ctx = nullptr;
    other.imgui_.io = nullptr;
    other.implot_.ctx = nullptr;
  }
  return *this;
}

void Context::clear(const RGB &color, const ClearBit &bit) {
  gl_->ClearColor(
      static_cast<float>(color.r) / 255.0f,
      static_cast<float>(color.g) / 255.0f,
      static_cast<float>(color.b) / 255.0f,
      static_cast<float>(color.a) / 255.0f);
  gl_->Clear(unwrap(bit));
}

ShaderBuilder Context::shader() {
  return shader(rnd::base58(11));
}

ShaderBuilder Context::shader(const std::string &tag) {
  return {*gl_, tag};
}

FramebufferBuilder Context::framebuffer(GLsizei width, GLsizei height) {
  return {*gl_, width, height};
}

std::unique_ptr<VertexArray> Context::vertex_array(
    Shader &shader,
    const std::vector<BufAttrs> &buf_attrs
) {
  return std::make_unique<VertexArray>(*gl_, shader, buf_attrs);
}

GladGLContext &Context::get_underlying_ctx() {
  return *gl_;
}

void Context::initialize_imgui_(GLFWwindow *glfw_window) {
  IMGUI_CHECKVERSION();
  imgui_.ctx = ImGui::CreateContext();
  imgui_.io = &ImGui::GetIO();
  imgui_.io->IniFilename = nullptr;

  ImGui_ImplGlfw_InitForOpenGL(glfw_window, true);
  ImGui_ImplOpenGL3_Init();

  implot_.ctx = ImPlot::CreateContext();

  YAGI_LOG_DEBUG("Initialized ImGui");
}

void GLAPIENTRY Context::gl_message_callback_(
    GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei,
    const GLchar *message,
    const void *
) {
#define STRINGIFY(e) case e: return #e;
  std::string source_str = ([source](){
    switch (source) {
      STRINGIFY(GL_DEBUG_SOURCE_API)
      STRINGIFY(GL_DEBUG_SOURCE_WINDOW_SYSTEM)
      STRINGIFY(GL_DEBUG_SOURCE_SHADER_COMPILER)
      STRINGIFY(GL_DEBUG_SOURCE_THIRD_PARTY)
      STRINGIFY(GL_DEBUG_SOURCE_APPLICATION)
      STRINGIFY(GL_DEBUG_SOURCE_OTHER)
      default: return "?";
    }
  })();

  std::string type_str = ([type](){
    switch (type) {
      STRINGIFY(GL_DEBUG_TYPE_ERROR)
      STRINGIFY(GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR)
      STRINGIFY(GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR)
      STRINGIFY(GL_DEBUG_TYPE_PORTABILITY)
      STRINGIFY(GL_DEBUG_TYPE_PERFORMANCE)
      STRINGIFY(GL_DEBUG_TYPE_MARKER)
      STRINGIFY(GL_DEBUG_TYPE_PUSH_GROUP)
      STRINGIFY(GL_DEBUG_TYPE_POP_GROUP)
      STRINGIFY(GL_DEBUG_TYPE_OTHER)
      default: return "?";
    }
  })();

  switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH:
      YAGI_LOG_ERROR("OpenGL: source={} type={} id={} msg={}", source_str, type_str, id, message);
      break;
    case GL_DEBUG_SEVERITY_MEDIUM:
      YAGI_LOG_WARN("OpenGL: source={} type={} id={} msg={}", source_str, type_str, id, message);
      break;
    case GL_DEBUG_SEVERITY_LOW:
      YAGI_LOG_DEBUG("OpenGL: source={} type={} id={} msg={}", source_str, type_str, id, message);
      break;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
      YAGI_LOG_TRACE("OpenGL: source={} type={} id={} msg={}", source_str, type_str, id, message);
      break;
    default:
      break; // won't happen
  }
#undef STRINGIFY
}

} // namespace yagi
