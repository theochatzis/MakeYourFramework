#pragma once
// Looper: the event loop. It owns the modules, reads the events, runs the modules
// in order and does the bookkeeping. Python creates one per sample.
#include <memory>
#include <string>
#include <vector>

#include "myf/Module.h"
#include "myf/ParameterSet.h"
#include "myf/Sample.h"

namespace myf {

class Looper {
 public:
  Looper(const Sample& sample, const std::string& outputFile);

  void add(const std::string& type, const std::string& label, const ParameterSet& params);
  void setConfig(const std::string& config) { config_ = config; }
  void run(long long maxEvents = -1);

 private:
  Sample sample_;
  std::string outputFile_;
  std::string config_;
  std::vector<std::unique_ptr<Module>> modules_;
};

}  // namespace myf
