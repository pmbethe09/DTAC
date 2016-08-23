#ifndef _BRIDGE_Auction_INCLUDED
#define _BRIDGE_Auction_INCLUDED

#include <vector>

#include "dtac/Bid.hpp"
#include "dtac/Constants.hpp"

namespace dtac {

class Auction {
  DIRECTION dealer;
  DIRECTION nextBidder;
  bool constructiveAuction;  // for now, means all passes by opps
  std::vector<Bid> bids;     // TODO currently only constructive
 public:
  explicit Auction(DIRECTION _dealer)
      : dealer(_dealer), nextBidder(dealer), constructiveAuction(true) {}

  DIRECTION nextToBid() const;
  void makeBid(DIRECTION bidder, Bid bid);
  bool isConstructive() const;  // return true if non-competitive auction
};

}  // namespace dtac

#endif  // hpp
