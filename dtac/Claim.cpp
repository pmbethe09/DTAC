#include "dtac/Claim.h"

#include <fstream>

using std::endl;
using std::ofstream;
using std::ostream;
using std::ostringstream;
using std::shared_ptr;
using std::string;
using std::vector;

namespace dtac {

/// Claim

Claim::Claim(const Claim& other) : tricks(other.tricks), toTake(other.toTake) {
  memcpy(round_of, other.round_of, sizeof(round_of));
  if (other.lostTrick) {
    lostTrick = shared_ptr<LoseATrick>(new LoseATrick(*other.lostTrick));
  }
}

// verify that claim holds
bool Claim::verify(const CombinedDefender& cd, SUIT trumps) const {
  Claim dummy(toTake);  // use to keep track of rounds, call cd.beats

  for (int round = 0; round < toTake; ++round) {
    const ClaimTrick& trick = tricks[round];
    if (!trick.beats(dummy, cd, trumps)) return false;
    dummy.round_of[(int)trick.lead.suit] += 1;
  }
  return true;
}

// output in dot format
void Claim::printDot(ostream& os) const {
  int label = 0;
  os << "digraph \"source tree\" {"
     << "\n overlap=scale;\n size=\"8,10\";"
     << "\n fontsize=\"16\";\n fontname=\"Helvetica\";"
     << "\n clusterrank=\"local\";\n edge [arrowsize=0.5]" << endl;
  //  os << " 0 [label=\"Start\"];" << endl;
  label = 0;

  printDot(os, -1, label, string(""));

  os << "};" << endl;
}

class DotHelper {
 public:
  static void printDotTrick(ostream& os, const ClaimTrick& ct, int label) {
    os << label << " [label=\"" << suit2Char(ct.lead.suit)
       << rank2Char(ct.lead.rank) << " " << suit2Char(ct.follow.suit)
       << rank2Char(ct.follow.rank) << "\"];" << endl;
  }

  static void printDotArc(ostream& os, int parent, int label,
                          const string& lbstr) {
    os << (parent) << " -> " << (label) << " "
       << lbstr;  // label from parent to us
    os << ";" << endl;
  }

  static void printNode(ostream& os, int label, const string& lstr) {
    static string emptys;
    printNode(os, label, lstr, emptys);
  }
  static void printNode(ostream& os, int label, const string& lstr,
                        const string& shape) {
    os << label << " [label=\"" << lstr << "\"" << shape << "];" << endl;
  }

  static void printLoss(ostream& os, const vector<shared_ptr<Claim> >& eachSuit,
                        const string& lossLabel, int parent, int& label) {
    int first = -1;

    for (int s = 0; s < NUM_SUITS; ++s) {
      if (eachSuit[s]) {
        if (first == -1) {
          first = ++label;
          printNode(os, first, "Opps Lead", " shape=\"box\"");
          printDotArc(os, parent, first, lossLabel);
        }
        ostringstream rtnSuit;
        rtnSuit << " [label=\"" << suit2Char((SUIT)s) << "\"]";

        eachSuit[s]->printDot(os, first, label, rtnSuit.str());
      }
    }
  }
};  // class DotHelper

void Claim::printDot(const string& fname) const {
  ofstream ofile(fname.c_str());
  printDot(ofile);
  ofile.close();
}

void Claim::printDot(ostream& os, int parent, int& label,
                     const string& lbstr) const {
  if (tricks.size() == 0 && !lostTrick) {
    if (label != 0) {
      DotHelper::printDotArc(os, parent, ++label, lbstr);
      DotHelper::printNode(os, label, "Sucess", " shape=\"box\"");
    }
    return;
  }

  for (int i = 0; i < tricks.size(); ++i) {
    const ClaimTrick& ct = tricks[i];
    int oldlabel = label++;
    if (i == 0) {
      if (parent != -1) DotHelper::printDotArc(os, parent, label, lbstr);
    } else
      DotHelper::printDotArc(os, oldlabel, label, "");
    DotHelper::printDotTrick(os, ct, label);
  }

  if (tricks.size() > 0) {
    parent = label;  // use last trick as branch, else use parent
  }

  //  ASSERT_MSG(lostTricks.size() == 1, "exactly one Loss trick");
  //  for (int l=0; l < lostTricks.size(); ++l) {
  if (lostTrick) {
    const LoseATrick& lTrick = *lostTrick;  //[l];

    if (lTrick.theyDuck) {
      lTrick.theyDuck->printDot(os, parent, label, " [label=\"Duck\"]");
      //      parent = label;
    }

    DotHelper::printLoss(os, lTrick.ruffSuitClaims, " [label=\"Ruff in\"]",
                         parent, label);
    DotHelper::printLoss(os, lTrick.winSuitClaims, " [label=\"Outrank\"]",
                         parent, label);

  }  // end lose section
}

void Claim::print(ostream& os, const string& indent) const {
  if (tricks.size() == 0 && !lostTrick) {  // lostTricks.size() == 0) {
    os << "no claim" << endl;
    return;
  }
  for (int i = 0; i < tricks.size(); ++i) {
    const ClaimTrick& ct = tricks[i];
    os << indent << suit2Char(ct.lead.suit) << " " << rank2Char(ct.lead.rank)
       << " " << suit2Char(ct.follow.suit) << " " << rank2Char(ct.follow.rank)
       << " " << endl;
  }

#if FIXME
  //  for (int l=0; l < lostTricks.size(); ++l) {
  if (lostTrick) {
    //  const LoseATrick & lTrick = lostTricks[l];
    const LoseATrick& lTrick = *lostTrick;
    os << indent << "lose in round=" << lTrick.round << endl;
    string newind = indent + "  ";
    if (lTrick.theyDuck) {
      cout << newind << "Duck::" << endl;
      lTrick.theyDuck->print(os, newind);
    }

    cout << newind << "Win::" << endl;
    for (int s = 0; s < NUM_SUITS; ++s) {
      if (lTrick.suitClaims[s]) {
        cout << newind << suit2Char((SUIT)s) << "::" << endl;
        lTrick.suitClaims[s]->print(os, newind + "  ");
      }
    }
  }
#endif
}

}  // namespace dtac
