#pragma once

#include "enet/enet.h"

class IntervalTimer {
 public:
  IntervalTimer(uint32_t interval);

  bool IsReset();

 private:
  uint32_t curr_time_;
  uint32_t prev_time_;

  uint32_t interval_;

  bool started_ = false;
};
