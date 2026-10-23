// JetSelector: jets passing kinematic and ID cuts, away from the selected muons.
// python: process.add("JetSelector", "goodJets", ptMin=30, etaMax=2.4, muons="goodMuons", dRMin=0.4)
#include <cmath>

#include "myf/ModuleFactory.h"

namespace myf {

class JetSelector : public Module {
 public:
  JetSelector(const std::string& label, const ParameterSet& params)
      : Module(label, params),
        ptMin_(params.getDouble("ptMin")),
        etaMax_(params.getDouble("etaMax")),
        dRMin_(params.getDouble("dRMin", 0.4)),
        muons_(params.getString("muons")) {}

  bool process(Event& event) override {
    const auto& muons = event.get<std::vector<Muon>>(muons_);
    std::vector<Jet> selected;
    for (const auto& jet : event.jets) {
      if (jet.pt < ptMin_ || std::abs(jet.eta) > etaMax_ || !jet.passesTightId()) continue;
      bool overlaps = false;
      for (const auto& mu : muons)
        if (deltaR(jet, mu) < dRMin_) overlaps = true;
      if (!overlaps) selected.push_back(jet);
    }
    event.put(label(), selected);
    return true;
  }

 private:
  double ptMin_, etaMax_, dRMin_;
  std::string muons_;
};

MYF_REGISTER_MODULE(JetSelector, "jets passing pt, eta, tight ID, cleaned against `muons`")

}  // namespace myf
