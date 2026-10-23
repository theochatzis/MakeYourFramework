#pragma once
// NanoReader: the only place that knows NanoAOD branch names.
// It translates the flat branches (Muon_pt[i], Muon_eta[i], ...) into Event objects.
// Branch types are those of NanoAODv9 (Run 2 UL).
#include <TChain.h>
#include <TTreeReader.h>
#include <TTreeReaderArray.h>
#include <TTreeReaderValue.h>

#include <map>
#include <memory>
#include <string>

namespace myf {

class Event;

class NanoReader {
 public:
  NanoReader(TChain& chain, bool isData);

  void addTrigger(const std::string& path);
  bool next();  // move to the next entry, false at the end
  void fill(Event& event);

 private:
  TTreeReader reader_;
  bool isData_;

  TTreeReaderValue<UInt_t> run_{reader_, "run"};
  TTreeReaderValue<UInt_t> lumi_{reader_, "luminosityBlock"};
  TTreeReaderValue<ULong64_t> event_{reader_, "event"};
  std::unique_ptr<TTreeReaderValue<Float_t>> genWeight_;  // MC only

  TTreeReaderArray<Float_t> muonPt_{reader_, "Muon_pt"};
  TTreeReaderArray<Float_t> muonEta_{reader_, "Muon_eta"};
  TTreeReaderArray<Float_t> muonPhi_{reader_, "Muon_phi"};
  TTreeReaderArray<Float_t> muonMass_{reader_, "Muon_mass"};
  TTreeReaderArray<Int_t> muonCharge_{reader_, "Muon_charge"};
  TTreeReaderArray<Float_t> muonIso_{reader_, "Muon_pfRelIso04_all"};
  TTreeReaderArray<Bool_t> muonTightId_{reader_, "Muon_tightId"};
  TTreeReaderArray<Bool_t> muonMediumId_{reader_, "Muon_mediumId"};

  TTreeReaderArray<Float_t> jetPt_{reader_, "Jet_pt"};
  TTreeReaderArray<Float_t> jetEta_{reader_, "Jet_eta"};
  TTreeReaderArray<Float_t> jetPhi_{reader_, "Jet_phi"};
  TTreeReaderArray<Float_t> jetMass_{reader_, "Jet_mass"};
  TTreeReaderArray<Int_t> jetId_{reader_, "Jet_jetId"};
  TTreeReaderArray<Float_t> jetBtag_{reader_, "Jet_btagDeepFlavB"};

  std::map<std::string, std::unique_ptr<TTreeReaderValue<Bool_t>>> triggers_;
};

}  // namespace myf
