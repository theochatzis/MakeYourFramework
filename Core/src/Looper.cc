#include "myf/Looper.h"

#include <TChain.h>
#include <TError.h>
#include <TFile.h>

#include <cstdio>
#include <stdexcept>

#include "myf/Bookkeeper.h"
#include "myf/Event.h"
#include "myf/JobContext.h"
#include "myf/ModuleFactory.h"
#include "myf/NanoReader.h"

namespace myf {

Looper::Looper(const Sample& sample, const std::string& outputFile)
    : sample_(sample), outputFile_(outputFile) {}

void Looper::add(const std::string& type, const std::string& label, const ParameterSet& params) {
  for (const auto& module : modules_)
    if (module->label() == label) throw std::runtime_error("[myf] module label '" + label + "' used twice");
  modules_.push_back(ModuleFactory::instance().create(type, label, params));
}

// sum of genEventSumw over the Runs trees: the sum of weights of the full input files
static double runsSumw(const std::vector<std::string>& files) {
  TChain runs("Runs");
  for (const auto& f : files) runs.Add(f.c_str());
  if (!runs.GetBranch("genEventSumw")) return 0;
  double sumw = 0, total = 0;
  runs.SetBranchAddress("genEventSumw", &sumw);
  for (Long64_t i = 0; i < runs.GetEntries(); ++i) {
    runs.GetEntry(i);
    total += sumw;
  }
  return total;
}

void Looper::run(long long maxEvents) {
  if (sample_.files.empty()) throw std::runtime_error("[myf] sample '" + sample_.name + "' has no files");
  std::printf("[myf] sample %s (%s), %zu file(s), %zu module(s)\n", sample_.name.c_str(),
              sample_.isData ? "data" : "MC", sample_.files.size(), modules_.size());

  // NanoAOD files carry CMSSW metadata classes without dictionaries here: silence those warnings
  gErrorIgnoreLevel = kError;

  TChain chain("Events");
  for (const auto& f : sample_.files) chain.Add(f.c_str());
  NanoReader reader(chain, sample_.isData);

  TFile output(outputFile_.c_str(), "RECREATE");
  if (output.IsZombie()) throw std::runtime_error("[myf] cannot create " + outputFile_);

  std::vector<std::string> labels;
  for (auto& module : modules_) {
    labels.push_back(module->label());
    JobContext context(sample_, output.mkdir(module->label().c_str()), reader);
    module->beginJob(context);
  }
  Bookkeeper book(labels);

  Event event;
  long long n = 0;
  while ((maxEvents < 0 || n < maxEvents) && reader.next()) {
    ++n;
    if (n % 50000 == 0) std::printf("[myf] %lld events\n", n);
    event.clear();
    reader.fill(event);
    book.countRead(event.weight);
    if (sample_.isData) book.addLumiSection(event.run, event.lumi);

    for (size_t i = 0; i < modules_.size(); ++i) {
      if (!modules_[i]->process(event)) break;
      book.countPass(i, event.weight);
    }
  }

  for (auto& module : modules_) module->endJob();
  if (!sample_.isData) book.setRunsSumw(runsSumw(sample_.files));
  book.print();
  book.write(output.mkdir("myf"), sample_, config_);
  output.Write();
  output.Close();
  std::printf("[myf] wrote %s\n", outputFile_.c_str());
}

}  // namespace myf
