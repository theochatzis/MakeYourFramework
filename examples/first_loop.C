// first_loop.C: the dimuon mass in 30 lines, no framework.  Run with:  root -l -b -q examples/first_loop.C
#include <iostream>

#include <TFile.h>
#include <TH1D.h>
#include <TTreeReader.h>
#include <TTreeReaderArray.h>
#include <Math/Vector4D.h>

void first_loop() {
  auto* file = TFile::Open("root://eospublic.cern.ch//eos/opendata/cms/Run2016H/SingleMuon/NANOAOD/"
                           "UL2016_MiniAODv2_NanoAODv9-v1/120000/61FC1E38-F75C-6B44-AD19-A9894155874E.root");
  TTreeReader reader("Events", file);
  TTreeReaderArray<Float_t> pt(reader, "Muon_pt"), eta(reader, "Muon_eta"), phi(reader, "Muon_phi"),
      mass(reader, "Muon_mass");
  TTreeReaderArray<Int_t> charge(reader, "Muon_charge");

  TH1D h("mass", ";m_{#mu#mu} [GeV];Events", 60, 60, 120);
  while (reader.Next()) {
    if (pt.GetSize() < 2) continue;                 // need two muons
    if (charge[0] * charge[1] > 0) continue;        // of opposite charge
    ROOT::Math::PtEtaPhiMVector mu1(pt[0], eta[0], phi[0], mass[0]);
    ROOT::Math::PtEtaPhiMVector mu2(pt[1], eta[1], phi[1], mass[1]);
    h.Fill((mu1 + mu2).M());
  }
  std::cout << "dimuon candidates: " << h.GetEntries() << std::endl;
  h.SaveAs("first_loop_mass.root");
}
