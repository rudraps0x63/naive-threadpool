#include "threadpool/threadsafe-queue.h"
#include "util/util.h"

#include <cassert>
#include <exception>
#include <functional>

int test_queue_singlethreaded()
{
  int elems = 25;
  threadpool::ThreadsafeQueue<int> queue;

  for (int i = 0; i < elems; ++i) {
    ASSERT(queue.enqueue(i + 1));
  }

  ASSERT(queue.empty() != false);
  ASSERT(queue.size() == elems);

  while (!queue.empty()) {
    auto optional = queue.dequeue();

    ASSERT(optional.has_value());

    auto _ = optional.value();
  }

  return 0;
}

int test_queue_multithreaded()
{
  return 5;
}

int main()
{
  TEST_ASSERT(test_queue_singlethreaded);
#if defined(BUILD_ALL_UNIT_TESTS)
  TEST_ASSERT(test_queue_multithreaded);
#endif
}

