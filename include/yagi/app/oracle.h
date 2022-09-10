#ifndef YAGI_APP_ORACLE_H
#define YAGI_APP_ORACLE_H

#include "yagi/app/application.h"
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include <memory>
#include <mutex>
#include <type_traits>

namespace yagi {

class Oracle {
public:
  Oracle();
  ~Oracle();

  template <class T> requires std::derived_from<T, Application>
  void run_application();

private:
  std::unique_ptr<Application> application_{nullptr};

  static std::once_flag initialized_glfw_;
};

template <class T> requires std::derived_from<T, Application>
void Oracle::run_application() {
  application_ = std::make_unique<T>();

  application_->initialize();

  do {
    glfwPollEvents();
    msg::Bus::poll_all();
  } while (!application_->window->should_close());
}

} // namespace yagi

#endif //YAGI_APP_ORACLE_H