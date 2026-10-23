#pragma once
// Module: the base class of every step of the analysis.
// process() returns false to stop the event (a filter); producers and analyzers return true.
#include <string>

#include "myf/Event.h"
#include "myf/JobContext.h"
#include "myf/ParameterSet.h"

namespace myf {

class Module {
 public:
  Module(const std::string& label, const ParameterSet& params) : label_(label), params_(params) {}
  virtual ~Module() = default;

  virtual void beginJob(JobContext&) {}
  virtual bool process(Event& event) = 0;
  virtual void endJob() {}

  const std::string& label() const { return label_; }

 protected:
  const ParameterSet& params() const { return params_; }

 private:
  std::string label_;
  ParameterSet params_;
};

}  // namespace myf
