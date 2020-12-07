//----------------------------------------------------------------------------//
// $Header: //depot/Explore/Explore/condition.cpp#16 $
// $Author: Rijnbeek $
// $DateTime: 2008/09/09 14:16:02 $
// $Change: 87 $
//----------------------------------------------------------------------------//

#include "condition.h"

/**********************************************************************
Function: CONDITION()
Category: Constructors and Destructors
Scope: public
In: -
Out: -
Description: Default constructor for a Condition.
**********************************************************************/
CONDITION::CONDITION() {
  FeatureNumber = 0;
  CutoffNumber = 0;
  
  // Needed for cutoffsets
  IsSolo = false;
  NonSoloIncluded = false;

  NextSame = false;
  PreviousSame = false;
}

/**********************************************************************
Function: CONDITION()
Category: Constructors and Destructors
Scope: public
In: unsigned in, feature number
    string, feature name
    vector<CUTOFF>, cutoffs
    OPERATOR, operator
Out: -
Description: Default constructor for a Condition.
**********************************************************************/
CONDITION::CONDITION(unsigned int CNumber, string CName, vector<CUTOFF> CCutoffs, OPERATOR COperator, unsigned int FOperator) {
  FeatureNumber = CNumber;
  FeatureName = CName;
  Cutoffs = CCutoffs;
  Operator = COperator;
  FeatureOperator = FOperator;
  CutoffNumber = NULL;
}

/**********************************************************************
Function: ~CONDITION()
Category: Constructors and Destructors
Scope: public
In: -
Out: -
Description: Destructor for a rulefeature.
**********************************************************************/
CONDITION::~CONDITION() {
}

/**********************************************************************
Function: NextCutoff()
Category: Modifiers
Scope: public
In: -
Out: -
Description: Go to the next cutoff of this rulefeature.
**********************************************************************/
void CONDITION::NextCutoff() {
  CutoffNumber++;
}

/**********************************************************************
Function: SetCutoff()
Category: Modifiers
Scope: public
In: float, the new cutoff
Out: -
Description: Sets the cutoff for this condition.
**********************************************************************/
void CONDITION::SetCutoff(string NCutoff) {
  vector<CUTOFF>::iterator CurrentCutoff(Cutoffs.begin());
  vector<CUTOFF>::iterator LastCutoff(Cutoffs.end());

  for (unsigned int i=1; CurrentCutoff != LastCutoff; CurrentCutoff++) {
    if ((*CurrentCutoff).GetValue()==NCutoff) {
      CutoffNumber = i;
      return;
    }
    i++;
  }
}

/**********************************************************************
Function: Prune()
Category: Modifiers
Scope: public
In: -
Out: -
Description: Prunes cutoffs based on subsumption.
**********************************************************************/
void CONDITION::Prune() {
  //!! Cutoffs should be orderer on value and sets should be made
  vector<CUTOFF>::iterator CurrentCutoff(Cutoffs.begin());
  vector<CUTOFF>::iterator LastCutoff(Cutoffs.end());
  vector<CUTOFF>::iterator NextCutoff(Cutoffs.begin()+1);
  vector<CUTOFF>::iterator RemoveCutoff;
  bool Remove = false;

  if (Cutoffs.size()<=1) return;

  while ((NextCutoff != LastCutoff) && (CurrentCutoff != LastCutoff)) {

    // Is CP of current equal to the CP of the next Cutoff?
    if (((*CurrentCutoff).Set.CorrectPositive==(*NextCutoff).Set.CorrectPositive)||
        ((*CurrentCutoff).Set.CorrectNegative==(*NextCutoff).Set.CorrectNegative)){
      Remove = true;

      // Is CT of next higher than the CT of current?
      if(((*NextCutoff).Set.CorrectPositive+(*NextCutoff).Set.CorrectNegative)>=
         ((*CurrentCutoff).Set.CorrectPositive+(*CurrentCutoff).Set.CorrectNegative)){
        RemoveCutoff = CurrentCutoff;
      } else {
        RemoveCutoff = NextCutoff;
      }

    }

    CurrentCutoff++;
    NextCutoff++;
    if (Remove) {
      Cutoffs.erase(RemoveCutoff);
      LastCutoff = Cutoffs.end();
      Remove = false;
    }

  }  
}

/**********************************************************************
Function: ResetCutoff()
Category: Modifiers
Scope: public
In: -
Out: -
Description: Reset the cutoffs of this rulefeature.
**********************************************************************/
void CONDITION::ResetCutoff() {
  CutoffNumber = 0;
}

/**********************************************************************
Function: SortCutoffsByCP()
Category: Modifiers
Scope: public
In: -
Out: -
Description: Sort all cutoffs for this FeatureOperator based on
CorrectPositive (Sensitivity).
**********************************************************************/
void CONDITION::SortCutoffsByCP() {
  sort(Cutoffs.begin(), Cutoffs.end(), OrderByCP);
}

/**********************************************************************
Function: SortCutoffsByCT()
Category: Modifiers
Scope: public
In: -
Out: -
Description: Sort all cutoffs for this FeatureOperator based on
CorrectPositive + CorrectNegative (Accuracy).
**********************************************************************/
void CONDITION::SortCutoffsByCT() {
  sort(Cutoffs.begin(), Cutoffs.end(), OrderByCT);
}

/**********************************************************************
Function: CalculateSets()
Category: Modifiers
Scope: public
In: -
Out: -
Description: Reset the cutoffs of this rulefeature.
**********************************************************************/
void CONDITION::InitialiseSets(vector<CLASSES>* PartitionClasses) {
  vector<CUTOFF>::iterator CurrentCutoff(Cutoffs.begin());
  vector<CUTOFF>::iterator LastCutoff(Cutoffs.end());

  for (; CurrentCutoff != LastCutoff; CurrentCutoff++) {
    (*CurrentCutoff).InitialiseSet(PartitionClasses, &Operator);
  }
}

/**********************************************************************
Function: CalculatePerformance()
Category: Modifiers
Scope: public
In: enum PARTITION_TYPE, type of partition.
Out: PERFORMANCE, performance of this rulefeature.
Description: Calculate the performance of this rulefeature, including
current operator and cutoff.
**********************************************************************/
SET* CONDITION::CalculatePerformance() {
  return &Cutoffs[CutoffNumber].Set;
}

/**********************************************************************
Function: PrintPerformance()
Category: Selectors
Scope: public
In: -
Out: -
Description: Prints the current performance of this rulefeature.
**********************************************************************/
string CONDITION::PrintPerformance() {
  ostringstream Result;
//  Result << "Feature " << FeatureNumber << (*ConditionSet).PrintPerformance();
  return Result.str();
}

/**********************************************************************
Function: operator=()
Category: Operator Overloading
Scope: public
In: CONDITION, a condition to be copied
Out: CONDITION, the copied condition
Description: Operator overloading for conditions.
**********************************************************************/
CONDITION* CONDITION::operator=(CONDITION* SourceCondition) {
  FeatureNumber   = SourceCondition->FeatureNumber;
  FeatureName     = SourceCondition->FeatureName;
  FeatureOperator = SourceCondition->FeatureOperator;
    
  Cutoffs         = SourceCondition->Cutoffs;
  CutoffNumber    = SourceCondition->CutoffNumber;
  Operator        = SourceCondition->Operator;

  return this;
}

