#include "myf/Bookkeeper.h"

#include <TH1D.h>
#include <TNamed.h>

#include <cstdio>
#include <sstream>

namespace myf {

Bookkeeper::Bookkeeper(const std::vector<std::string>& moduleLabels)
    : labels_(moduleLabels), passN_(moduleLabels.size(), 0), passW_(moduleLabels.size(), 0.) {}

void Bookkeeper::countRead(double weight) {
  ++nRead_;
  sumw_ += weight;
  sumw2_ += weight * weight;
}

void Bookkeeper::countPass(size_t moduleIndex, double weight) {
  ++passN_[moduleIndex];
  passW_[moduleIndex] += weight;
}

void Bookkeeper::addLumiSection(unsigned run, unsigned lumi) { lumis_[run].insert(lumi); }

void Bookkeeper::print() const {
  std::printf("[myf] %-20s %12s %10s\n", "cutflow", "events", "eff.");
  std::printf("[myf] %-20s %12lld %10s\n", "read", nRead_, "");
  for (size_t i = 0; i < labels_.size(); ++i)
    std::printf("[myf] %-20s %12lld %9.1f%%\n", labels_[i].c_str(), passN_[i],
                nRead_ ? 100. * passN_[i] / nRead_ : 0.);
}

void Bookkeeper::write(TDirectory* dir, const Sample& sample, const std::string& config) const {
  dir->cd();

  // counts and cutflows are histograms with labelled bins: hadd adds them correctly
  TH1D counts("counts", "bookkeeping", 4, 0, 4);
  const char* names[] = {"eventsRead", "sumGenWeight", "sumGenWeight2", "runsGenEventSumw"};
  double values[] = {double(nRead_), sumw_, sumw2_, runsSumw_};
  for (int i = 0; i < 4; ++i) {
    counts.GetXaxis()->SetBinLabel(i + 1, names[i]);
    counts.SetBinContent(i + 1, values[i]);
  }
  counts.Write();

  int n = labels_.size() + 1;
  TH1D cutflow("cutflow", "events passing each module", n, 0, n);
  TH1D cutflowW("cutflow_w", "sum of weights passing each module", n, 0, n);
  cutflow.GetXaxis()->SetBinLabel(1, "read");
  cutflowW.GetXaxis()->SetBinLabel(1, "read");
  cutflow.SetBinContent(1, nRead_);
  cutflowW.SetBinContent(1, sumw_);
  for (size_t i = 0; i < labels_.size(); ++i) {
    cutflow.GetXaxis()->SetBinLabel(i + 2, labels_[i].c_str());
    cutflowW.GetXaxis()->SetBinLabel(i + 2, labels_[i].c_str());
    cutflow.SetBinContent(i + 2, passN_[i]);
    cutflowW.SetBinContent(i + 2, passW_[i]);
  }
  cutflow.Write();
  cutflowW.Write();

  // provenance: what was run, on what
  std::ostringstream info;
  info << "{\"name\": \"" << sample.name << "\", \"isData\": " << (sample.isData ? "true" : "false")
       << ", \"xsec\": " << sample.xsec << ", \"files\": [";
  for (size_t i = 0; i < sample.files.size(); ++i) info << (i ? ", " : "") << "\"" << sample.files[i] << "\"";
  info << "]}";
  TNamed("sample", info.str().c_str()).Write();
  TNamed("config", config.c_str()).Write();

  // the processed luminosity sections, in the CMS JSON format (input for brilcalc)
  if (sample.isData) {
    std::ostringstream json;
    json << "{";
    bool firstRun = true;
    for (const auto& [run, lumis] : lumis_) {
      json << (firstRun ? "" : ", ") << "\"" << run << "\": [";
      firstRun = false;
      bool firstRange = true;
      // compress consecutive sections into [first, last] ranges
      auto it = lumis.begin();
      while (it != lumis.end()) {
        unsigned first = *it, last = *it;
        while (++it != lumis.end() && *it == last + 1) last = *it;
        json << (firstRange ? "" : ", ") << "[" << first << ", " << last << "]";
        firstRange = false;
      }
      json << "]";
    }
    json << "}";
    TNamed("lumis", json.str().c_str()).Write();
  }
}

}  // namespace myf
