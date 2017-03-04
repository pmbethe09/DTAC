#ifndef _BRIDGE_ASSERTS_HPP
#define _BRIDGE_ASSERTS_HPP

#include <ostream>
#include <sstream>
#include <string>

#include "dtac/Exceptions.h"

namespace dtac {

#define ASSERT(x)                                            \
  if ((x))                                                   \
    ;                                                        \
  else {                                                     \
    throw bridge_exception(std::string("Failed assertion")); \
  }
#define ASSERT_MSG(x, msg)                                             \
  if ((x))                                                             \
    ;                                                                  \
  else {                                                               \
    std::ostringstream oss;                                            \
    oss << "(" << (char *)__FUNCTION__ << ":" << (int)__LINE__ << ")"; \
    oss << msg;                                                        \
    throw bridge_exception(oss.str());                                 \
  }
#ifdef DEBUG
#define ASSERT_DBG(x) ASSERT(x)
#define ASSERT_MSG_DBG(x, msg) ASSERT_MSG(x, msg)
#else
// turn off debug assertions under non debug
#define ASSERT_DBG(x)
#define ASSERT_MSG_DBG(x, msg)
#endif

}  // namespace dtac

#endif  //.h
