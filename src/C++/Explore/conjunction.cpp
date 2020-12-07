//----------------------------------------------------------------------------//
// $Header: //depot/Explore/Explore/conjunction.cpp#14 $
// $Author: Pepe $
// $DateTime: 2004/10/11 08:11:59 $
// $Change: 100 $
//----------------------------------------------------------------------------//

#include "conjunction.h"

#ifdef DEBUG_TIMING
extern TIMING ExploreTiming;
#endif

/**********************************************************************
Function: CONJUNCTION()
Category: Constructors and Destructors
Scope: public
In: -
Out: -
Description: Default constructor for a Conjunction.
**********************************************************************/
CONJUNCTION::CONJUNCTION() {
  Changed = true;
}

/**********************************************************************
Function: ~CONJUNCTION
Category: Constructors and Destructors
Scope: public
In: -
Out: -
Description: Destructor for a Conjunction.
**********************************************************************/
CONJUNCTION::~CONJUNCTION() {
}

/**********************************************************************
Function: CONJUNCTION()
Category: Constructors and Destructors
Scope: public
In: unsigned int, conjunction size
Out: -
Description: Constructor for a Conjunction.
**********************************************************************/
CONJUNCTION::CONJUNCTION(unsigned int CSize) {
  if (CSize>0) {
    Size = CSize;
  }
}

/**********************************************************************
Function: CalculatePerformance()
Category: Modifiers
Scope: public
In: PARTITION_TYPE, type of partition.
Out: PERFORMANCE, performance of the Conjunction.
Description: Calculates the performance of this Conjunction on the learn or
test partition.
**********************************************************************/
SET& CONJUNCTION::CalculatePerformance() {
#ifdef DEBUG_TIMING
  clock_t Start, End;
  Start = clock();
#endif
  SET Dummy;

  for (unsigned int i=0; i < Conditions.size(); i++) {
    if (i==0) {
      ConjunctionSet = *Conditions[i].CalculatePerformance();
    } else {
      ConjunctionSet = ConjunctionSet & *Conditions[i].CalculatePerformance();
    }
  }

  // Only when optimizing on sensitivity!!!
  ConjunctionSet.SummariseBags();

#ifdef DEBUG_TIMING
  End = clock();
  ExploreTiming.AddTime("CONJUNCTION::CalculatePerformance", Start, End);
#endif

  return ConjunctionSet;
}

/**********************************************************************
Function: PrintPerformance()
Category: Selectors
Scope: public
In: -
Out: -
Description: Prints the current performance of this conjunction.
**********************************************************************/
string CONJUNCTION::PrintPerformance() {
  ostringstream Result;

  ConjunctionSet.PrintPerformance();
  for (unsigned int i=0; i < Conditions.size(); i++) {
    Result << Conditions[i].PrintPerformance();
  }

  return Result.str();
}

