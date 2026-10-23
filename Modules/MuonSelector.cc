// MuonSelector: muons passing kinematic, identification and isolation cuts, sorted by pt.
// python: process.add("MuonSelector", "goodMuons", ptMin=20, etaMax=2.4, id="tight", isoMax=0.15)
#include <algorithm>
#include <cmath>

#include "myf/ModuleFactory.h"

namespace myf {

class MuonSelector : public Module {
 public:
  MuonSelector(const std::string& label, const ParameterSet& params)
      : Module(label, params),
        ptMin_(params.getDouble("ptMin")),
        etaMax_(params.getDouble("etaMax")),
        isoMax_(params.getDouble("isoMax")),
        id_(params.getString("id", "tight")) {
    if (id_ != "tight" && id_ != "medium") throw std::runtime_error("[myf] " + label + ": id must be tight or medium");
  }

  bool process(Event& event) override {
    std::vector<Muon> selected;
    for (const auto& mu : event.muons) {
      if (mu.pt < ptMin_ || std::abs(mu.eta) > etaMax_) continue;
      if (id_ == "tight" ? !mu.tightId : !mu.mediumId) continue;
      if (mu.relIso > isoMax_) continue;
      selected.push_back(mu);
    }
    // NanoAOD collections are already pt-ordered, sort anyway: the next modules rely on it
    std::sort(selected.begin(), selected.end(), [](const Muon& a, const Muon& b) { return a.pt > b.pt; });
    event.put(label(), selected);
    return true;
  }

 private:
  double ptMin_, etaMax_, isoMax_;
  std::string id_;
};

MYF_REGISTER_MODULE(MuonSelector, "muons passing pt, eta, ID and isolation cuts")

}  // namespace myf
