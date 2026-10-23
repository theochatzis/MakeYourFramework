#include "myf/NanoReader.h"

#include <stdexcept>

#include "myf/Event.h"

namespace myf {

NanoReader::NanoReader(TChain& chain, bool isData) : reader_(&chain), isData_(isData) {
  if (!isData_) genWeight_ = std::make_unique<TTreeReaderValue<Float_t>>(reader_, "genWeight");
}

void NanoReader::addTrigger(const std::string& path) {
  if (!triggers_.count(path)) triggers_[path] = std::make_unique<TTreeReaderValue<Bool_t>>(reader_, path.c_str());
}

bool NanoReader::next() {
  if (!reader_.Next()) {
    if (reader_.GetEntryStatus() != TTreeReader::kEntryBeyondEnd)
      throw std::runtime_error("[myf] error reading the input (missing or wrongly typed branch?)");
    return false;
  }
  return true;
}

void NanoReader::fill(Event& event) {
  event.run = *run_;
  event.lumi = *lumi_;
  event.number = *event_;
  event.isData = isData_;
  event.weight = isData_ ? 1.0 : **genWeight_;

  for (size_t i = 0; i < muonPt_.GetSize(); ++i) {
    Muon mu;
    mu.pt = muonPt_[i];
    mu.eta = muonEta_[i];
    mu.phi = muonPhi_[i];
    mu.mass = muonMass_[i];
    mu.charge = muonCharge_[i];
    mu.relIso = muonIso_[i];
    mu.tightId = muonTightId_[i];
    mu.mediumId = muonMediumId_[i];
    event.muons.push_back(mu);
  }

  for (size_t i = 0; i < jetPt_.GetSize(); ++i) {
    Jet jet;
    jet.pt = jetPt_[i];
    jet.eta = jetEta_[i];
    jet.phi = jetPhi_[i];
    jet.mass = jetMass_[i];
    jet.jetId = jetId_[i];
    jet.btag = jetBtag_[i];
    event.jets.push_back(jet);
  }

  for (auto& [path, value] : triggers_) event.triggers[path] = **value;
}

}  // namespace myf
