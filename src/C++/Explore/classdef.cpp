//----------------------------------------------------------------------------//
// $Header: //depot/Explore/Explore/classdef.cpp#11 $
// $Author: Rijnbeek $
// $DateTime: 2008/09/09 14:16:02 $
// $Change: 87 $                                                              
//----------------------------------------------------------------------------//

#include "classdef.h"
#include "initialise.h"
// #include "sysutils.hpp"

/**********************************************************************
Function: CLASSES()
Category: Constructors and Destructors
Scope: public
In: -
Out: -
Description: Default constructor for a Class.
**********************************************************************/
CLASSES::CLASSES() {
}

/**********************************************************************
Function: CLASSES()
Category: Constructors and Destructors
Scope: public
In: unsigned int, class.
Out: -
Description: Default constructor for a Class.
**********************************************************************/
CLASSES::CLASSES(unsigned int ClassNumber) {
  Class = ClassNumber;
}

/**********************************************************************
Function: ~CLASSES()
Category: Constructors and Destructors
Scope: public
In: -
Out: -
Description: Destructor for a Class.
**********************************************************************/
CLASSES::~CLASSES() {
}

/**********************************************************************
Function: AddObservation()
Category: Modifiers
Scope: public
In: OBSERVATION*, pointer to an original observation.
Out: -
Description: Adds an observation to this Class.
**********************************************************************/
void CLASSES::AddObservation(OBSERVATION* NewObservation) {
  Observations.push_back(NewObservation);
}

/**********************************************************************
Function: Clear()
Category: Modifiers
Scope: public
In: -
Out: -
Description: Clears all observations associated with this Class.
**********************************************************************/
void CLASSES::Clear() {
  Observations.clear();
}

/**********************************************************************
Function: SortByOrder()
Category: Modifiers
Scope: public
In: -
Out: -
Description: Sorts the observations of this Class based on
their order. 
**********************************************************************/
void CLASSES::SortOrder() {
  sort(Observations.begin(), Observations.end(), ByOrder);
}

/**********************************************************************
Function: SortByValue()
Category: Modifiers
Scope: public
In: -
Out: -
Description: Sorts the observations of this Class based on
their value.
**********************************************************************/
void CLASSES::SortValue() {
  sort(Observations.begin(), Observations.end(), ByValue);
}

/**********************************************************************
Function: GetSize() 
Category: Selectors
Scope: public
In: -
Out: -
Description: Returns the number of observations associated with this
Class.
**********************************************************************/
unsigned int CLASSES::GetSize() {
  return Observations.size();
}

/**********************************************************************
Function:
Category: Selectors
Scope: public
In: -
Out: -
Description: 
**********************************************************************/
float CLASSES::GetSensitivity(float OValue) {
  vector<OBSERVATION*>::iterator CurrentObservation(Observations.begin());
  vector<OBSERVATION*>::iterator LastObservation(Observations.end());
  float Overlap;
  unsigned int i;

  for (i=0; CurrentObservation != LastObservation; CurrentObservation++) {
  if (atof((*CurrentObservation)->GetValue().c_str()) >= OValue){
        i++;
    }
  }

  Overlap = (float)i/(float)GetSize();
  return Overlap;
}

/**********************************************************************
Function: GetSpecificity()
Category: Selectors
Scope: public
In: float, cutoff value
Out: -
Description: Determines the specificity of a cutoff for use in
calculating ROC Area and Diagonal methods for finding Operators.
**********************************************************************/
float CLASSES::GetSpecificity(float OValue) {
  vector<OBSERVATION*>::iterator CurrentObservation(Observations.begin());
  vector<OBSERVATION*>::iterator LastObservation(Observations.end());
  float Overlap;
  unsigned int i=0;

  for (i=0; CurrentObservation != LastObservation; CurrentObservation++) {
  if (atof((*CurrentObservation)->GetValue().c_str()) >= OValue) {
      i++;
    }
  }

  Overlap = ((float)GetSize() - (float)i)/(float)GetSize();
  return Overlap;
}

/**********************************************************************
Function: GetClass()
Category: Selectors
Scope: public
In: -
Out: unsigned int, classnumber
Description: Get the classnumber of this Class.
**********************************************************************/
unsigned int CLASSES::GetNumber() {
  return Class;
}

/**********************************************************************
Function: GetObservation()
Category: Selectors
Scope: public
In: unsigned int, number of an observation.
Out: OBSERVATION*, pointer to an original Observation.
Description: Returns a reference to an observation based on it's order.
**********************************************************************/
OBSERVATION* CLASSES::GetObservation(unsigned int ObservationNumber) {
  return Observations.at(ObservationNumber);
}

/**********************************************************************
Function: GetNoObservations()
Category: Selectors
Scope: public
In: -
Out: unsigned int, number of observations in this class
Description: Returns the number of observations for a class.
**********************************************************************/
unsigned int CLASSES::GetNoObservations() {
  return Observations.size();
}

/**********************************************************************
Function: PrintObservations()
Category: Selectors
Scope: public
In: -
Out: -
Description: Prints all observations in the Class.
**********************************************************************/
void CLASSES::PrintObservations() {
  vector<OBSERVATION*>::iterator CurrentObservation(Observations.begin());
  vector<OBSERVATION*>::iterator LastObservation(Observations.end());
  OBSERVATION* Observation;

  cout << "CLASS " << Class << "(" << Observations.size() << " observations)" << endl;
  cout << "Order\tValue" << endl;
  for (; CurrentObservation != LastObservation; CurrentObservation++) {
    Observation = (*CurrentObservation);
    cout << (*Observation).GetOrder() << "\t" << (*Observation).GetValue() << endl;
  }
  cout << endl;
}

