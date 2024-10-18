//----------------------------------------------------------------------------//
// $Header: //depot/Explore/Explore/set.cpp#15 $
// $Author: Rijnbeek $
// $DateTime: 2008/09/09 14:16:02 $
// $Change: 87 $
//----------------------------------------------------------------------------//

#include "set.h"
#ifdef HYPER
  #include <jibu4c++.h>
#endif
#ifdef DEBUG_TIMING
extern TIMING ExploreTiming;
#endif
/**********************************************************************
Function: SET()
Category: Constructors and Destructors
Scope: public
In: -
Out: -
Description: Default constructor for a Performance.
**********************************************************************/
SET::SET() {
  CorrectPositive = 0;
  FalsePositive = 0;

  CorrectNegative = 0;
  FalseNegative = 0;
}

/**********************************************************************
Function: ~SET()
Category: Constructors and Destructors
Scope: public
In: -
Out: -
Description: Default constructor for a Performance.
**********************************************************************/
SET::~SET() {
}

/**********************************************************************
Function: Initialise()
Category: Modifiers
Scope: public
In: -
Out: -
Description: Initialises and calculates set.
**********************************************************************/
void SET::Initialise(vector<CLASSES>* PClasses, OPERATOR* SOperator, string* SCutoffValue) {
  Classes = PClasses;
  CutoffValue = SCutoffValue;
  Operator = SOperator;

  vector<CLASSES>::iterator CurrentClass((*Classes).begin());                   // Set all negative observations to 0
  NegativeBag = boost::dynamic_bitset<>((*CurrentClass).GetSize());
  CurrentClass++;
  PositiveBag = boost::dynamic_bitset<>((*CurrentClass).GetSize());

  InitialiseBags();                                                             // Initialise bags, and set observations

  SummariseBags();                                                              // Summarise the bags
}

/**********************************************************************
Function: InitialiseBags()
Category: Modifiers
Scope: private
In: -
Out: -
Description: Creates bags with elements that do or do not conform
to the Condition (Operator + Cutoff)
**********************************************************************/
void SET::InitialiseBags() {
  vector<CLASSES>::iterator CurrentClass((*Classes).begin());                   // Iterator to the first class
  vector<CLASSES>::iterator LastClass((*Classes).end());                        // Iterator to the last class (end+1)

  OBSERVATION* CurrentObservation;
  unsigned int NoClassObservations, CurrentClassNumber;

  for (; CurrentClass != LastClass; CurrentClass++) {                           // For each class
	NoClassObservations = (*CurrentClass).GetSize();
	CurrentClassNumber = (*CurrentClass).GetNumber();
	(*CurrentClass).SortOrder();                                                // Sort observations by order or performances won't match!!!

	for (unsigned int i=0; i < NoClassObservations; i++) {                      // For each observation in the class
	  if (CurrentClassNumber==0) {
		CurrentObservation = (*CurrentClass).GetObservation(i);
		if ((*Operator)==GREATER) {                                             // Based on operator check if current observation is above or below cutoff value
		  if (atof((*CurrentObservation).GetValue().c_str()) > atof((*CutoffValue).c_str())) {
			NegativeBag[i] = 1;
		  } else {
			NegativeBag[i] = 0;
		  }
		}
		if ((*Operator)==LESS){
		  if (atof((*CurrentObservation).GetValue().c_str()) <= atof((*CutoffValue).c_str())) {
			NegativeBag[i] = 1;
		  } else {
			NegativeBag[i] = 0;
		  }
		}
		if ((*Operator)==EQUAL){
		  if ((*CurrentObservation).GetValue() == (*CutoffValue)) {
			NegativeBag[i] = 1;
		  } else {
			NegativeBag[i] = 0;
		  }
		}
	  } else {
		CurrentObservation = (*CurrentClass).GetObservation(i);
		if ((*Operator)==GREATER) {                                             // Based on operator check if current observation is above or below cutoff value
		  if (atof((*CurrentObservation).GetValue().c_str()) > atof((*CutoffValue).c_str())) {
			PositiveBag[i] = 1;
		  } else {
			PositiveBag[i] = 0;
		  }
		}
		if ((*Operator)==LESS) {                                                // Based on operator check if current observation is above or below cutoff value
		  if (atof((*CurrentObservation).GetValue().c_str()) <= atof((*CutoffValue).c_str())) {
			PositiveBag[i] = 1;
		  } else {
			PositiveBag[i] = 0;
		  }
		}
		if ((*Operator)==EQUAL){
		  if ((*CurrentObservation).GetValue() == (*CutoffValue)) {
			PositiveBag[i] = 1;
		  } else {
			PositiveBag[i] = 0;
		  }
		}
	  }
	  CurrentObservation++;
	}
  }
}

