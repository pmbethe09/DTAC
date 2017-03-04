#include "dtac/Auction.h"

namespace dtac {

DIRECTION Auction::nextToBid() const { return nextBidder; }

void Auction::makeBid(DIRECTION bidder, Bid bid) {
  ASSERT_MSG(bidder == nextBidder, "unexpected bidder");

  bids.push_back(bid);
  ++nextBidder;
}

bool Auction::isConstructive() const { return constructiveAuction; }

}  // namespace dtac
