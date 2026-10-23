#pragma once
// Lepton: what muons and electrons have in common.
#include "myf/Particle.h"

namespace myf {

struct Lepton : public Particle {
  int charge = 0;
  float relIso = 0;  // relative PF isolation (dR < 0.4 for muons)
};

}  // namespace myf
