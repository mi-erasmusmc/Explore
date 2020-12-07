//----------------------------------------------------------------------------//
// $Header: //depot/Explore/Explore/rocpair.h#6 $
// $Author: Pepe $
// $DateTime: 2004/10/11 08:11:59 $
// $Change: 100 $
//----------------------------------------------------------------------------//

#ifndef ROCPAIR_H
#define ROCPAIR_H

#include "initialise.h"

class ROCPair {
  private:
    float Sensitivity;
    float FPF;
    float Value;

  public:
    ROCPair(float RSensitivity, float RFPF, float RValue);
    ~ROCPair();

    float GetSensitivity();
    float GetFPF();
    float GetValue();

    bool operator<(const ROCPair& a) const;
};

#endif

 