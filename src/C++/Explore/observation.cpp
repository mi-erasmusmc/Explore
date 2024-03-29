//----------------------------------------------------------------------------//
// $Header: //depot/Explore/Explore/observation.cpp#8 $
// $Author: Rijnbeek $
// $DateTime: 2008/09/09 14:16:02 $
// $Change: 87 $
//----------------------------------------------------------------------------//

#include "observation.h"

/**********************************************************************
Function: OBSERVATION() 
Category: Constructors and Destructors
Scope: public
In: -
Out: -
Description: Default constructor for an observation.
**********************************************************************/
OBSERVATION::OBSERVATION() {
}

/**********************************************************************
Function: OBSERVATION()
Category: Constructors and Destructors
Scope: public
In: float, value of the observation.
    unsigned int, class of the observation.
    unsigned int, order of the observation.
Out: -
Description: Constructor for an observation.
**********************************************************************/
OBSERVATION::OBSERVATION(string OValue, unsigned int OClass, unsigned int OOrder) {
  Value = OValue;
  Class = OClass;
  Order = OOrder;
}

/**********************************************************************
Function: OBSERVATION()
Category: Constructors and Destructors
Scope: public
In: OBSERVATION&, observation to be copied.
Out: -
Description: Copy-constructor for an observation.
**********************************************************************/
OBSERVATION::OBSERVATION(const OBSERVATION& CObservation) {
  Value = CObservation.GetValue();
  Class = CObservation.GetClass();
  Order = CObservation.GetOrder();
}

/**********************************************************************
Function: ~OBSERVATION()
Category: Constructors and Destructors
Scope: public
In: -
Out: -
Description: Destructor for an observation.
**********************************************************************/
OBSERVATION::~OBSERVATION() {
}

/**********************************************************************
Function: GetOrder() const 
Category: Selectors
Scope: public
In: -
Out: -
Description: Returns the order of the observation.
**********************************************************************/
unsigned int OBSERVATION::GetOrder() const {
  return Order;
}

/**********************************************************************
Function: GetClass() const
Category: Selectors
Scope: public
In: -
Out: -
Description: Returns the class of the observation.
**********************************************************************/
unsigned int OBSERVATION::GetClass() const {
  return Class;
}

/**********************************************************************
Function: GetVValue() const
Category: Selectors
Scope: public
In: -
Out: -
Description: Returns the continuous value of the observation.
**********************************************************************/
string OBSERVATION::GetValue() const {
  return Value;
}

/**********************************************************************
Function: SetClass() const
Category: Selectors
Scope: public
In: - OClass the class
Out: -
Description: Sets the class of the observation.
**********************************************************************/
void OBSERVATION::SetClass(unsigned int OClass){
  Class = OClass;
}

