//----------------------------------------------------------------------------//
// $Header: //depot/Explore/Explore/mandatory.cpp#6 $
// $Author: Pepe $
// $DateTime: 2004/10/11 08:11:59 $
// $Change: 100 $
//----------------------------------------------------------------------------//

#include "mandatory.h"

/**********************************************************************
Function: MANDATORY()
Category: Constructors and Destructors
Scope: public
In: -
Out: -
Description: Default constructor for Mandatory class.
**********************************************************************/
MANDATORY::MANDATORY() {
}

/**********************************************************************
Function: ~MANDATORY()
Category: Constructors and Destructors
Scope: public
In: -
Out: -
Description: Default destructor for Mandatory class.
**********************************************************************/
MANDATORY::~MANDATORY() {
}

/**********************************************************************
Function: AddFeature()
Category: Modifiers
Scope: public
In: unsigned int, feature number
Out: -
Description: Adds a mandatory feature.
**********************************************************************/
void MANDATORY::AddFeature(unsigned int Number) {
	Features.push_back(Number);
    Included.push_back(false);
}

/**********************************************************************
Function: RemoveFeature()
Category: Modifiers
Scope: public
In: unsigned int, feature number
Out: -
Description: Removes a mandatory feature.
**********************************************************************/
void MANDATORY::RemoveFeature(unsigned int Number) {
  list<unsigned int>::iterator CurrentFeature(Features.begin());
  list<unsigned int>::iterator LastFeature(Features.end());
  list<bool>::iterator CurrentIncluded(Included.begin());

  while (CurrentFeature != LastFeature) {
    if ((*CurrentFeature)==Number) {
      Features.erase(CurrentFeature);
      Included.erase(CurrentIncluded);
    }
    CurrentFeature++;
    CurrentIncluded++;
  }
}

/**********************************************************************
Function: RemoveAll()
Category: Modifiers
Scope: public
In: unsigned int, feature number
Out: -
Description: Removes all mandatory features.
**********************************************************************/
void MANDATORY::RemoveAll() {
  Features.clear();
  Included.clear();
}

/**********************************************************************
Function: IsIncluded()
Category: Selectors
Scope: public
In: unsigned int, feature number
Out: -
Description: Check if a mandatory feature has been included yet.
**********************************************************************/
bool MANDATORY::IsIncluded(unsigned int Number) {
  list<unsigned int>::iterator CurrentFeature(Features.begin());
  list<unsigned int>::iterator LastFeature(Features.end());
  list<bool>::iterator CurrentIncluded(Included.begin());

  while (CurrentFeature != LastFeature) {
    if ((*CurrentFeature)==Number && (*CurrentIncluded)>0) {
      return true;
    }
    CurrentFeature++;
    CurrentIncluded++;
  }
  return false;
}

/**********************************************************************
Function: TimesIncluded()
Category: Selectors
Scope: public
In: unsigned int, feature number
Out: -
Description: Returns the number of times a mandatory feature has been
included.
**********************************************************************/
unsigned int MANDATORY::TimesIncluded(unsigned int Number) {
  list<unsigned int>::iterator CurrentFeature(Features.begin());
  list<unsigned int>::iterator LastFeature(Features.end());
  list<bool>::iterator CurrentIncluded(Included.begin());

  while (CurrentFeature != LastFeature) {
    if ((*CurrentFeature)==Number) {
      return (*CurrentIncluded);
    }
    CurrentFeature++;
    CurrentIncluded++;
  }
  return 0;
}

/**********************************************************************
Function: IsMandatory()
Category: Selectors
Scope: public
In: unsigned int, feature number
Out: -
Description: Returns whether a feature is mandatory or not.
**********************************************************************/
bool MANDATORY::IsMandatory(unsigned int Number) {
  list<unsigned int>::iterator CurrentFeature(Features.begin());
  list<unsigned int>::iterator LastFeature(Features.end());

  for (; CurrentFeature != LastFeature; CurrentFeature++) {
    if ((*CurrentFeature)==Number) {
      return true;
    }
  }
  return false;
}

/**********************************************************************
Function: FeaturesLeft()
Category: Selectors
Scope: public
In: -
Out: -
Description: Returns the number of mandatory features which haven't
been included yet (TimesIncluded[FeatureNumber] == 0).
**********************************************************************/
unsigned int MANDATORY::FeaturesLeft() {
  list<bool>::iterator CurrentIncluded(Included.begin());
  list<bool>::iterator LastIncluded(Included.end());

  unsigned int FeaturesLeft=0;

  for (; CurrentIncluded != LastIncluded; CurrentIncluded++) {
    if ((*CurrentIncluded)==0) {
      FeaturesLeft++;
    }
  }
  return FeaturesLeft;
}
 
