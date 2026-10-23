#pragma once
// JobContext: what a module may use in beginJob(): the sample, its own histogram
// directory, and the reader (to ask for extra branches such as trigger bits).
#include <TDirectory.h>
#include <TH1D.h>

#include <string>

#include "myf/NanoReader.h"
#include "myf/Sample.h"

namespace myf {

class JobContext {
 public:
  JobContext(const Sample& sample, TDirectory* dir, NanoReader& reader)
      : sample_(sample), dir_(dir), reader_(reader) {}

  const Sample& sample() const { return sample_; }
  bool isData() const { return sample_.isData; }

  // the histogram lives in this module's directory of the output file, which owns it
  TH1D* book1D(const std::string& name, const std::string& title, int nbins, double lo, double hi) {
    dir_->cd();
    auto* h = new TH1D(name.c_str(), title.c_str(), nbins, lo, hi);
    h->Sumw2();
    return h;
  }

  void requestTrigger(const std::string& path) { reader_.addTrigger(path); }

 private:
  const Sample& sample_;
  TDirectory* dir_;
  NanoReader& reader_;
};

}  // namespace myf
