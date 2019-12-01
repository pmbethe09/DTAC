#include <sys/time.h>

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "dtac/ClaimData.h"
#include "dtac/PBNDeal.h"
#include "dtac/PBNFile.h"
#include "dtac/Trick.h"
#include "dtac/WorstSearch.h"

using std::cerr;
using std::cout;
using std::endl;
using std::exception;
using std::ios;
using std::lower_bound;
using std::resetiosflags;
using std::setprecision;
using std::vector;

using namespace dtac;

#define ClaimSP std::shared_ptr<Claim>
#define LoseATrickSP std::shared_ptr<LoseATrick>
#define PBNDealSP std::shared_ptr<PBNDeal>
#define PBNFileSP std::shared_ptr<PBNFile>

static ClaimSP search(const PBNDeal& deal, const ClaimData& cd, CStats& cstat,
                      BranchingStats& bstats, bool noLosers) {
  // apply already played
  const SingleClaim& oc = cd.lastHand;

  // cout << "Claim: (" << deal->contract << ") " << oc.declarer.toString() << "
  // " << oc.dummy.toString() << endl;
  Hand dfndr1 = oc.LHO, dfndr2 = oc.RHO;
  dfndr1 += dfndr2;
  DIRECTION decl = Hand::toDirection(deal.declarer[0]);

  Hand played = Hand::complement(oc.declarer, oc.dummy, dfndr1);

  bool stillDeclarer = oc.nextToLead == decl;

  WorstSearch wd(stillDeclarer ? oc.declarer : oc.dummy,
                 stillDeclarer ? oc.dummy : oc.declarer, played,
                 Hand::toSuit(deal.contract[1]), !noLosers);
  // bool oppsLead = ((int)oc.nextToLead & 1) == ((int)decl & 1); // ? use this?
  int toTake = oc.tricksTakenInclaim;
  ASSERT_MSG(!oc.tookAll || toTake == oc.tricksTakenInclaim,
             "take all matches");
  ASSERT_MSG(!oc.tookAll || (toTake == oc.declarer.size() &&
                             oc.declarer.size() == oc.dummy.size()),
             "Took All, but toTake("
                 << toTake << ") != oc.declarer.size(" << oc.declarer.size()
                 << ") != oc.dummy.size(" << oc.dummy.size() << ")");
  // ! what! adding toTake changes claimAlls!
  ClaimSP c = wd.search(toTake);  // , NULL, oppsLead);
  if (c) {
    cstat.mergeIn(*wd.getCacheStats());
    bstats.mergeIn(wd.bstats);
  }
  return c;
}

template <typename T>
class SumValuePass : public SumValue<T> {
 public:
  int passes;
  SumValuePass() : passes(0) {}
  void add(T val, bool ifPass) {
    SumValue<T>::add(val);
    if (ifPass) ++passes;
  }
};

