//----------------------------------------------------------------------------//
// $Header: //depot/Explore/Explore/rocpair.cpp#6 $
// $Author: Pepe $
// $DateTime: 2004/10/11 08:11:59 $
// $Change: 100 $
//----------------------------------------------------------------------------//

#include "rocpair.h"

ROCPair::ROCPair(float RSensitivity, float RFPF, float RValue) {
  Sensitivity = RSensitivity;
  FPF = RFPF;
  Value = RValue;
}

ROCPair::~ROCPair() {
}

float ROCPair::GetSensitivity() {
  return Sensitivity;
}

float ROCPair::GetFPF() {
  return FPF;
}

float ROCPair::GetValue() {
  return Value;
}

bool ROCPair::operator<(const ROCPair& a) const {
  return FPF < a.FPF;
}

 