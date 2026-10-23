#pragma once
// ModuleFactory: creates a module from its type name, so python can say "MuonSelector".
// Every module registers itself with MYF_REGISTER_MODULE(Type, "one line doc").
#include <functional>
#include <map>
#include <memory>
#include <string>

#include "myf/Module.h"

namespace myf {

class ModuleFactory {
 public:
  using Creator = std::function<std::unique_ptr<Module>(const std::string&, const ParameterSet&)>;

  static ModuleFactory& instance();

  bool add(const std::string& type, Creator creator, const std::string& doc);
  std::unique_ptr<Module> create(const std::string& type, const std::string& label,
                                 const ParameterSet& params) const;
  std::map<std::string, std::string> available() const;  // type -> doc

 private:
  ModuleFactory() = default;
  std::map<std::string, Creator> creators_;
  std::map<std::string, std::string> docs_;
};

}  // namespace myf

#define MYF_REGISTER_MODULE(TYPE, DOC)                                                       \
  namespace {                                                                                \
  const bool registered_##TYPE = ::myf::ModuleFactory::instance().add(                       \
      #TYPE,                                                                                 \
      [](const std::string& label, const ::myf::ParameterSet& params) {                      \
        return std::unique_ptr<::myf::Module>(std::make_unique<TYPE>(label, params));        \
      },                                                                                     \
      DOC);                                                                                  \
  }
