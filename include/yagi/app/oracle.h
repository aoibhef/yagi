#ifndef YAGI_APP_ORACLE_H
#define YAGI_APP_ORACLE_H

#include <mutex>

namespace yagi {

class Oracle {
public:
  Oracle();
  ~Oracle();

  void run_main_loop();

private:
  static std::once_flag initialized_glfw_;
};

} // namespace yagi

#endif //YAGI_APP_ORACLE_H
