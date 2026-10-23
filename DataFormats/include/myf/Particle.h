#pragma once
// Particle: the common base of every physics object (muons, jets, ...).
#include <Math/Vector4D.h>
#include <Math/VectorUtil.h>
#include <cmath>

namespace myf {

using LorentzVector = ROOT::Math::PtEtaPhiMVector;

struct Particle {
  float pt = 0;
  float eta = 0;
  float phi = 0;
  float mass = 0;

  LorentzVector p4() const { return LorentzVector(pt, eta, phi, mass); }
};

inline float deltaPhi(float phi1, float phi2) {
  float dphi = phi1 - phi2;
  while (dphi > M_PI) dphi -= 2 * M_PI;
  while (dphi <= -M_PI) dphi += 2 * M_PI;
  return dphi;
}

inline float deltaR(const Particle& a, const Particle& b) {
  float deta = a.eta - b.eta;
  float dphi = deltaPhi(a.phi, b.phi);
  return std::sqrt(deta * deta + dphi * dphi);
}

}  // namespace myf
