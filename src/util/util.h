#include <iostream>
#include <ostream>
#include <stdio.h>

#define ASSERT_MESSAGE(condition, message) \
if (!(condition)) {                        \
  std::string m = std::move(message);      \
  std::cerr << m;                          \
  assert((condition));                     \
}                                          \

#define ASSERT(condition) ASSERT_MESSAGE(condition, "")

#define TEST_ASSERT(x) ASSERT_MESSAGE((x()) == 0, "** Testcase failure! **\n-> [" #x "]:\n")

namespace util {

  #define LOG(x)                                                    \
  printf("[%s]:%s:%d:\n", __FILE__, __PRETTY_FUNCTION__, __LINE__); \
  std::cout << (x)                                                  \

  enum class LogLevel {
    Debug, Warn, Fatal
  };

  inline std::ostream& log(std::ostream& ostream)
  {
    ostream.operator<<("");

    return ostream;
  }

  template<typename... Ts>
  inline std::ostream& operator<<(std::ostream& ostream, const Ts&... ts)
  {
    ostream.operator<<(ts...);

    return ostream;
  }

}

