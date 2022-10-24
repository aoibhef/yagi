#ifndef YAGI_APP_ORACLE_H
#define YAGI_APP_ORACLE_H

#include "yagi/core/application.h"
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
    application_->update_();

    application_->draw_();

    application_->post_draw_();

    Bus::poll_all();
  } while (!application_->window->should_close());

  application_->deinitialize();
  application_.reset(nullptr);
}

} // namespace yagi

#endif //YAGI_APP_ORACLE_H
