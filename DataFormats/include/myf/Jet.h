#pragma once
#include "myf/Particle.h"

namespace myf {

struct Jet : public Particle {
  int jetId = 0;     // NanoAOD bit field: bit 1 = tight, bit 2 = tight + lepton veto
  float btag = 0;    // DeepJet b-tag score

  bool passesTightId() const { return jetId & 2; }
};

}  // namespace myf
