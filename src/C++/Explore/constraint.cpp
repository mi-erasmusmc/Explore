//----------------------------------------------------------------------------//
// $Header: //depot/Explore/Explore/constraint.cpp#8 $
// $Author: Pepe $
// $DateTime: 2004/10/11 08:11:59 $
// $Change: 100 $
//----------------------------------------------------------------------------//

#include "constraint.h"

CONSTRAINT::CONSTRAINT() {
  Value = NULL;
}

/**********************************************************************
Function: Operator==
Category: Constructors and Destructors
Scope: public
In: CONSTRAINT, a constraint
Out: bool, the same or not
Description: Compares two constraints.
**********************************************************************/
bool CONSTRAINT::operator==(CONSTRAINT SourceConstraint) {
  if ((Value == SourceConstraint.Value) && (Measure == SourceConstraint.Measure)) {
    return true;
  }
  return false;
}

/**********************************************************************
Function: Operator<
Category: Constructors and Destructors
Scope: public
In: CONSTRAINT, a constraint
Out: bool, less or not
Description: Compares two constraints.
**********************************************************************/
bool CONSTRAINT::operator<(CONSTRAINT SourceConstraint) {
  if (Value < SourceConstraint.Value) {
    return true;
  }
  return false;
}

