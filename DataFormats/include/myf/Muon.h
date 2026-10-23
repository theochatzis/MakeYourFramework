#pragma once
#include "myf/Lepton.h"

namespace myf {

struct Muon : public Lepton {
  bool tightId = false;
  bool mediumId = false;
};

}  // namespace myf
