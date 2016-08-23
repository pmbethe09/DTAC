#ifndef _BRIDGE_MAKE_STRING_HPP
#define _BRIDGE_MAKE_STRING_HPP

#include <sstream>
#include <string>

namespace dtac {

class MakeString {
 public:
  std::stringstream stream;
  operator std::string() const { return stream.str(); }

  template <class T>
  MakeString& operator<<(T const& VAR) {
    stream << VAR;
    return *this;
  }
};
}  // namespace dtac {

#endif  // hpp
