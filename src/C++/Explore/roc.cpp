//----------------------------------------------------------------------------//
// $Header: //depot/Explore/Explore/roc.cpp#6 $
// $Author: Pepe $
// $DateTime: 2004/10/11 08:11:59 $
// $Change: 100 $
//----------------------------------------------------------------------------//

#include "roc.h"

/**********************************************************************
Function: ROC()
Category: Constructors and Destructors
Scope: public
In: unsigned int, the rulelength for this roc curve
Out: -
Description: Default constructor for this ROC curve. 
**********************************************************************/
ROC::ROC(unsigned int RRuleLength) {
  RuleLength = RRuleLength;
  MaxSensitivity.reserve(100);
  for (unsigned int i=0; i<100; i++) {
    MaxSensitivity.push_back(0);
  }
}

/**********************************************************************
Function: ROC()
Category: Constructors and Destructors
Scope: public
In: -
Out: -
Description: Default destructor for this ROC curve.
**********************************************************************/
ROC::~ROC() {
}

/**********************************************************************
Function: ROC()
Category: Modifiers
Scope: public
In: double, specificity for this entry
    double, sensitivity for this entry
Out: -
Description: Adds an entry to the ROC curve if the sensitivity is
higher than the maximum sensitivity up till now.
**********************************************************************/
void ROC::AddEntry(double Specificity, double Sensitivity) {
  unsigned int Entry;
  double MaxValue;  

  Entry = Round((1 - Specificity)*100);
  if (MaxSensitivity[Entry-1] < (Sensitivity*100)) {
    MaxSensitivity[Entry-1] = (Sensitivity*100);
  }
}

/**********************************************************************
Function: GetEntry()
Category: Selectors
Scope: public
In: double, false-positive fraction
Out: -
Description: Returns the maximum sensitivity for a given
false-positive fraction.
**********************************************************************/
double ROC::GetEntry(double FPF) {
  return MaxSensitivity[(Round(FPF*100)-1)];
}

/**********************************************************************
Function: GetRuleLength()
Category: Selectors
Scope: public
In: -
Out: unsigned int, the rulelength
Description: Returns the rulelength for this ROC Curve
**********************************************************************/
unsigned int ROC::GetRuleLength() {
  return RuleLength;
}

/**********************************************************************
Function: GetROC()
Category: Selectors
Scope: public
In: -
Out: vector<double>*, vector containing maximum sensitivity values
Description: Returns the addres of this ROC Curve.
**********************************************************************/
std::vector<double>* ROC::GetROC() {
  return &MaxSensitivity;
}

/**********************************************************************
Function: Round()
Category: Private
Scope: public
In: double, a double value
Out: int, the rounded value
Description: Rounds a given double value.
**********************************************************************/
int ROC::Round(double d) {
 int lo = d>0 ? (int)d : (int)d-1, hi = lo+1;
 return (d-lo)<(hi-d) ? lo : hi;
}