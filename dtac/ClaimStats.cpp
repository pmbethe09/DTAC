#include <iostream>
#include <memory>
#include <sstream>

#include "dtac/ClaimData.hpp"
#include "dtac/PBNDeal.hpp"
#include "dtac/PBNFile.hpp"
#include "dtac/Trick.hpp"

using namespace dtac;

using std::cout;
using std::endl;
using std::shared_ptr;
using std::vector;

#define PBNFileSP std::shared_ptr<PBNFile>

int main(int argc, char **argv) {
  vector<shared_ptr<PBNFile> > allFiles;

  if (argc <= 1) {
    shared_ptr<PBNFile> file(PBNFile::loadFromFile("/dev/fd/0"));
    allFiles.push_back(file);
    cout << "read " << file->data.size() << " deals." << endl;
  } else {  // read names from cmd line
    for (int i = 1; i < argc; ++i)
      allFiles.push_back(shared_ptr<PBNFile>(PBNFile::loadFromFile(argv[i])));
  }

  ClaimData rd;
  int ndeals = 0, badData = 0;

  for (int f = 0; f < allFiles.size(); ++f) {
    const PBNFile &file = *allFiles[f];
    for (int i = 0, size = file.data.size(); i < size; ++i) {
      const PBNDeal *deal = file.data[i];
      try {
        rd.calcClaim(*deal);
      } catch (bridge_exception &be) {
        ++badData;
      }
    }
    ndeals += file.data.size();
  }

  cout << "deals= " << ndeals
       << ", deals w lead ClaimAll  =" << rd.onLead.samples << " ClAll ";
  rd.onLead.printAvgStd(cout);
  cout << endl;
  cout << " deals w opps but ClaimAll  =" << rd.oppsLead.samples
       << " opps ClAll ";
  rd.oppsLead.printAvgStd(cout);
  cout << endl;
  cout << "Bad data:" << badData << endl;
}
