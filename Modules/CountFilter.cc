// CountFilter: keep events with at least `min` (and at most `max`) objects in a collection.
// python: process.add("CountFilter", "atLeastOneJet", jets="goodJets", min=1)
#include "myf/ModuleFactory.h"

namespace myf {

class CountFilter : public Module {
 public:
  CountFilter(const std::string& label, const ParameterSet& params)
      : Module(label, params),
        jets_(params.getString("jets")),
        min_(params.getInt("min", 0)),
        max_(params.getInt("max", 1000)) {}

  bool process(Event& event) override {
    int n = event.get<std::vector<Jet>>(jets_).size();
    return n >= min_ && n <= max_;
  }

 private:
  std::string jets_;
  int min_, max_;
};

MYF_REGISTER_MODULE(CountFilter, "keep events with min <= number of `jets` <= max")

}  // namespace myf
