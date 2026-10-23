// ZBuilder: the Z candidate from the two leading selected muons. A filter: events without
// two opposite-charge muons in the mass window are rejected.
// python: process.add("ZBuilder", "zCandidate", muons="goodMuons", leadPtMin=26, massMin=60, massMax=120)
#include "myf/ModuleFactory.h"
#include "myf/ZCandidate.h"

namespace myf {

class ZBuilder : public Module {
 public:
  ZBuilder(const std::string& label, const ParameterSet& params)
      : Module(label, params),
        muons_(params.getString("muons")),
        leadPtMin_(params.getDouble("leadPtMin")),
        massMin_(params.getDouble("massMin")),
        massMax_(params.getDouble("massMax")) {}

  bool process(Event& event) override {
    const auto& muons = event.get<std::vector<Muon>>(muons_);
    if (muons.size() < 2) return false;
    ZCandidate z{muons[0], muons[1], muons[0].p4() + muons[1].p4()};
    if (z.mu1.pt < leadPtMin_) return false;
    if (z.mu1.charge * z.mu2.charge >= 0) return false;
    if (z.mass() < massMin_ || z.mass() > massMax_) return false;
    event.put(label(), z);
    return true;
  }

 private:
  std::string muons_;
  double leadPtMin_, massMin_, massMax_;
};

MYF_REGISTER_MODULE(ZBuilder, "Z -> mumu candidate from the two leading `muons`")

}  // namespace myf
