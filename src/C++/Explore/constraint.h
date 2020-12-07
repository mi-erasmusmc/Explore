//----------------------------------------------------------------------------//
// $Header: //depot/Explore/Explore/constraint.h#8 $
// $Author: Pepe $
// $DateTime: 2004/10/11 08:11:59 $
// $Change: 100 $
//----------------------------------------------------------------------------//

#ifndef CONSTRAINT_H
#define CONSTRAINT_H

#include "initialise.h"

class CONSTRAINT {
  private:
    bool operator==(CONSTRAINT SourceConstraint);
    bool operator<(CONSTRAINT SourceConstraint);
    bool operator<(float SourceValue);

  public:
    CONSTRAINT();

    PERFORMANCE_MEASURE Measure;                                                // Variable (sensitivity, specificity, ppv, npv, accuracy)
    float Value;                                                                // Performance value (can be a minimum)
};

#endif

