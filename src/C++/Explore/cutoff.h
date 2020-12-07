//----------------------------------------------------------------------------//
// $Header: //depot/Explore/Explore/cutoff.h#13 $
// $Author: Rijnbeek $
// $DateTime: 2008/09/09 14:16:02 $
// $Change: 87 $
//----------------------------------------------------------------------------//

#ifndef CUTOFF_H
#define CUTOFF_H

#include "initialise.h"
#include "set.h"

class CUTOFF {
  public:
	string Value;
    SET Set;

    //Constructors and destructors
    CUTOFF();
	CUTOFF(string CValue);                                                       // Create a new cutoff value
    ~CUTOFF();                                                                  // Destructor

    // Modifiers
    void InitialiseSet(vector<CLASSES>* PopulationClasses, OPERATOR* ConditionOperator);

    // Selectors
	string GetValue();                                                           // Return value of this cutoff
    string* GetReferentValue();                                                  // Return a pointer to the value of this cutoff
};

#endif
