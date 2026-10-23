// TriggerFilter: keep the event if any of the listed HLT paths fired.
// python: process.add("TriggerFilter", "trigger", paths=["HLT_IsoMu24", "HLT_IsoTkMu24"])
#include "myf/ModuleFactory.h"

namespace myf {

class TriggerFilter : public Module {
 public:
  TriggerFilter(const std::string& label, const ParameterSet& params)
      : Module(label, params), paths_(params.getStrings("paths")) {}

  void beginJob(JobContext& job) override {
    for (const auto& path : paths_) job.requestTrigger(path);
  }

  bool process(Event& event) override {
    for (const auto& path : paths_)
      if (event.triggers.at(path)) return true;
    return false;
  }

 private:
  std::vector<std::string> paths_;
};

MYF_REGISTER_MODULE(TriggerFilter, "keep events in which any of `paths` fired")

}  // namespace myf
