#include "yagi/util/framecounter.h"

namespace yagi {

FrameCounter::FrameCounter() {
  start_time_ = time_nsec();
}

void FrameCounter::update() {
  timestamps_.emplace_back(time_nsec());
  if (timestamps_.size() == 1)
    dts_.emplace_back(static_cast<double>(timestamps_.back() - start_time_) / 1e9);
  else
    dts_.emplace_back(static_cast<double>(timestamps_.back() - timestamps_[timestamps_.size() - 2]) / 1e9);

  // Keep size >= 2 otherwise dt will give bad results on pauses longer than 1s
  while (timestamps_.size() > 2 && timestamps_.back() - timestamps_.front() > 1'000'000'000) {
    timestamps_.pop_front();
    dts_.pop_front();
  }

  averager_.update(static_cast<double>(timestamps_.size()));
  if (ticker_.tick())
    averager_.alpha = 2.0 / (1.0 + static_cast<double>(timestamps_.size()));
}

double FrameCounter::fps() const {
  return averager_.value();
}

double FrameCounter::dt() const {
  if (dts_.empty())
    return 0.0;

  return dts_.back();
}

std::vector<double> FrameCounter::dts_vec() const {
  return {dts_.begin(), dts_.end()};
}

} // namespace yagi
