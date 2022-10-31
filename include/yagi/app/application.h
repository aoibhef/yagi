#ifndef YAGI_APP_APPLICATION_H
#define YAGI_APP_APPLICATION_H

#include "yagi/app/window.h"
#include <memory>

namespace yagi {

class Application {
public:
  std::unique_ptr<Window> window{nullptr};

  Application();

  virtual void initialize();
  virtual void update(double dt);
  virtual void draw();

private:
};

} // namespace yagi

#endif //YAGI_APP_APPLICATION_H