/**********************************************************************
Function: SummariseBags()
Category: Modifiers
Scope: private
In: -
Out: -
Description: Counts the elements of each bag and creates performance
measures.
**********************************************************************/
void SET::SummariseBags() {
  CorrectPositive = 0;
  CorrectNegative = 0;
  FalsePositive = 0;
  FalseNegative = 0;

  CorrectPositive = PositiveBag.count();
  FalseNegative = PositiveBag.size() - PositiveBag.count();                     //  Count = count where bits are 1

  FalsePositive = NegativeBag.count();
  CorrectNegative = NegativeBag.size() - NegativeBag.count();
}

/**********************************************************************
Function: GetNegativeItem()
Category: Selectors
Scope: public
In: unsigned int, observation number
Out: unsigned int, negative observation indication
Description: Get a negative observation indication whether it is/isn't
included by the combination of feature, operator and cutoff value.
**********************************************************************/
int SET::GetNegativeItem(unsigned int Number) {
  if (Number<NegativeBag.size()) {
    return NegativeBag[Number];
  } else {
    return -1;
  }
}

/**********************************************************************
Function: GetPositiveItem()
Category: Selectors
Scope: public
In: unsigned int, observation number
Out: unsigned int, positive observation indication
Description: Get a positive observation indication whether it is/isn't
included by the combination of feature, operator and cutoff value.
**********************************************************************/
int SET::GetPositiveItem(unsigned int Number) {
  if (Number<PositiveBag.size()) {
    return PositiveBag[Number];
  } else {
    return -1;
  }
}

/**********************************************************************
Function: GetNegativeObs()
Category: Selectors
Scope: public
In: unsigned int, observation number
Out: -
Description:
**********************************************************************/
OBSERVATION* SET::GetNegativeObs(unsigned int Number) {
  vector<CLASSES>::iterator CurrentClass((*Classes).begin());
  if (Number<(*CurrentClass).GetNoObservations()) {
    return (*CurrentClass).GetObservation(Number);
  }
  OBSERVATION *Dummy = new OBSERVATION("-1",-1,-1);
  return Dummy;
}

/**********************************************************************
Function: GetPositiveObs()
Category: Selectors
Scope: public
In: unsigned int, observation number
Out: -
Description:
**********************************************************************/
OBSERVATION* SET::GetPositiveObs(unsigned int Number) {
  vector<CLASSES>::iterator CurrentClass((*Classes).begin());
  CurrentClass++;

  if (Number<(*CurrentClass).GetNoObservations()) {
    return (*CurrentClass).GetObservation(Number);
  }
  OBSERVATION *Dummy = new OBSERVATION("-1",-1,-1);
  return Dummy;
}

/**********************************************************************
Function: GetNoObservations()
Category: Selectors
Scope: public
In: -
Out: unsigned int, number of observations
Description: Returns the number of observations used for calculating
the performance (all observations in the learn partition).
**********************************************************************/
unsigned int SET::GetNoObservations() {
  return max(PositiveBag.size(),NegativeBag.size());
}

/**********************************************************************
Function: GetPositiveBagSize()
Category: Selectors
Scope: public
In: -
Out: int, number of positive observations
Description:
**********************************************************************/
int SET::GetPositiveBagSize() {
  return PositiveBag.size();
}

/**********************************************************************
Function: GetNegativeBagSize()
Category: Selectors
Scope: public
In: -
Out: int, number of negative observations
Description:
**********************************************************************/
int SET::GetNegativeBagSize() {
  return NegativeBag.size();
}

/**********************************************************************
Function: GetSensitivity()
Category: Selectors
Scope: public
In: -
Out: -
Description: Return the current sensitivity of the cutoff/operator
combination.
**********************************************************************/
float SET::GetSensitivity() {
 if ((CorrectPositive + FalseNegative) != 0) {
   return ((float)CorrectPositive / ((float)CorrectPositive + (float)FalseNegative));
 }
   return -1;
}

/**********************************************************************
Function: GetSpecificity()
Category: Selectors
Scope: public
In: -
Out: -
Description: Return the current specificity of the cutoff/operator
combination.
**********************************************************************/
float SET::GetSpecificity() {
  if ((FalsePositive + CorrectNegative) != 0) {
	return ((float)CorrectNegative / ((float)FalsePositive + (float)CorrectNegative));
  }
  else
	return -1;
}

/**********************************************************************
Function: GetPPV()
Category: Selectors
Scope: public
In: -
Out: -
Description: Return the positive predictive value of the cutoff/operator
combination.
**********************************************************************/
float SET::GetPPV() {
  if ((CorrectPositive + FalsePositive) != 0) {
	return ((float)CorrectPositive / ((float)CorrectPositive + (float)FalsePositive));
  }
  else
	return -1;
}

/**********************************************************************
Function: GetNPV()
Category: Selectors
Scope: public
In: -
Out: -
Description: Return the negative predictive value of the cutoff/operator
combination.
**********************************************************************/
float SET::GetNPV() {
  if ((FalseNegative + CorrectNegative) != 0) {
	return ((float)CorrectNegative / ((float)FalseNegative + (float)CorrectNegative));
  }
  else
	return -1;
}

