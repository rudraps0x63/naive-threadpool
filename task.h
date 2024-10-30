#pragma once

#include <memory>
#include <functional>
#include <stdexcept>
#include <type_traits>

namespace Threadpool {

template<typename R, typename... Ts>
class Task {
public:
  using Ftype = std::function<R(Ts...)>;

public:
  Task() = delete;

  template<class Callable>
  Task(Callable&& callable)
  {
    constexpr bool is_invocable
      = std::is_invocable<
        typename std::remove_reference<
          typename std::remove_pointer<Callable>::type
        >::type
      >::value;

    constexpr bool is_function
      = std::is_function<
        typename std::remove_reference<
          typename std::remove_pointer<Callable>::type
        >::type
      >::value;

    static_assert(is_invocable || is_function, "Passed callable neither invocable nor a function!");

    this->set_task(std::forward<Callable>(callable));
  }

  template<typename... Args>
  R execute(Args&&... ts)
  {
    using FType = decltype(*func_.get());

    Ftype f = *func_.get();

    if constexpr (!std::is_same<R, void>::value)
      return f(ts...);
    else
      f(ts...);
  }

private:
  template<class Callable>
  void set_task(Callable& callable)
  {
    func_ = std::make_unique<Ftype>(callable);
  }

  template<class Callable>
  void set_task(Callable&& callable)
  {
    func_ = std::move(callable);
  }

  std::shared_ptr<Ftype> func_;
};

} // namespace Threadpool
