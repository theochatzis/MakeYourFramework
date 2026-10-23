#pragma once
// ZCandidate: a pair of opposite-charge muons, the leading one first.
#include "myf/Muon.h"

namespace myf {

struct ZCandidate {
  Muon mu1;
  Muon mu2;
  LorentzVector p4;

  double mass() const { return p4.M(); }
  double pt() const { return p4.Pt(); }
};

}  // namespace myf
