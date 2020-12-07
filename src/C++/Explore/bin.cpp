//----------------------------------------------------------------------------//
// $Header: //depot/Explore/Explore/bin.cpp#8 $
// $Author: Rijnbeek $
// $DateTime: 2008/09/09 14:16:02 $
// $Change: 87 $
//----------------------------------------------------------------------------//


#include "bin.h"

/**********************************************************************
Function: BIN()
Category: Constructors and Destructors
Scope: public
In: -
Out: -
Description: Default constructor for a Bin.
**********************************************************************/
BIN::BIN() {
}

/**********************************************************************
Function: BIN()
Category: Constructors and Destructors
Scope: public
In: float, value of the bin.
Out: -
Description: Constructor for a bin.
**********************************************************************/
BIN::BIN(string BValue) {
  Value = BValue;
  NegativeCount = 0;
  PositiveCount = 0;
}

/**********************************************************************
Function: ~BIN()
Category: Constructors and Destructors
Scope: public
In: -
Out: -
Description: Destructor for a bin.
**********************************************************************/
BIN::~BIN() {
}

/**********************************************************************
Function: GetValue()
Category: Selectors
Scope: public
In: -
Out: float, value of the bin.
Description:
**********************************************************************/
string BIN::GetValue() {
  return Value;
}


/**********************************************************************
Function: GetNegativeCount()
Category: Selectors
Scope: public
In: -
Out: -
Description: Returns the number of negative observations
associated with this bin.
**********************************************************************/
unsigned int BIN::GetNegativeCount() {                                          // Get negative count
  return NegativeCount;
}

/**********************************************************************
Function: GetPositiveCount()
Category: Selectors
Scope: public
In: -
Out: -
Description: Returns the number of positive observations
associated with this bin.
**********************************************************************/
unsigned int BIN::GetPositiveCount() {                                          // Get positive count
  return PositiveCount;
}

/**********************************************************************
Function: IncreaseNegativeCount()
Category: Modifiers
Scope: public
In: -
Out: -
Description: Increases the number of negative observations
associated with this bin by one.
**********************************************************************/
void BIN::IncreaseNegativeCount() {                                                  // Increase negative count
  NegativeCount += 1;
}

/**********************************************************************
Function: IncreasePositiveCount()
Category: Modifiers
Scope: public
In: -
Out: -
Description: Increases the number of positive observations
associated with this bin by one.
**********************************************************************/
void BIN::IncreasePositiveCount() {                                                  // Increase positive count
  PositiveCount += 1;
}

