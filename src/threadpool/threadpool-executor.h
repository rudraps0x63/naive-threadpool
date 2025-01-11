#pragma once

#include "task.h"
#include "broker.h"
#include "threadsafe-queue.h"
#include "threadpool.h"

namespace threadpool {

class ThreadpoolExecutor {
public:
  ThreadpoolExecutor(const ThreadpoolExecutor&) = delete;
  ThreadpoolExecutor(ThreadpoolExecutor&&) = delete;

  ThreadpoolExecutor& operator=(const ThreadpoolExecutor&) = delete;
  ThreadpoolExecutor& operator=(ThreadpoolExecutor&&) = delete;

  static ThreadpoolExecutor& new_with_size(std::size_t size);
  static ThreadpoolExecutor& instance();

  bool submit_task(Task& task);
  bool submit_task(Task&& task);

private:
  explicit ThreadpoolExecutor(std::size_t size);

private:
  Broker broker_;
  ThreadsafeQueue<std::shared_ptr<Task>> queue_;
  Threadpool pool_;
};

} // namespace threadpool

