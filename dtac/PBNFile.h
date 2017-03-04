#ifndef _PBN_FILE_HPP
#define _PBN_FILE_HPP

/** represents a logical file with all the PBN Deals inside it */
#include <memory>
#include <string>
#include <vector>

#include "dtac/PBNDeal.h"

class PBNFile {
 public:
  explicit PBNFile(const std::string& fname) : filename(fname) {}
  PBNFile() {}
  ~PBNFile();

  static PBNFile* loadFromFile(const std::string& file_name);

  std::string filename;
  std::vector<PBNDeal*> data;
};

#endif /* HPP */
