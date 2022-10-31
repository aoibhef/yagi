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
  std::uint64_t start_{0};
  std::uint64_t last_{0};
  std::uint64_t dt_{0};
  std::uint64_t interval_{0};
  std::uint64_t acc_{0};
};

} // namespace yagi

#endif //YAGI_UTIL_TICKER_H
