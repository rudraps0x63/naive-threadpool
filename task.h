#pragma once

#include <functional>
#include <stdexcept>
#include <type_traits>

template<typename R, typename... Ts>
class Task {
public:
  Task() = delete;

  template<class Callable>
  Task(Callable&& callable) {
    static_assert(
      std::is_invocable<
        typename std::remove_reference<
          typename std::remove_pointer<Callable>::type
        >::type
      >::value
      ||
      std::is_function<
        typename std::remove_reference<
          typename std::remove_pointer<Callable>::type
        >::type
      >::value,
      "Passed callable cannot be invoked!"
    );

    this->set_task(std::forward<Callable>(callable));
  }

  template<typename... Args>
  R execute(Args&&... ts) {
    if constexpr (!std::is_same<R, void>::value)
      return func_(ts...);

    func_(ts...);
  }

private:
  template<class Callable>
  void set_task(Callable&& callable) {
    func_ = std::move(callable);
  }

  std::function<R(Ts...)> func_;
};

