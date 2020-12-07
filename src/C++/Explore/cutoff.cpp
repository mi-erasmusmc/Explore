//----------------------------------------------------------------------------//
// $Header: //depot/Explore/Explore/cutoff.cpp#12 $
// $Author: Rijnbeek $
// $DateTime: 2008/09/09 14:16:02 $
// $Change: 87 $
//----------------------------------------------------------------------------//

#include "cutoff.h"

/**********************************************************************
Function: CUTOFF() 
Category: Constructors and Destructors
Scope: public
In: -
Out: -
Description: Default constructor for a cutoff.
**********************************************************************/
CUTOFF::CUTOFF() {
}

/**********************************************************************
Function: CUTOFF()
Category: Constructors and Destructors
Scope: public
In: float, cutoff value.
Out: -
Description: Default constructor for a cutoff.
**********************************************************************/
CUTOFF::CUTOFF(string CValue) {                                                  // Create a new cutoff value
  Value = CValue;
}

/**********************************************************************
Function: ~CUTOFF() 
Category: Constructors and Destructors
Scope: public
In: -
Out: -
Description: Destructor for an cutoff.
**********************************************************************/
CUTOFF::~CUTOFF() {
}

/**********************************************************************
Function: InitialiseSet()
Category: Modifiers
Scope: public
In: vector<CLASSES>*, pointer to classes of learn or test partition
    OPERATOR*, conditionset operator
Out: -
Description: Creates a set for each cutoff/operator combination.
Need SourceLearnClasses fo rthis
**********************************************************************/
void CUTOFF::InitialiseSet(vector<CLASSES>* PopulationClasses, OPERATOR* ConditionOperator) {
  Set.Initialise(PopulationClasses, ConditionOperator, &Value);
}

/**********************************************************************
Function: GetValue()
Category: Selectors
Scope: public
In: -
Out: -
Description: Returns the cutoff value.
**********************************************************************/
string CUTOFF::GetValue() {                                                     // Return cutoff value
  return Value;
}

/**********************************************************************
Function: GetReferentValue()
Category: Selectors
Scope: public
In: -
Out: -
Description: Returns the cutoff value.
**********************************************************************/
string* CUTOFF::GetReferentValue() {                                             // Return cutoff value
  return &Value;
}


