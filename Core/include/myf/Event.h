#pragma once
// Event: everything we know about one collision.
// The reader fills the raw objects; modules add "products" (selected muons, a Z candidate, ...)
// with put() and later modules read them back with get(), by label.
#include <any>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

#include "myf/Jet.h"
#include "myf/Muon.h"

namespace myf {

class Event {
 public:
  // raw content, filled by the NanoReader
  unsigned run = 0;
  unsigned lumi = 0;
  unsigned long long number = 0;
  bool isData = false;
  double weight = 1.0;  // generator weight for MC, 1 for data
  std::vector<Muon> muons;
  std::vector<Jet> jets;
  std::map<std::string, bool> triggers;

  template <typename T>
  void put(const std::string& label, T product) {
    if (has(label)) throw std::runtime_error("[myf] product '" + label + "' already exists");
    products_[label] = std::move(product);
  }

  template <typename T>
  const T& get(const std::string& label) const {
    auto it = products_.find(label);
    if (it == products_.end()) throw std::runtime_error("[myf] no product '" + label + "' in the event");
    if (const T* product = std::any_cast<T>(&it->second)) return *product;
    throw std::runtime_error("[myf] product '" + label + "' has a different type");
  }

  bool has(const std::string& label) const { return products_.count(label) > 0; }

  void clear() {
    muons.clear();
    jets.clear();
    triggers.clear();
    products_.clear();
  }

 private:
  std::map<std::string, std::any> products_;
};

}  // namespace myf
