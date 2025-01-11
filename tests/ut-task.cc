#include "threadpool/task.h"
#include "util/util.h"

#include <cassert>
#include <exception>
#include <functional>

int test_non_copy_constructible_type()
{

  struct A {
    A() = default;

    const int public_mem = 42;
  } a;

  std::any any { a };
  std::function<A(void)> ff = [] () -> A { return { }; };
  threadpool::DefaultTask<decltype(ff)> f(ff);

  f.execute();
  auto res = f.result();

  if (res.has_value()) {
    auto any = res.value();
    auto a = std::any_cast<A>(any);

    ASSERT(a.public_mem == A().public_mem);
  }

  return 0;
}

int test_void_return_type()
{
  using F = std::function<void()>;

  F f;
  int a = 19;
  int b = 23;
  int res = 0;
  f = [a, b, &res] () -> void { res = a + b; };
  threadpool::DefaultTask<F> task { f };
  threadpool::Task* t = &task;

  t->execute();
  auto result = t->result();

  ASSERT(!result.has_value());
  ASSERT(res == (a + b));

  return 0;
}

int test_int_return_type()
{
  using F = std::function<int()>;

  F f;
  {
    int a = 19;
    int b = 23;
    f = { [=] () { return a + b; } };
  }
  std::unique_ptr<threadpool::Task> t = std::make_unique<threadpool::DefaultTask<F>>(f);

  t->execute();
  auto result = t->result();

  ASSERT(result.has_value());

  try {
    auto executed_result = std::any_cast<F::result_type>(result.value());

    ASSERT(executed_result == f());
  }
  catch (std::exception& e) {
    std::cerr << e.what() << '\n';

    return -1;
  }

  return 0;
}

int main()
{
  TEST_ASSERT(test_void_return_type);
  TEST_ASSERT(test_int_return_type);
  TEST_ASSERT(test_non_copy_constructible_type);
}

