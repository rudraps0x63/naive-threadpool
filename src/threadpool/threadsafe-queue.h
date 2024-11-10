#pragma once

#include "task.h"

#include <queue>
#include <mutex>
#include <thread>
#include <optional>

namespace threadpool {

const constexpr std::size_t DEFAULT_MAX_QUEUE_SIZE = 1000;

template<typename T>
class ThreadsafeQueue {
public:
  ThreadsafeQueue()
    : ThreadsafeQueue(DEFAULT_MAX_QUEUE_SIZE)
  {
  }

  explicit ThreadsafeQueue(std::size_t size)
    : max_queue_size_(size)
  {
  }

  bool enqueue(const T& t)
  {
    if (q_.size() == max_queue_size_)
      return false;

    {
      std::scoped_lock<std::mutex> sl(mu_);

      q_.push(t);
    }

    return true;
  }

  bool enqueue(T&& t)
  {
    {
      std::scoped_lock<std::mutex> sl(mu_);

      if (q_.size() == max_queue_size_)
        return false;

      q_.push(std::move(t));
    }

    return true;
  }

  std::optional<T> dequeue()
  {
    std::optional<T> result;

    {
      std::scoped_lock<std::mutex> sl(mu_);

      if (!q_.empty()) {
        result = std::make_optional<T>(std::move(q_.front()));
        q_.pop();
      }
    }

    return result;
  }

  inline bool empty() const
  {
    return this->size();
  }

  inline std::size_t size() const
  {
    return q_.size();
  }

private:
  std::mutex mu_;
  std::queue<T> q_;
  std::size_t max_queue_size_;
};

} // namespace threadpool
