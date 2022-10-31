#include "yagi/util/log.h"
#include "yagi/core/oracle.h"
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

namespace yagi {

std::once_flag Oracle::initialized_glfw_{};

Oracle::Oracle() {
  std::call_once(initialized_glfw_, [&]{
    if (glfwInit() == GLFW_FALSE)
      std::exit(EXIT_FAILURE);

    int major, minor, revision;
    glfwGetVersion(&major, &minor, &revision);
    YAGI_LOG_DEBUG("Initialized GLFW v{}.{}.{}", major, minor, revision);
  });
}

Oracle::~Oracle() {
  glfwTerminate();
}

} // namespace yagi
