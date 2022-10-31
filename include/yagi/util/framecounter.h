#ifndef YAGI_UTIL_FRAMECOUNTER_H
#define YAGI_UTIL_FRAMECOUNTER_H

#include "yagi/util/averagers.h"
#include "yagi/util/ticker.h"
#include "yagi/util/time.h"
#include <cstdint>
#include <deque>

namespace yagi {

class FrameCounter {
public:
  FrameCounter();

  void update();

  double fps() const;
  double dt() const;

  std::vector<double> dts_vec() const;

private:
  std::uint64_t start_time_{};
  std::deque<std::uint64_t> timestamps_{};
  std::deque<double> dts_{};

  Ticker ticker_{0.5};
  EMA averager_{1.0};
};

} // namespace yagi

#endif //YAGI_UTIL_FRAMECOUNTER_H
