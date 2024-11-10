#pragma once

#include "threadsafe-queue.h"

#include <thread>
#include <condition_variable>

namespace threadpool {

struct Broker {
public:
  std::mutex mu;
  std::condition_variable cv;
};

} // namespace threadpool
