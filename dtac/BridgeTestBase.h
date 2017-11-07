#ifndef _BRIDGE_TEST_BASE_INCLUDED
#define _BRIDGE_TEST_BASE_INCLUDED

#include "dtac/CombinedDefender.h"
#include "dtac/Hand.h"
#include "dtac/Play.h"

#include <memory>

namespace dtac {

class Claim;

#define ClaimSP std::shared_ptr<Claim>

class BridgeTestBase {
 public:
  static ClaimSP runOne(const char* lead, const char* follow, const char* dfnd,
                        SUIT trumps = NOTRUMP,
                        const CombinedDefender* constraints = NULL,
                        int tricks = -1);

  static ClaimSP runOne(const char* lead,
                        const char* follow, /* assume dfndr is all */
                        SUIT trumps = NOTRUMP,
                        const CombinedDefender* constraints = NULL,
                        int tricks = -1);

  static ClaimSP runOne(const Hand& lead, const Hand& follow, const Hand& dfndr,
                        SUIT trumps = NOTRUMP,
                        const CombinedDefender* constraints = NULL,
                        int tricks = -1, bool oppsLead = false,
                        SUIT oppsSuit = NOTRUMP);

  static void parseClaim(const char* tricks, Claim& c, SUIT trumps,
                         SUIT suitLead = NOTRUMP);

  /* find for two hands, third string is NOT a hand, but the CD string fmt */
  static ClaimSP runOneConstr(const char* lead, const char* follow,
                              SUIT trumps = NOTRUMP, const char* cdef = NULL,
                              int tricks = -1);

  static void workBack(const char* hands[3], SUIT trumps,
                       const CombinedDefender& baseCd, const Claim& claim);

  static void workBack(Hand leadin, Hand followin, Hand dfndr, SUIT trumps,
                       const CombinedDefender& baseCd, const Claim& claim,
                       SUIT leadTrickOne = NOTRUMP);

  static bool hasCard(const Hand& h, const Play& p);

  static void applyTricks(const int toApply, const Claim& claim,
                          bool& stillOnLead, Hand& lead, Hand& follow,
                          SUIT trumps, CombinedDefender& cd);
};

}  // namespace dtac

#endif /* HPP */
