#include "dtac/HandCache1.h"

namespace dtac {

HandCache1::HandCache1() : stats(new CStats) {}

// for fwd compat
HandCache1::HandCache1(const Hand& l, const Hand& follow) : stats(new CStats) {}

}  // namespace dtac
