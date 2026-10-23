#pragma once
// Bookkeeper: counts what went in and what came out, so that every output file can be
// normalised and checked later without rerunning.
#include <TDirectory.h>

#include <map>
#include <set>
#include <string>
#include <vector>

#include "myf/Sample.h"

namespace myf {

class Bookkeeper {
 public:
  explicit Bookkeeper(const std::vector<std::string>& moduleLabels);

  void countRead(double weight);                   // every event read from the input
  void countPass(size_t moduleIndex, double weight);  // events that passed module i
  void addLumiSection(unsigned run, unsigned lumi);   // data only
  void setRunsSumw(double sumw) { runsSumw_ = sumw; }

  void print() const;
  void write(TDirectory* dir, const Sample& sample, const std::string& config) const;

 private:
  std::vector<std::string> labels_;
  long long nRead_ = 0;
  double sumw_ = 0, sumw2_ = 0, runsSumw_ = 0;
  std::vector<long long> passN_;
  std::vector<double> passW_;
  std::map<unsigned, std::set<unsigned>> lumis_;
};

}  // namespace myf
