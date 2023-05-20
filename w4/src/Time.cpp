#include "Time.hpp"

IntervalTimer::IntervalTimer(uint32_t interval) : interval_(interval) {
  prev_time_ = curr_time_ = enet_time_get();
}

bool IntervalTimer::IsReset() {
  curr_time_ = enet_time_get();
  if (curr_time_ - prev_time_ > interval_) {
    prev_time_ = curr_time_;
    return true;
  }
  return false;
}
