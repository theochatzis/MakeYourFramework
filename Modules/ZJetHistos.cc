// ZJetHistos: control histograms of the Z candidate and the jets.
// python: process.add("ZJetHistos", "zjetHistos", z="zCandidate", jets="goodJets")
#include <TH1D.h>

#include "myf/ModuleFactory.h"
#include "myf/ZCandidate.h"

namespace myf {

class ZJetHistos : public Module {
 public:
  ZJetHistos(const std::string& label, const ParameterSet& params)
      : Module(label, params), z_(params.getString("z")), jets_(params.getString("jets")) {}

  void beginJob(JobContext& job) override {
    mass_ = job.book1D("mass", ";m_{#mu#mu} [GeV];Events", 60, 60, 120);
    zPt_ = job.book1D("zPt", ";p_{T}^{Z} [GeV];Events", 40, 0, 200);
    mu1Pt_ = job.book1D("mu1Pt", ";leading muon p_{T} [GeV];Events", 40, 0, 200);
    nJets_ = job.book1D("nJets", ";number of jets;Events", 6, -0.5, 5.5);
    jet1Pt_ = job.book1D("jet1Pt", ";leading jet p_{T} [GeV];Events", 40, 0, 400);
    dPhi_ = job.book1D("dPhiZJet", ";|#Delta#phi(Z, leading jet)|;Events", 32, 0, 3.2);
  }

  bool process(Event& event) override {
    const auto& z = event.get<ZCandidate>(z_);
    const auto& jets = event.get<std::vector<Jet>>(jets_);
    double w = event.weight;
    mass_->Fill(z.mass(), w);
    zPt_->Fill(z.pt(), w);
    mu1Pt_->Fill(z.mu1.pt, w);
    nJets_->Fill(jets.size(), w);
    if (!jets.empty()) {
      jet1Pt_->Fill(jets[0].pt, w);
      dPhi_->Fill(std::abs(deltaPhi(z.p4.Phi(), jets[0].phi)), w);
    }
    return true;
  }

 private:
  std::string z_, jets_;
  TH1D *mass_ = nullptr, *zPt_ = nullptr, *mu1Pt_ = nullptr, *nJets_ = nullptr, *jet1Pt_ = nullptr,
       *dPhi_ = nullptr;
};

MYF_REGISTER_MODULE(ZJetHistos, "histograms of the Z candidate `z` and the `jets`")

}  // namespace myf
