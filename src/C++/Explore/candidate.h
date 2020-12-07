//----------------------------------------------------------------------------//
// $Header: //depot/Explore/Explore/candidate.h#15 $
// $Author: Rijnbeek $
// $DateTime: 2008/09/09 14:16:02 $
// $Change: 87 $
//----------------------------------------------------------------------------//

#ifndef CANDIDATE_H
#define CANDIDATE_H

#include "initialise.h"
#include "performance.h"

class CANDIDATE {
  private:
    bool InterpretRule(string StringRule);                                      // Interpret a rule represented as a string
    bool InterpretCombinations(string StringCombinations);                      // Interpret a list of combination sizes as a string

  public:
    vector<string>        FeatureNames;
    vector<unsigned int>  Conjunctions;                                         // Conjunctions of the rule
    vector<unsigned int>  Features;                                             // Features of the rule
    vector<OPERATOR>      Operators;                                            // Operators of the rule
    vector<string>         Cutoffs;                                              // Cutoffs of the rule

    PERFORMANCE Performance;                                                    // Performance of this rule

    // Modifiers
    void Clear();
    CANDIDATE* operator=(CANDIDATE* SourceCandidate);
    bool Interpret(string StringRule);

    // Selectors
    unsigned int Size();
    bool IsValid();
    
    // Output
    string Print();
    string PrintRule();
    string PrintRuleIndexed();
    string PrintRuleAsNumber();
    string PrintFeatureSetAsNumber();        
    string PrintCombination();
    string PrintPerformance(bool headers);

    bool operator<(const CANDIDATE& CompareCandidate);
};

#endif

