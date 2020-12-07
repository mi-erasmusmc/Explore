//----------------------------------------------------------------------------//
// $Header: //depot/Explore/Explore/conjunction.h#11 $
// $Author: Pepe $
// $DateTime: 2004/10/11 08:11:59 $
// $Change: 100 $
//----------------------------------------------------------------------------//

#ifndef CONJUNCTION_H
#define CONJUNCTION_H

#include "initialise.h"
#include "condition.h"

class CONJUNCTION {
  private:
    vector<FEATURE>* SourceFeatures;                                            // Pointer to a list of features

  public:
    SET ConjunctionSet;                                                         // A SET for calculating conjunction-performance
    
    unsigned int Size;

    // Public properties
    vector<CONDITION> Conditions;                                               // List of conditions

    // -------------------------------------------------------------------------

    bool Changed;
    SET CumulativeSet;

    // -------------------------------------------------------------------------

    // Constructors and destructors
    CONJUNCTION();                                                              // Default constructor
    CONJUNCTION(unsigned int CSize);

    ~CONJUNCTION();                                                             // Default destructor

    // Modifiers
    SET& CalculatePerformance();                                                // Calculate the performance of this conjunction on the current Test or Learn partition

    // Output
    string PrintPerformance();

};

#endif

