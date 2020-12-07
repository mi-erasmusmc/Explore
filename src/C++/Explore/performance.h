//----------------------------------------------------------------------------//
// $Header: //depot/Explore/Explore/performance.h#9 $
// $Author: Pepe $
// $DateTime: 2004/10/11 08:11:59 $
// $Change: 100 $
//----------------------------------------------------------------------------//

#ifndef PERFORMANCE_H
#define PERFORMANCE_H

#include "initialise.h"
#include "constraint.h"

class PERFORMANCE {
  public:
    CONSTRAINT Sensitivity;
    CONSTRAINT Specificity;
    CONSTRAINT NPV;
    CONSTRAINT PPV;
    CONSTRAINT Accuracy;                                   

    PERFORMANCE operator/(PERFORMANCE SourcePerformance);
    
    void Print();
    string PrintString(bool headers);
};

#endif

