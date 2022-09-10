#include "yagi/app/oracle.h"

#include "yagi/util/log.h"
#include "GLFW/glfw3.h"

namespace yagi {

std::once_flag Oracle::initialized_glfw_{};

Oracle::Oracle() {
  std::call_once(initialized_glfw_, [&]{
    if (glfwInit() == GLFW_FALSE)
      std::exit(1);

    int major, minor, revision;
    glfwGetVersion(&major, &minor, &revision);
    YAGI_LOG_DEBUG("Initialized GLFW v{}.{}.{}", major, minor, revision);
  });
}

Oracle::~Oracle() {
  glfwTerminate();
  YAGI_LOG_DEBUG("Terminated GLFW");
}

} // namespace yagi
