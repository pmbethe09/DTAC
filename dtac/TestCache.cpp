#include <gtest/gtest.h>

#include <string>

#include "dtac/HandCache2.h"

using std::cout;
using std::endl;
using std::string;
using namespace dtac;

TEST(NewCacheWhiteBoxTest, newCaching) {
#ifdef DEBUG
#ifdef __x86_64__
  cout << "64-bit" << endl;
#else
  cout << "32-bit" << endl;
#endif
#endif /* DEBUG */
  // add more
  const char* hands[] = {"AKQT762.9.A5.J5", "93.AK852.QJ984."};
  Hand lead(Hand::fromString(hands[0])), follow(Hand::fromString(hands[1]));
  HandCache2 hc2(lead, follow);

  Claim claim(12);

  HandCache2::cacheType::value_type vt =
      hc2.getCacheKey(lead, follow, claim, false);
  ASSERT_TRUE(hc2.notInCache(vt)) << "should be not yet cached";

  hc2.addToCache(vt);
  ASSERT_TRUE(!hc2.notInCache(vt)) << "should now be in cache";
  HandCache2::cacheType::value_type vt2 =
      hc2.getCacheKey(lead, follow, claim, true /*changed*/);
  ASSERT_TRUE(hc2.notInCache(vt2)) << "should be not be cached from opp hand";

  claim.round_of[0] = 5;
  HandCache2::cacheType::value_type vt3 =
      hc2.getCacheKey(lead, follow, claim /*changed*/, false);
  ASSERT_TRUE(hc2.notInCache(vt3))
      << "should be not be cached after 1 round of";

  Hand lead2(Hand::fromString(string(hands[0]).substr(1))),
      follow2(Hand::fromString(string(hands[1]).substr(1)));
  claim.round_of[0] = 0;  // reset
  HandCache2::cacheType::value_type vt4 =
      hc2.getCacheKey(lead2, follow2, claim, false);
  ASSERT_TRUE(hc2.notInCache(vt4)) << "should be not yet cached";
}

TEST(NewCacheWhiteBoxTest, testBadHit) {
  HandCache2::hash_key high32 = 26, low32 = 272761862,
                       testKey =
                           (high32 << 32) | low32;  //((uint64_t)111941911558);

  HandCache2 hc2(Hand::fromString("A86532.A6.K.85"),
                 Hand::fromString("KQJ.KT732.A8.J"));

  Hand follow(Hand::fromString(".T..J")), lead(Hand::fromString("8...8"));
  Claim claim(12);
  claim.round_of[0] = 0;
  claim.round_of[1] = 2;
  claim.round_of[2] = 4;
  claim.round_of[3] = 3;

  HandCache2::cacheType::value_type vt =
      hc2.getCacheKey(lead, follow, claim, true);

  ASSERT_EQ(vt.first, testKey)
      << "Get the right key: " << vt.first << " not " << testKey;

  claim.round_of[0] = 0;
  claim.round_of[1] = 2;
  claim.round_of[2] = 2;
  claim.round_of[3] = 5;
  HandCache2::cacheType::value_type vt2 =
      hc2.getCacheKey(lead, follow, claim, true);

  ASSERT_NE(vt.first, vt2.first) << "Get the different keys" << vt.first;
}
