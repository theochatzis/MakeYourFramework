#include "myf/ModuleFactory.h"

#include <stdexcept>

namespace myf {

ModuleFactory& ModuleFactory::instance() {
  static ModuleFactory factory;
  return factory;
}

bool ModuleFactory::add(const std::string& type, Creator creator, const std::string& doc) {
  if (creators_.count(type)) throw std::runtime_error("[myf] module type '" + type + "' registered twice");
  creators_[type] = std::move(creator);
  docs_[type] = doc;
  return true;
}

std::unique_ptr<Module> ModuleFactory::create(const std::string& type, const std::string& label,
                                              const ParameterSet& params) const {
  auto it = creators_.find(type);
  if (it == creators_.end()) throw std::runtime_error("[myf] unknown module type '" + type + "'");
  return it->second(label, params);
}

std::map<std::string, std::string> ModuleFactory::available() const { return docs_; }

}  // namespace myf
