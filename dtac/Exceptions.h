#ifndef _BRIDGE_EXCEPTIONS_HPP
#define _BRIDGE_EXCEPTIONS_HPP

#include <stdexcept>
#include <string>

namespace dtac {

class bridge_exception : public std::logic_error {
 public:
  explicit bridge_exception(const std::string& msg) : logic_error(msg) {}
};

}  // namespace dtac

#endif  //.h
