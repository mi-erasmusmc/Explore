//----------------------------------------------------------------------------//
// $Header: //depot/Explore/Explore/set.h#14 $
// $Author: Rijnbeek $
// $DateTime: 2008/09/09 14:16:02 $
// $Change: 87 $
//----------------------------------------------------------------------------//

#ifndef SET_H
#define SET_H

#include "initialise.h"
#include "classdef.h"
#include "candidate.h"
#include <boost/dynamic_bitset.hpp>

class SET {
  private:
    vector<CLASSES>* Classes;

	string*   CutoffValue;
    OPERATOR* Operator;

    void InitialiseBags();                                                      // Creates bags of elements that do/do not conform to cutoff+operator

  public:
    int CorrectPositive;
    int FalsePositive;

    int CorrectNegative;
    int FalseNegative;

    boost::dynamic_bitset<> PositiveBag;
    boost::dynamic_bitset<> NegativeBag;

    // Constructors & destructors
    SET();
    ~SET();

    // Modifiers
    void Initialise(vector<CLASSES>* PClasses, OPERATOR* SOperator, string* SCutoffValue);

    // Selectors
    float GetSensitivity();
    float GetSpecificity();
    float GetPPV();
    float GetNPV();
    float GetAccuracy();

    void SummariseBags();                                                       // Count instances of elements in both bags

    int GetPositiveItem(unsigned int Number);                                   // Get negative indicator (included by feature+cutoff+operator or not)
    int GetNegativeItem(unsigned int Number);                                   // Get positive indicator (included by feature+cutoff+operator or not)

    OBSERVATION* GetNegativeObs(unsigned int Number);
    OBSERVATION* GetPositiveObs(unsigned int Number);        

    unsigned int GetNoObservations();                                           // Get number of observations
    int GetPositiveBagSize();
    int GetNegativeBagSize();    

    // Output
    string PrintPerformance();                                                  // Print set performance

    // Operator overloading
    SET operator&(SET& SourceSet);                                             // AND two sets
    SET operator|(SET& SourceSet);                                             // OR two sets

//    SET operator=(SET& SourceSet);

};

#endif


