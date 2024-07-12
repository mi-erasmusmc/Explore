//----------------------------------------------------------------------------//
// $Header: //depot/Explore/Explore/condition.h#16 $
// $Author: Rijnbeek $
// $DateTime: 2008/09/09 14:16:02 $
// $Change: 87 $
//----------------------------------------------------------------------------//

#ifndef CONDITION_H
#define CONDITION_H

#include "initialise.h"
#include "feature.h"
#include "set.h"

class CONDITION {
  public:
    // Structure to sort observations based on their class
    struct SortByCP {
      bool operator()(const CUTOFF& a, const CUTOFF& b) {
        return a.Set.CorrectPositive > b.Set.CorrectPositive;
      }
    } OrderByCP;

    struct SortByCT {
      bool operator()(const CUTOFF& a, const CUTOFF& b) {
        return (a.Set.CorrectPositive + a.Set.CorrectNegative) > (b.Set.CorrectPositive + b.Set.CorrectNegative);
      }
    } OrderByCT;

    // Public properties
    unsigned int FeatureNumber;
    string FeatureName;

    unsigned int FeatureOperator;

    vector<CUTOFF> Cutoffs;
    unsigned int CutoffNumber;
    OPERATOR Operator;

    // ------------------------- CutoffSet Optimization ------------------------

    // FeatureOperator
    bool IsSolo;                                                                // FeatureOperator occurs within rule on it's own
    bool NonSoloIncluded;                                                       // FeatureOperator included in a conjunction with size>1

    // Feature
    bool RepeatedFeature;

    // Condition
    bool PreviousSame;                                                          // Feature is equal to previous feature within conjunction (left one)
    bool NextSame;                                                              // Feature is equal to next feature (right one)
    bool StopNext;                                                              // Stop when next cutoff increase

    //--------------------------------------------------------------------------

    unsigned int NoOperators;

    //--------------------------------------------------------------------------

    // Constructors and destructors
    CONDITION();
    CONDITION(unsigned int CNumber, string CName, vector<CUTOFF> CCutoffs, OPERATOR COperator, unsigned int FOperator);
    ~CONDITION();

    // Modifiers
    void SetCutoff(string NCutoff);
    void NextCutoff();                                                          // Use next cutoff of this feature
    void ResetCutoff();                                                         // Reset cutoff value + operator
    void Prune();                                                               // Prune based on subsumption


    void SortCutoffsByCP();                                                     // Sort cutoffs by CorrectPositive
    void SortCutoffsByCT();                                                     // Sort cutoffs by CorrectPositive + CorrectNegative

    void InitialiseSets(vector<CLASSES>* PartitionClasses);                     // Set partition type conditions have to be tested on
    SET* CalculatePerformance();                                                // Calculate feature performance on the current Learn or Test partition

    // Operator overloading
    CONDITION* operator=(CONDITION* SourceCondition);                           // Operator overloading    

    // Output
    string PrintPerformance();                                                  // Print the performance of this condition
};

#endif
