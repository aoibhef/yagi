#ifndef YAGI_APP_APPLICATION_H
#define YAGI_APP_APPLICATION_H

#include "yagi/core/window.h"
#include <memory>

namespace yagi {

class Application {
public:
  std::unique_ptr<Window> window{nullptr};
  std::unique_ptr<Context> ctx{nullptr};

  Application();

  virtual void initialize();
  virtual void deinitialize();
  virtual void update(double dt);
  virtual void draw();

private:
};

} // namespace yagi

#endif //YAGI_APP_APPLICATION_H
