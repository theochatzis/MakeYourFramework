#pragma once
// ParameterSet: the configuration of one module, filled from python.
#include <map>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>

namespace myf {

class ParameterSet {
 public:
  using Value = std::variant<double, std::string, std::vector<std::string>>;

  ParameterSet() = default;
  explicit ParameterSet(std::map<std::string, Value> values) : values_(std::move(values)) {}

  bool has(const std::string& name) const { return values_.count(name) > 0; }

  double getDouble(const std::string& name) const { return get<double>(name); }
  double getDouble(const std::string& name, double fallback) const {
    return has(name) ? getDouble(name) : fallback;
  }
  int getInt(const std::string& name, int fallback) const {
    return has(name) ? static_cast<int>(getDouble(name)) : fallback;
  }
  bool getBool(const std::string& name, bool fallback) const {
    return has(name) ? getDouble(name) != 0 : fallback;
  }
  const std::string& getString(const std::string& name) const { return get<std::string>(name); }
  std::string getString(const std::string& name, const std::string& fallback) const {
    return has(name) ? getString(name) : fallback;
  }
  const std::vector<std::string>& getStrings(const std::string& name) const {
    return get<std::vector<std::string>>(name);
  }

 private:
  template <typename T>
  const T& get(const std::string& name) const {
    auto it = values_.find(name);
    if (it == values_.end()) throw std::runtime_error("[myf] missing parameter '" + name + "'");
    if (const T* value = std::get_if<T>(&it->second)) return *value;
    throw std::runtime_error("[myf] parameter '" + name + "' has the wrong type");
  }

  std::map<std::string, Value> values_;
};

}  // namespace myf