/**********************************************************************
Function: GetAccuracy()
Category: Selectors
Scope: public
In: -
Out: -
Description: Return the accuracy of the cutoff/operator combination.
**********************************************************************/
float SET::GetAccuracy() {
  if ((CorrectPositive + FalsePositive + (float)FalseNegative + (float)CorrectNegative) != 0) {
	return (((float)CorrectPositive + (float)CorrectNegative) / ((float)CorrectPositive + (float)FalsePositive + (float)FalseNegative + (float)CorrectNegative));
  }
  else
	return -1;
}


/**********************************************************************
Function: GetBalancedAccuracy()
Category: Selectors
Scope: public
In: -
Out: -
Description: Return the balanced accuracy of the cutoff/operator combination.
**********************************************************************/
float SET::GetBalancedAccuracy() {
    return ((float) GetSensitivity() + (float) GetSpecificity()) / 2;
}


/**********************************************************************
Function: GetF1score()
Category: Selectors
Scope: public
In: -
Out: -
Description: Return the F1 score of the cutoff/operator combination.
**********************************************************************/
float SET::GetF1score() {
    if ((2 * (float)CorrectPositive + (float)FalsePositive + (float)FalseNegative) != 0) {

        return (2 * (float)CorrectPositive / (2 * (float)CorrectPositive + (float)FalsePositive + (float)FalseNegative));
    }
    else
        return -1;
}





/**********************************************************************
Function: PrintPerformance()
Category: Selectors
Scope: public
In: -
Out: -
Description: Print the current performance.
**********************************************************************/
string SET::PrintPerformance() {
  float Sensitivity = 100 * GetSensitivity();
  float Specificity = 100 * GetSpecificity();
//  float NPV = 100 * GetNPV();
//  float PPV = 100 * GetPPV();
//  float Accuracy = 100 * GetAccuracy();
  ostringstream Result;

  Result << "Performance:\t";
  for (unsigned int i=0; i<5; i++) {
	switch (i) {
	  case 0:
		Result << "SE: " << Sensitivity << " ";
		break;
	  case 1:
		Result << "SP: " << Specificity << " ";
		break;
//      case 2:
//        Result << "NPV: " << NPV << " ";
//        break;
//      case 3:
//        Result << "PPV: " << PPV << " ";
//        break;
//      case 4:
//        Result << "AC: " << Accuracy;
//        break;
	}
  }

  return Result.str();
}

#ifdef __APPLE__
struct AndJibu : public std::__unary_function<int, void>
#else
  struct AndJibu : public std::unary_function<int, void>
#endif
{ 
  const boost::dynamic_bitset<> Source;
  boost::dynamic_bitset<> Dest;
  AndJibu(const boost::dynamic_bitset<> Source,boost::dynamic_bitset<> Dest) :
    Source(Source), Dest(Dest){}
  void operator()(int x)
  {
    Dest[x]=Dest[x] & Source[x];
  }
};

/**********************************************************************
Function: operator+()
Category: Operator overloading
Scope: public
In: SET, set to AND with.
Out: -
Description: Function to AND (+) two sets.
**********************************************************************/
SET SET::operator&(SET& SourceSet) {
  #ifdef DEBUG_TIMING
  clock_t Start, End;
  Start = clock();
  #endif

  PositiveBag = PositiveBag & SourceSet.PositiveBag;
  NegativeBag = NegativeBag & SourceSet.NegativeBag;

  #ifdef DEBUG_TIMING
  End = clock();
  ExploreTiming.AddTime("SET::& operator normal", Start, End);
  #endif


  #ifdef DEBUG_TIMING
  Start = clock();
  #endif
  #ifdef HYPER
   int dum = PositiveBag.size();
   Jibu::forLoop(0,dum,10000,AndJibu(SourceSet.PositiveBag,PositiveBag));
  #endif
  
  #ifdef DEBUG_TIMING
  End = clock();
  ExploreTiming.AddTime("SET::& operator jibu", Start, End);
  #endif

  return *this;
}

/**********************************************************************
Function: operator*()
Category: Operator overloading
Scope: public
In: SET, set to OR with.
Out: -
Description: Function to OR (*) two sets.
**********************************************************************/
SET SET::operator|(SET& SourceSet) {

  PositiveBag = PositiveBag | SourceSet.PositiveBag;
  NegativeBag = NegativeBag | SourceSet.NegativeBag;

  return *this;
}


/*
SET SET::operator=(SET& SourceSet) {
  CorrectPositive = SourceSet.CorrectPositive;
  FalsePositive = SourceSet.FalsePositive;

  CorrectNegative = SourceSet.CorrectNegative;
  FalseNegative = SourceSet.FalseNegative;

  Classes = SourceSet.Classes;
  CutoffValue = SourceSet.CutoffValue;
  Operator = SourceSet.Operator;

  PositiveBag = SourceSet.PositiveBag;
  NegativeBag = SourceSet.NegativeBag;

  return *this;
} */

