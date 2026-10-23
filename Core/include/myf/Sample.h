#pragma once
#include <string>
#include <vector>

namespace myf {

struct Sample {
  std::string name;
  bool isData = false;
  double xsec = 0;  // cross section in pb, MC only
  std::vector<std::string> files;
};

}  // namespace myf
