#pragma once

#include <exception>
#include <memory>
#include <functional>
#include <stdexcept>
#include <type_traits>
#include <iostream>
#include <optional>
#include <any>

namespace threadpool {

/**
 * TODO:
 * Consider using `Callable` inplace of `Func`
 * (e.g., `DefaultCallableType` vs. `DefaultFuncType`)
 */

namespace internal {

using DefaultFuncType = std::function<void()>;

template<typename FuncType>
using FuncReturnType = typename std::invoke_result<FuncType>::type;

template<typename T>
concept type_is_void = std::is_same<T, void>::value;

} // namespace internal

class Task {
public:
  [[nodiscard(__FILE__ ": Called method returns unmanaged memory!")]]
  virtual Task* clone() = 0;

  virtual void execute() = 0;

  virtual std::optional<std::any> result() = 0;

  virtual ~Task() = default;
};

template<typename FuncType = internal::DefaultFuncType>
  /* ReturnType, if not `void`, needs to be `CopyConstructible` for `std::any` */
  requires (std::copy_constructible<internal::FuncReturnType<FuncType>>
            || internal::type_is_void<internal::FuncReturnType<FuncType>>)
class DefaultTask : public Task {
public:
  using ReturnType = internal::FuncReturnType<FuncType>;

  DefaultTask() = delete;

  DefaultTask(const DefaultTask&) = default;
  DefaultTask(DefaultTask&&) = default;

  explicit DefaultTask(FuncType& callable)
    : func_(std::make_shared<FuncType>(callable))
  {
  }

  explicit DefaultTask(FuncType&& callable)
    : func_(std::make_shared<FuncType>(std::move(callable)))
  {
  }

  [[nodiscard(__FILE__ ": Called method returns unmanaged memory!")]]
  virtual DefaultTask* clone() override
  {
    return new DefaultTask(*this);
  }

  virtual void execute() override
  {
    try {
      if constexpr (!internal::type_is_void<ReturnType>) {
        ReturnType retval { std::invoke(*func_.get()) };
        retval_ = std::make_optional<std::any>(std::move(retval));
      }
      else {
        std::invoke(*func_.get());
      }
    }
    catch(std::exception& e) {
      std::cerr << __func__ << ":" << __LINE__ << " -- \n" << e.what() << '\n';
    }
  }

  virtual std::optional<std::any> result() override
  {
    return retval_;
  }

  virtual ~DefaultTask() = default;

private:
  std::optional<std::any> retval_;
  std::shared_ptr<FuncType> func_;
};

} // namespace Threadpool