int main(int argc, char** argv) {
  vector<PBNFileSP> allFiles;
  struct timeval start, stop;
  struct timezone dmy_tz;

  // exclusions?  1410 - seems to hang indefinitely... need to examine further.
  // int excludes[] = {201, 335, 416, 1026, 1042, 1177, 1410, 1411, 1413, 1995,
  // 2023, 2299, 2318, 2583, 2584, 2714, 3518};
  int excludes[] = {};
  const int esize = sizeof(excludes) / sizeof(int);
  int* excludes_end = excludes + esize;

  int argStart = 1;
  bool verbose = false;

  if (argc > 1) {
    if (!strcmp(argv[1], "-V")) {
      verbose = true;
      ++argStart;
    }
  }
  if (argc <= argStart) {
    PBNFileSP file(PBNFile::loadFromFile("/dev/fd/0"));
    allFiles.push_back(file);
  } else {
    for (int i = argStart; i < argc; ++i) {
      try {
        PBNFileSP file(PBNFile::loadFromFile(argv[i]));
        allFiles.push_back(file);
      } catch (exception& be) {
        cerr << "file: " << argv[i] << " contains bad data" << endl;
      }
    }
  }
  /*
  ofstream os("claimAlls.pbn");
  os << "% PBN 2.0" << endl << "% EXPORT" << endl << "%" << endl; // two leading
  burns
  */
  int deals = 0, badData = 0, failures = 0, nexcluded = 0;

  ClaimData cd;
  CStats cstat;
  BranchingStats bstats;

  SumValuePass<double> sv_all, sv_k /* claim to K*/, sv_k_loser;

  for (int f = 0; f < allFiles.size(); ++f) {
    PBNFileSP& file = allFiles[f];
    for (int i = 0, size = file->data.size(); i < size; ++i) {
      int* to_skip = lower_bound(excludes, excludes_end, (deals + i));
      if (to_skip < excludes_end && *to_skip == (deals + i)) {
        cout << "Skipping deal#=" << (deals + i) << endl;
        ++nexcluded;
        /*        if (verbose) {
          file->data[i]->printDeal(cerr);
          }*/
        continue;
      }
      //      if (deals+i == 3760) break; // for now!
      PBNDeal* deal = file->data[i];
      try {
        cd.calcClaim(*deal);
      } catch (bridge_exception& be) {
        cerr << "file: " << file->filename << " contains bad data" << endl;
        continue;
      }
      if (deal->badData) {
        ++badData;
        cerr << "file: " << file->filename
             << " contains bad data, deal#=" << (deals + i) << endl;
        continue;
      }
      try {
        bool noLoss = true;
        gettimeofday(&start, &dmy_tz);

        ClaimSP c = search(*deal, cd, cstat, bstats, true /*no losers*/);
        if (!c && !cd.lastHand.tookAll) {
          noLoss = false;
          c = search(*deal, cd, cstat, bstats, false /*allow losers*/);
        }

        gettimeofday(&stop, &dmy_tz);
        double diff = (stop.tv_sec - start.tv_sec) +
                      (stop.tv_usec - start.tv_usec) / 1000000.0;

        if (diff < 0.01) {
          cout << resetiosflags(ios::scientific) << setprecision(2);
        } else {
          cout << resetiosflags(ios::fixed) << setprecision(2);
        }

        // keep stats
        if (cd.lastHand.tookAll) {
          sv_all.add(diff, c.get());
        } else {
          if (noLoss) {
            sv_k.add(diff, c.get());
          } else {
            sv_k_loser.add(diff, c.get());
          }
        }

        if (verbose) {
          cout << "file=" << file->filename << " board#=" << (deal->board);
          cout << (c ? " Success:" : " Failure:") << " time=" << diff << endl;
        }
        /*
          } else { // not a claimAll
          if (printBadHands)
          file->data[i]->printDeal(cerr);
          }
        */
      } catch (bridge_exception& be) {
        cerr << "file=" << file->filename << " board#=" << (deal->board);
        cerr << be.what() << endl;
        ++failures;
        continue;
      }
    }  // end for
    deals += file->data.size();
  }

  deals -= nexcluded;

  cout << "Deals: " << (deals - badData) << ", claimAll: " << sv_all.samples
       << " WC finds it: " << sv_all.passes << " exception: " << failures
       << " bad data:" << badData << endl
       << "Claim All ";
  sv_all.printAvgStd(cout);
  cout << endl;

  cout << "WC(K) searches: " << (sv_k.samples + sv_k_loser.samples)
       << " finds it: " << sv_k.passes << endl;

  cout << "Claim first K ";

  sv_k.printAvgStd(cout);
  cout << endl;

  cout << "WC(K) with Loser: " << sv_k_loser.samples
       << " finds it: " << sv_k_loser.passes << endl;

  cout << "Claim w Loser ";

  sv_k_loser.printAvgStd(cout);
  cout << endl;

  cstat.print(cout);
  bstats.print(cout);

  //  os.close();
  return 0;
}
