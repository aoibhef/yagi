#ifndef YAGI_UTIL_TICKER_H
#define YAGI_UTIL_TICKER_H

#include <cstdint>

namespace yagi {

class Ticker {
public:
  Ticker(double interval = 0.0);

  std::uint64_t tick();

  double dt_nsec();
  double dt_usec();
  double dt_msec();
  double dt_sec();

private:
  double start_{0.0};
  double last_{0.0};
  double dt_{0.0};
  double interval_{0.0};
  double acc_{0.0};
};

} // namespace yagi

#endif //YAGI_UTIL_TICKER_H
