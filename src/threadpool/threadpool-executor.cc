#include "threadpool-executor.h"
#include <memory>

#define DLL_SOURCE

namespace threadpool {

const constexpr std::size_t DEFAULT_NB_THREADS = 10;

ThreadpoolExecutor& ThreadpoolExecutor::new_with_size(std::size_t size)
{
  static ThreadpoolExecutor executor(size);

  return executor;
}

ThreadpoolExecutor& ThreadpoolExecutor::instance()
{
  return new_with_size(DEFAULT_NB_THREADS);
}

bool ThreadpoolExecutor::submit_task(Task& task)
{
  std::shared_ptr<Task> ptr(task.clone());
  bool res = queue_.enqueue(std::move(ptr));

  if (res) {
    std::unique_lock<std::mutex> ul(broker_.mu);
    broker_.cv.notify_all();
  }

  return res;
}

bool ThreadpoolExecutor::submit_task(Task&& task)
{
  return this->submit_task(task);
}

ThreadpoolExecutor::ThreadpoolExecutor(std::size_t size)
  : pool_(size, broker_, queue_)
{
}

} // namespace threadpool

