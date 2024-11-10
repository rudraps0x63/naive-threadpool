#pragma once

#include "task.h"
#include "broker.h"
#include "threadsafe-queue.h"

#include <cstddef>
#include <memory>
#include <mutex>
#include <vector>
#include <queue>
#include <thread>
#include <condition_variable>

namespace threadpool {

const constexpr std::size_t NB_THREADS_LIMIT = 25;

class Threadpool {
public:
  Threadpool() = delete;

  explicit Threadpool(std::size_t nb_threads,
                      Broker& broker,
                      ThreadsafeQueue<std::shared_ptr<Task>>& task_queue)
    : nb_threads_(std::min(nb_threads, NB_THREADS_LIMIT))
    , broker_(broker)
  {
    for (std::size_t i = 0; i < nb_threads_; ++i) {
      active_threads_list_.push_back({ });
      active_threads_list_.back().thr = std::thread(
        [&self = active_threads_list_.back(), &broker, &task_queue] () {
          while (true) {
            {
              std::unique_lock<std::mutex> ul{ broker.mu };

              self.status = ThreadStatus::Waiting;
              broker.cv.wait(ul, [&] () {
                return !task_queue.empty()
                       || self.status == ThreadStatus::ForceStopped;
              });
            }

            if (self.status == ThreadStatus::ForceStopped)
              return;

            const auto optional = task_queue.dequeue();

            if (!optional.has_value())
              continue;

            self.status = ThreadStatus::Executing;

            optional.value()->execute();
          }
        });
    }
  }

  virtual ~Threadpool()
  {
    for (auto& thread : active_threads_list_) {
      if (thread.thr.joinable()) {
        if (thread.status == ThreadStatus::Waiting) {
          {
            std::unique_lock<std::mutex> ul(broker_.mu);
            thread.status = ThreadStatus::ForceStopped;
            broker_.cv.notify_all();
          }
          thread.thr.join();
        }
        else { // Don't wait to join, detach the thread for OS cleanup
          thread.thr.detach();
        }
      }
    }
  }

private:
  enum class ThreadStatus {
    Waiting,
    Executing,
    ForceStopped
  };

  struct Thread {
    std::thread thr;
    ThreadStatus status = ThreadStatus::Waiting;
  };

  std::size_t nb_threads_ = 0;
  Broker& broker_;
  std::vector<Thread> active_threads_list_;
};

} // namespace threadpool

