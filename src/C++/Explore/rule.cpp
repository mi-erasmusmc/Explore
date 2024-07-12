//----------------------------------------------------------------------------//
// $Header$
// $Author$
// $DateTime$
// $Change$
//----------------------------------------------------------------------------//

#include "rule.h"
#include "../common.h"
#include <sstream>
#include <tbb/concurrent_vector.h>
#include <tbb/parallel_for.h>
#ifdef DEBUG_TIMING
extern TIMING ExploreTiming;
#endif

/**********************************************************************
Function: RULE()
Category: Constructors and Destructors
Scope: public
In: -
Out: -
Description: Default constructor for a rule.
**********************************************************************/
RULE::RULE() {
    LastCombination = false;

    RestrictionSet = false;
    MandatorySet = false;

    BranchBound = false;
    BinaryReduction = false;

    CombinationGenerated = false;
    FeatureSetGenerated = false;
    CutoffSetGenerated = false;

    CombinationsGenerated = 0;
    FeatureSetsGenerated = 0;
    CutoffSetsGenerated = 0;

    CountCandidates = 0;

    CurrentLength = NULL;
    MinLength = NULL;
    MaxLength = NULL;

    CPBest = 0;
    CTBest = 0;
    FPConstraint = 0;
}

/**********************************************************************
Function: ~RULE()
Category: Constructors and Destructors
Scope: public
In: -
Out: -
Description: Destructor for a rule.
**********************************************************************/
RULE::~RULE() {
}



/**********************************************************************
Function: Size()
Category: Selectors
Scope: public
In: -
Out: unsigned int, number of conjunctions.
Description: Returns the number of conjunctions of the rule.
**********************************************************************/
unsigned int RULE::Size() {
    return Conjunctions.size();
}

/**********************************************************************
Function: GetCurrentRuleLength()
Category: Selectors
Scope: public
In: -
Out: unsigned, rule length
Description: Returns the current length of the rule.
**********************************************************************/
unsigned int RULE::GetCurrentRuleLength() {
    return CurrentLength;
}

/**********************************************************************
Function: GetMaxRuleLength()
Category: Selectors
Scope: public
In: -
Out: unsigned, maximum rule length
Description: Returns the maximum length of the rule.
**********************************************************************/
unsigned int RULE::GetMaxRuleLength() {
    return MaxLength;
}

/**********************************************************************
Function: GetMinRuleLength()
Category: Selectors
Scope: public
In: -
Out: unsigned, minimum rule length
Description: Returns the minimum length of the rule.
**********************************************************************/
unsigned int RULE::GetMinRuleLength() {
    return MinLength;
}

/**********************************************************************
Function: GetCombinationGenerated()
Category: Selectors
Scope: public
In: -
Out: bool
Description: Returns the bool CombinationGenerated
**********************************************************************/
bool RULE::IsCombinationGenerated() {
    return CombinationGenerated;
}

/**********************************************************************
Function: GetFeatureSetGenerated()
Category: Selectors
Scope: public
In: -
Out: bool
Description: Returns the bool FeatureSetGenerated
**********************************************************************/
bool RULE::IsFeatureSetGenerated() {
    return FeatureSetGenerated;
}

/**********************************************************************
Function: GetCutoffSetGenerated()
Category: Selectors
Scope: public
In: -
Out: bool
Description: Returns the bool CutoffSetGenerated
**********************************************************************/
bool RULE::IsCutoffSetGenerated() {
    return CutoffSetGenerated;
}

/**********************************************************************
Function: PrintSettings()
Category: Selectors
Scope: public
In: -
Out: -
Description: Prints the settings for the rule.
**********************************************************************/
void RULE::PrintSettings() {
    PrintRestriction();
    cout << endl << "RULE SETTINGS" << endl << endl;
    cout << "Minimum length: " << MinLength << endl;
    cout << "Maximum length: " << MaxLength << endl;
}

/**********************************************************************
Function: PrintCombination()
Category: Selectors
Scope: public
In: -
Out: -
Description: Prints the current combination.
**********************************************************************/
void RULE::PrintCombination() {
    cout << "Term tuple: ";
    for (unsigned int i=0; i<Conjunctions.size(); i++) {
        if (i) cout << " ";
        cout << Conjunctions[i].Size;
    }
    cout << endl;
}

/**********************************************************************
Function: PrintFeatureSet()
Category: Selectors
Scope: public
In: -
Out: -
Description: Prints the current featureset.
**********************************************************************/
void RULE::PrintFeatureSet_Thread() {
    std::stringstream thread;
    for (unsigned int i=0; i<Conjunctions.size(); i++) {
        if (i) thread << " OR ";
        for (unsigned int j=0; j<Conjunctions[i].Conditions.size(); j++) {
            if (j) thread << " AND ";
//      thread << Conjunctions[i].Conditions[j].FeatureName;
            thread << Conjunctions[i].Conditions[j].FeatureNumber+1;
            thread << " " << PrintOperator(Conjunctions[i].Conditions[j].Operator);
        }
    }
    thread << endl;

    cout << thread.str();
}



/**********************************************************************
Function: PrintFeatureSet()
Category: Selectors
Scope: public
In: -
Out: -
Description: Prints the current featureset.
**********************************************************************/
void RULE::PrintFeatureSet() {
    for (unsigned int i=0; i<Conjunctions.size(); i++) {
        if (i) cout << " OR ";
        for (unsigned int j=0; j<Conjunctions[i].Conditions.size(); j++) {
            if (j) cout << " AND ";
//      cout << Conjunctions[i].Conditions[j].FeatureName;
            cout << Conjunctions[i].Conditions[j].FeatureNumber+1;
            cout << " " << PrintOperator(Conjunctions[i].Conditions[j].Operator);
        }
    }
    cout << endl;
}

/**********************************************************************
Function: PrintCutoffSet()
Category: Selectors
Scope: public
In: -
Out: string, the conditionset
Description: Prints the current conditionset.
**********************************************************************/
void RULE::PrintCutoffSet() {
#ifdef DEBUG_TIMING
    clock_t Start, End;
  Start = clock();
#endif
    for (unsigned int i=0; i<Conjunctions.size(); i++) {
        if (i>0) cout << " OR  ";
        for (unsigned int j=0; j< Conjunctions[i].Conditions.size(); j++) {
            if (j>0) cout << " AND ";
            cout << Conjunctions[i].Conditions[j].FeatureName << " ";
            cout << PrintOperator(Conjunctions[i].Conditions[j].Operator) << " ";
            cout << Conjunctions[i].Conditions[j].Cutoffs[Conjunctions[i].Conditions[j].CutoffNumber].GetValue();
//      cout << " [CP:" << Conjunctions[i].Conditions[j].Cutoffs[Conjunctions[i].Conditions[j].CutoffNumber].Set.CorrectPositive  << "]";

//      cout << " [";
//      if (Conjunctions[i].Conditions[j].PreviousSame) cout << "1";
//      else cout << "0";
//      cout << ",";
//      if (Conjunctions[i].Conditions[j].NextSame) cout << "1";
//      else cout << "0";
//      cout << "] ";
        }
//    cout << " [" << Conjunctions[i].ConjunctionSet.FalsePositive << "]";
//    cout << " [CP:" << Conjunctions[i].ConjunctionSet.CorrectPositive << ",FP:" << Conjunctions[i].ConjunctionSet.FalsePositive << "]";
//    cout << "[" << Conjunctions[i].Changed << "] ";
    }
//  cout << " [FPC:" << FPConstraint << ",CPB:" << CPBest << "]";
    cout << endl;

#ifdef DEBUG_TIMING
    End = clock();
  ExploreTiming.AddTime("RULE::PrintCutoffSet", Start, End);
#endif

}

/**********************************************************************
Function: PrintRuleString()
Category: Selectors
Scope: public
In: -
Out: string, the rule
Description: Prints the current rule and returns this as a string.
**********************************************************************/
string RULE::PrintRuleString() {
#ifdef DEBUG_TIMING
    clock_t Start, End;
  Start = clock();
#endif

    ostringstream Result;

    for (unsigned int i=0; i<Conjunctions.size(); i++) {
        if (i>0) {
            Result << " OR  ";
        }
        for (unsigned int j=0; j< Conjunctions[i].Conditions.size(); j++) {
            if (j>0) {
                Result << " AND ";
            }
            Result << Conjunctions[i].Conditions[j].FeatureName << " ";
            Result << PrintOperator(Conjunctions[i].Conditions[j].Operator) << " ";
            Result << Conjunctions[i].Conditions[j].Cutoffs[Conjunctions[i].Conditions[j].CutoffNumber].GetValue();
        }
    }

#ifdef DEBUG_TIMING
    End = clock();
  ExploreTiming.AddTime("RULE::PrintRuleString", Start, End);
#endif

    return Result.str();
}

/**********************************************************************
Function: PrintPerformance()
Category: Selectors
Scope: public
In: -
Out: -
Description: Prints the current performance of the rule.
**********************************************************************/
void RULE::PrintPerformance() {
#ifdef DEBUG_TIMING
    clock_t Start, End;
  Start = clock();
#endif

    cout << RuleSet.PrintPerformance();

#ifdef DEBUG_TIMING
    End = clock();
  ExploreTiming.AddTime("RULE::PrintPerformance", Start, End);
#endif

    cout << endl;
}

/**********************************************************************
Function: PrintSets
Category: Selectors
Scope: public
In: -
Out: -
Description:
**********************************************************************/
void RULE::PrintSets() {
    vector<CONJUNCTION>::iterator CurrentConjunction;
    vector<CONJUNCTION>::iterator LastConjunction;

    vector<CONDITION>::iterator CurrentCondition;
    vector<CONDITION>::iterator LastCondition;

    CUTOFF* CurrentCutoff;

    unsigned int MaxSize = RuleSet.PositiveBag.size();
    if (MaxSize<RuleSet.NegativeBag.size()) {
        MaxSize = RuleSet.NegativeBag.size();
    }

    for (int i=-1; i<(int)MaxSize; i++) {
        CurrentConjunction = Conjunctions.begin();
        LastConjunction = Conjunctions.end();
        for (unsigned int j=1; CurrentConjunction != LastConjunction; CurrentConjunction++) {
            CurrentCondition = CurrentConjunction->Conditions.begin();
            LastCondition = CurrentConjunction->Conditions.end();
            for (; CurrentCondition != LastCondition; CurrentCondition++) {
                if (i==-1) cout << "F" << CurrentCondition->FeatureNumber << "\t";
                else {
                    CurrentCutoff = &CurrentCondition->Cutoffs[CurrentCondition->CutoffNumber];
                    if (i<(int)CurrentCutoff->Set.PositiveBag.size()) {
                        cout << CurrentCutoff->Set.PositiveBag[i] << " ";
                    } else {
                        cout << "- ";
                    }
                    if (i<(int)CurrentCutoff->Set.NegativeBag.size()) {
                        cout << CurrentCutoff->Set.NegativeBag[i] << "\t";
                    } else {
                        cout << "-\t";
                    }
                }
            }
            if (i==-1) cout << "C" << j << "\t";
            else {
                if (i<(int)CurrentConjunction->ConjunctionSet.PositiveBag.size()) {
                    cout << CurrentConjunction->ConjunctionSet.PositiveBag[i] << " ";
                } else {
                    cout << "- ";
                }
                if (i<(int)CurrentConjunction->ConjunctionSet.NegativeBag.size()) {
                    cout << CurrentConjunction->ConjunctionSet.NegativeBag[i] << "\t";
                } else {
                    cout << "-\t";
                }
            }
            j++;
        }
        if (i==-1) cout << "R" << endl;
        else {
            if (i<(int)RuleSet.PositiveBag.size()) {
                cout << RuleSet.PositiveBag[i] << " ";
            } else {
                cout << "- ";
            }
            if (i<(int)RuleSet.NegativeBag.size()) {
                cout << RuleSet.NegativeBag[i] << endl;
            } else {
                cout << endl;
            }
        }
    }
}

/**********************************************************************
Function: PrintFeatureOperators()
Category: Selectors
Scope: public
In: -
Out: -
Description: Prints all FeatureOperators of the rule.
**********************************************************************/
void RULE::PrintFeatureOperators() {
    cout.precision(4);
    cout.width(4);
    unsigned int NoCutoffs = 0;

    cout << endl << "FEATURE OPERATORS" << endl << endl;

    for (unsigned int i=0; i<FeatureOperators.size(); i++) {
        cout << FeatureOperators[i].FeatureName << " ";
        cout << PrintOperator(FeatureOperators[i].Operator) << " ";
        cout << "(" << FeatureOperators[i].Cutoffs.size() << " cutoffs)" << endl;
        cout << "\tCutoff\t\tCP\tFP\tCT" << endl;

        for (unsigned int j=0; j<FeatureOperators[i].Cutoffs.size(); j++) {
            cout << "\t" << FeatureOperators[i].Cutoffs[j].GetValue();
            cout << "\t\t" << FeatureOperators[i].Cutoffs[j].Set.CorrectPositive;
            cout << "\t" << FeatureOperators[i].Cutoffs[j].Set.FalsePositive;
            cout << "\t" << (FeatureOperators[i].Cutoffs[j].Set.CorrectPositive + FeatureOperators[i].Cutoffs[j].Set.CorrectNegative);
            cout << endl;
            NoCutoffs++;
        }
    }
    cout << "Total number of cutoffs: " << NoCutoffs << endl;
}

/**********************************************************************
Function: PrintRestrictions()
Category: Selectors
Scope: public
In: -
Out: -
Description: Prints any restrictions set.
**********************************************************************/
void RULE::PrintRestriction() {
    cout << endl << "RESTRICTION" << endl << endl;
    cout << Restriction.Print();
}

/**********************************************************************
Function: PrintCumulativeSets()
Category: Selectors
Scope: public
In: -
Out: -
Description: Prints the cumulative sets of all conjunctions within
this rule.
**********************************************************************/
void RULE::PrintCumulativeSets() {
    for (unsigned int i=0; i<Conjunctions.size(); i++) {
        cout << "[" << Conjunctions[i].CumulativeSet.CorrectPositive << "] ";
    }
    cout << endl;
}

/**********************************************************************
Function: GetROC()
Category: Selectors
Scope: public
In: unsigned int, rulelength
Out: vector<double>*, the roc curve
Description: Returns the ROC Curve for the given rulelength.
**********************************************************************/
/*
vector<double>* RULE::GetROC(unsigned int RRuleLength) {
  return ROCCurves[RRuleLength-MinLength].GetROC();
} */

/**********************************************************************
Function: GetCombination()
Category: Selectors
Scope: public
In: -
Out: string, current combination
Description: Returns the current combination as a string
**********************************************************************/
string RULE::GetCombination() {
    ostringstream Result;
    for (unsigned int i=0; i<Conjunctions.size(); i++) {
        if (i) Result << " ";
        Result << "(" << Conjunctions[i].Size << ")";
    }
    return Result.str();
}

/**********************************************************************
Function: GetFeatureSet()
Category: Selectors
Scope: public
In: -
Out: string, current featureset
Description: Returns the current featureset as a string
**********************************************************************/
string RULE::GetFeatureSet() {
    ostringstream Result;
    for (unsigned int i=0; i<Conjunctions.size(); i++) {
        if (i) Result << " OR ";
        for (unsigned int j=0; j<Conjunctions[i].Conditions.size(); j++) {
            if (j) Result << " AND ";
            Result << Conjunctions[i].Conditions[j].FeatureNumber+1;
            Result << " " << PrintOperator(Conjunctions[i].Conditions[j].Operator);
        }
    }
    return Result.str();
}

/**********************************************************************
Function: IsLastRuleLengthCombination()
Category: Selectors
Scope: public
In: -
Out: bool, last/not last combination for the current rulelength
Description: Returns true if the current combination is the last for
the current rulelength being processed.
**********************************************************************/
bool RULE::IsLastRuleLengthCombination() {
    vector<CONJUNCTION>::iterator CurrentConjunction(Conjunctions.begin());
    vector<CONJUNCTION>::iterator LastConjunction(Conjunctions.end());

    for (; CurrentConjunction != LastConjunction; CurrentConjunction++) {
        if ((*CurrentConjunction).Size>1) return false;
    }
    return true;
}

/**********************************************************************
Function: GetConjunctions()
Category: Selectors
Scope: public
In: -
Out: list<unsigned int>, the list of Conjunctions
Description: Returns a list of conjunctions of the current rule.
**********************************************************************/
vector<unsigned int> RULE::GetConjunctions() {
    vector<unsigned int> Dummy;

    for (unsigned int i=0; i < Conjunctions.size(); i++) {
        Dummy.push_back(Conjunctions[i].Conditions.size());
    }
    return Dummy;
}

/**********************************************************************
Function: GetFeature()
Category: Selectors
Scope: public
In: -
Out: list<unsigned int>, the list of features
Description: Returns a list of features.
**********************************************************************/
vector<unsigned int> RULE::GetFeatures() {
    vector<unsigned int> Features;

    for (unsigned int i=0; i<Conjunctions.size(); i++) {
        for (unsigned int j=0; j<Conjunctions[i].Conditions.size(); j++) {
            Features.push_back(Conjunctions[i].Conditions[j].FeatureNumber);
        }
    }
    return Features;
}

/**********************************************************************
Function: GetFeatureNames()
Category: Selectors
Scope: public
In: -
Out: list<string>, the list of featurenames
Description: Returns a list of featurenames.
**********************************************************************/
vector<string> RULE::GetFeatureNames() {
    vector<string> Features;

    for (unsigned int i=0; i<Conjunctions.size(); i++) {
        for (unsigned int j=0; j<Conjunctions[i].Conditions.size(); j++) {
            Features.push_back(Conjunctions[i].Conditions[j].FeatureName);
        }
    }
    return Features;
}

/**********************************************************************
Function: GetOperatorList()
Category: Selectors
Scope: public
In: -
Out: list<OPERATOR>, the list of operators
Description: Returns a list of operators.
**********************************************************************/
vector<OPERATOR> RULE::GetOperators() {
    vector<OPERATOR> Operators;

    for (unsigned int i=0; i<Conjunctions.size(); i++) {
        for (unsigned int j=0; j<Conjunctions[i].Conditions.size(); j++) {
            Operators.push_back(Conjunctions[i].Conditions[j].Operator);
        }
    }
    return Operators;
}

/**********************************************************************
Function: GetCutoffList()
Category: Selectors
Scope: public
In: -
Out: list<float> the list of cutoffs
Description: Returns a list of cutoffs.
**********************************************************************/
vector<string> RULE::GetCutoffs() {
    vector<string> Cutoffs;

    for (unsigned int i=0; i<Conjunctions.size(); i++) {
        for (unsigned int j=0; j<Conjunctions[i].Conditions.size(); j++) {
            Cutoffs.push_back(Conjunctions[i].Conditions[j].Cutoffs[Conjunctions[i].Conditions[j].CutoffNumber].GetValue());
        }
    }
    return Cutoffs;
}

/**********************************************************************
Function: GetCutoffList()
Category: Selectors
Scope: public
In: unsigned int, order of the featureoperator
Out: unsigned int, the minimum order of a cutoff
Description: Returns the order of the minimal cutoff for a specific
FeatureOperator currently used in the rule.
**********************************************************************/
unsigned int RULE::GetMinCutoff(unsigned int Fnum) {
    CONDITION* CurrentCondition;
    unsigned int Result = Features[0][Fnum].Cutoffs.size();

    for (unsigned int i=0; i<Conjunctions.size(); i++) {
        for (unsigned int j=0; j<Conjunctions[i].Conditions.size(); j++) {
            CurrentCondition = &Conjunctions[i].Conditions[j];
            if (CurrentCondition->FeatureNumber==Fnum && (Conjunctions[i].Size>1 || FeatureOperators[CurrentCondition->FeatureOperator].RepeatedFeature)) {
                if (CurrentCondition->CutoffNumber<Result) {
                    Result = CurrentCondition->CutoffNumber;
                }
            }
        }
    }
    return Result;
}

/**********************************************************************
Function: CutoffsAtMax()
Category: Selectors
Scope: public
In:
Out: boolean
Description: Returns whether all cutoffs currently used in the rule are at the maximum.
**********************************************************************/
bool RULE::CutoffsAtMax(int ConjunctionNr, int ConditionNr) {
    CONDITION* CurrentCondition;

    for (unsigned int i=ConjunctionNr; i<Conjunctions.size(); i++) {
        for (unsigned int j=ConditionNr; j<Conjunctions[i].Conditions.size(); j++) {
            CurrentCondition = &Conjunctions[i].Conditions[j];
            if (CurrentCondition->CutoffNumber<CurrentCondition->Cutoffs.size()-1) {
                return false;
            }
        }
    }
    return true;
}
/**********************************************************************
Function: GetMaxCutoff()
Category: Selectors
Scope: public
In: unsigned int, order of the featureoperator
Out: unsigned int, the maximum order of a cutoff
Description: Returns the order of the maximum cutoff for a specific
FeatureOperator currently used in the rule.
**********************************************************************/
unsigned int RULE::GetMaxCutoff(unsigned int FOperator) {
    CONDITION* CurrentCondition;
    unsigned int Result = 0;

    for (unsigned int i=0; i<Conjunctions.size(); i++) {
        for (unsigned int j=0; j<Conjunctions[i].Conditions.size() && Conjunctions[i].Size>1; j++) {
            CurrentCondition = &Conjunctions[i].Conditions[j];
            if (CurrentCondition->FeatureOperator==FOperator) {
                if (CurrentCondition->CutoffNumber>Result) {
                    Result = CurrentCondition->CutoffNumber;
                }
            }
        }
    }
    return Result;
}

/**********************************************************************
Function: GetPartitionsGenerated()
Category: Selectors
Scope: public
In: -
Out: double, number of partitions
Description: Returns the number of partitions generated.
**********************************************************************/
unsigned int RULE::GetCombinationsGenerated() {
    return CombinationsGenerated;
}

/**********************************************************************
Function: GetFeatureSetsGenerated()
Category: Selectors
Scope: public
In: -
Out: double, number of featuresets
Description: Returns the number of featuresets generated.
**********************************************************************/
long RULE::GetFeatureSetsGenerated() {
    return FeatureSetsGenerated;
}

/**********************************************************************
Function: GetCutoffSetsGenerated()
Category: Selectors
Scope: public
In: -
Out: double, number of instances
Description: Returns the number of instances generated.
**********************************************************************/
long RULE::GetCutoffSetsGenerated() {
    return CutoffSetsGenerated;
}


/**********************************************************************
Function: GetCountCandidates()
Category: Selectors
Scope: public
In: -
Out: double, number of instances
Description: Returns the number of candidates found.
**********************************************************************/
long RULE::GetCountCandidates() {
    return CountCandidates;
}

/**********************************************************************
Function: GetCombinationsTotal()
Category: Selectors
Scope: public
In: -
Out: unsigned long, the total number of combinations
Description: Returns the total number of combinations.
**********************************************************************/
unsigned int RULE::GetCombinationsTotal() {
    return CombinationsTotal;
}

/**********************************************************************
Function: GetTotalComplexityLimit()
Category: Selectors
Scope: public
In: -
Out: unsigned long, the total number of rules limit
Description: Returns the total number of rules limit
**********************************************************************/
double RULE::GetTotalComplexityLimit() {
    return TotalComplexityLimit;
}
// EXPONENTIAL_WORKUNITS and EXPONENTIAL_BASE must be defined and accessible from rule.cpp
// if weighted complexities have to be calculated

double RULE::GetExponentialComplexityLimit() {
    return TotalExponentialLimit;
}


/**********************************************************************
Function: GetTestMode()
Category: Modifiers
Scope: public
In:
Out: PARTITION_TYPE, partition type rules should be tested on
Description: Rule must cout skipped conditionsets that it generates.
**********************************************************************/
PARTITION_TYPE RULE::GetTestMode() {
    return TestMode;
}

/**********************************************************************
Function: RuleComplexity()
Category: Selectors
Scope: public
In: -
Out: long double, complexity of the rule.
Description: Calculates the complexity of a given rulelength.
**********************************************************************/
double RULE::RuleComplexity(unsigned int NoPartitions) {
#ifdef DEBUG_TIMING
    clock_t Start, End;
  Start = clock();
#endif

    Reset();                                                                      // Reset the rule

    double RuleComplexity=0;
    double FeatureSetComplexity=0;
    TotalComplexityLimit=0;
    CombinationsTotal=0;
#if defined(EXPONENTIAL_WORKUNITS)
    TotalExponentialLimit=0;
#endif

    while (NextCombination()) { // TODO: check if should be NextCombinationGenerator()
        RuleComplexity += CombinationComplexity();
        while (NextFeatureSet(0, GetFeatureOperatorSize())) { // TODO: check if should be NextFeatureSetGenerator()
#if defined(EXPONENTIAL_WORKUNITS)
            FeatureSetComplexity = FeatureSetComplexityLimit();
      TotalExponentialLimit += (FeatureSetComplexity / pow(EXPONENTIAL_BASE,(Conjunctions.size()-1)));
      TotalComplexityLimit  += FeatureSetComplexity;
#else
            TotalComplexityLimit += FeatureSetComplexityLimit();
#endif
        }
        CombinationsTotal++;
    }

    RuleComplexity    *= NoPartitions;
    CombinationsTotal *= NoPartitions;

    Reset();                                                                      // Reset the rule (currently on last combination)

    LastCombination = false;

    CombinationsGenerated = 0;
    FeatureSetsGenerated = 0;
    CutoffSetsGenerated = 0;

    CountCandidates = 0;

#ifdef DEBUG_TIMING
    End = clock();
  ExploreTiming.AddTime("RULE::Initialise", Start, End);
#endif

    return RuleComplexity;
}


/**********************************************************************
Function: CombinationsTotal()
Category: Selectors
Scope: public
In: -
Out: long double, complexity of the rule.
Description: Calculates the complexity of a given rulelength.
**********************************************************************/
int RULE::CalculateCombinationsTotal() {
    while (NextCombinationGenerator()) {
        CombinationsTotal++;
    }

    Reset();                                                                      // Reset the rule

    return (int) CombinationsTotal;
}

/**********************************************************************
Function: CombinationComplexity()
Category: Selectors
Scope: public
In: -
Out: long double, complexity of the partition.
Description: Calculates the complexity of a single combination.
**********************************************************************/
double RULE::CombinationComplexity() {                                          // Calculate complexity of current combination
#ifdef DEBUG_TIMING
    clock_t Start, End;
  Start = clock();
#endif

    unsigned int LastConjunctionSize,SameConjunctionSize,ConjunctionSize;
    double TotalComplexity;

    TotalComplexity = 1;
    LastConjunctionSize = SameConjunctionSize = ConjunctionSize = 0;

    // In case there are more Conjunctions than the number of FeatureOperators available
    if (Conjunctions.size() > FeatureOperators.size()) return 0;

    // For each conjunction
    for (unsigned int i=0; i<Conjunctions.size(); i++) {
        ConjunctionSize = Conjunctions[i].Size;
        // In case we can't fit all FeatureOperators in the current Conjunction of the Combination.
        if (ConjunctionSize > FeatureOperators.size()) {
            return 0;
        }
        // Has the combination size changed?
        if ((ConjunctionSize != LastConjunctionSize) && (LastConjunctionSize != 0)) {
            TotalComplexity *= ConjunctionComplexity(LastConjunctionSize,FeatureOperators.size(),SameConjunctionSize);
            SameConjunctionSize = 1;
        } else SameConjunctionSize++;
        LastConjunctionSize = ConjunctionSize;
    }
    TotalComplexity *= ConjunctionComplexity(LastConjunctionSize,FeatureOperators.size(),SameConjunctionSize);

#ifdef DEBUG_TIMING
    End = clock();
  ExploreTiming.AddTime("RULE::CombinationComplexity", Start, End);
#endif

    return TotalComplexity;
}

/**********************************************************************
Function: ConjunctionComplexity()
Category: Selectors
Scope: private
In: unsigned int, size of the conjunction
    unsigned int, number of features
    unsigned int, number of conjunctions
Out: long double, complexity of the conjunction
Description: Calculates the complexity of a conjunction.
**********************************************************************/
double RULE::ConjunctionComplexity(unsigned int ConjunctionSize, unsigned int NoFeatureOperators, unsigned int NoConjunctions) {
#ifdef DEBUG_TIMING
    clock_t Start, End;
  Start = clock();
#endif

    double Result=0;

    if (NoConjunctions==1) {
        Result = Binomial(NoFeatureOperators,ConjunctionSize);
//    Result = bico(NoFeatureOperators,ConjunctionSize);
    } else {
        if (ConjunctionSize==1) {
            Result = Binomial(NoFeatureOperators,NoConjunctions);
//      Result = bico(NoFeatureOperators,ConjunctionSize);
        } else {
            long Dummy = NoConjunctions+Binomial(NoFeatureOperators,ConjunctionSize)-1;
//      long Dummy = NoConjunctions+bico(NoFeatureOperators,ConjunctionSize)-1;
            Result = Binomial(Dummy,NoConjunctions);
//      Result = bico(Dummy,NoConjunctions);
        }
    }

#ifdef DEBUG_TIMING
    End = clock();
  ExploreTiming.AddTime("RULE::ConjunctionComplexity", Start, End);
#endif

    return Result;
}

/**********************************************************************
Function: Binomial()
Category: Selectors
Scope: private
In: unsigned int, first coefficient.
    unsigned int, second coefficient.
Out: long double, the binomial.
Description: Calculates the binomial of two coefficients.
**********************************************************************/
/*
double RULE::Binomial(unsigned int a,unsigned int b) {
#ifdef DEBUG_TIMING
  clock_t Start, End;
  Start = clock();
#endif

  double Result=0;

  Result = Faculty(a)/(Faculty(b)*Faculty(a-b));

#ifdef DEBUG_TIMING
  End = clock();
  ExploreTiming.AddTime("RULE::Binomial", Start, End);
#endif

  return Result;
} */

double RULE::Binomial(unsigned int a, unsigned int b) {
    if (a < b) return 0;
    else if (!b|| a == b) return 1;
    else return (Binomial(a-1, b)*a)/(a-b);
}


/**********************************************************************
Function: Faculty()
Category: Selectors
Scope: private
In: unsigned int, value for which the faculty has to be calculated for.
Out: long double, the faculty.
Description: Calculates the faculty of a specific value.
**********************************************************************/
long double RULE::Faculty(long double a) {
#ifdef DEBUG_TIMING
    clock_t Start, End;
  Start = clock();
#endif

    long double Result=1;

    for (; a!=0; a--) {
        Result = Result * a;
    }

#ifdef DEBUG_TIMING
    End = clock();
  ExploreTiming.AddTime("RULE::Faculty", Start, End);
#endif

    return Result;
}
/**********************************************************************
Function: FeatureSetComplexityLimit()
Category: Selectors
Scope: private
In: -
Out: long double, upper limit for complexity of the current featureset
Description: Calculates the upper-limit complexity of a featureset.
**********************************************************************/
double RULE::FeatureSetComplexityLimit() {
    vector<CONJUNCTION>::iterator CurrentConjunction(Conjunctions.begin());
    vector<CONJUNCTION>::iterator LastConjunction(Conjunctions.end());
    vector<CONDITION>::iterator CurrentCondition;
    vector<CONDITION>::iterator LastCondition;
    double FeatureSetLimit = 0;

    for (; CurrentConjunction != LastConjunction; CurrentConjunction++) {
        CurrentCondition = (*CurrentConjunction).Conditions.begin();
        LastCondition = (*CurrentConjunction).Conditions.end();
        for (; CurrentCondition != LastCondition; CurrentCondition++) {
            if (FeatureSetLimit==0) {
                if ((*CurrentCondition).Cutoffs.size()>0) {
                    FeatureSetLimit = (*CurrentCondition).Cutoffs.size();
                }
            } else {
                if ((*CurrentCondition).Cutoffs.size()>0) {
                    FeatureSetLimit = FeatureSetLimit*(*CurrentCondition).Cutoffs.size();
                }
            }
        }
    }
    return FeatureSetLimit;
}

/**********************************************************************
Function: CalculatePerformance()
Category: Selectors
Scope: public
In: enum PARTITION_TYPE, type of partition.
Out: PERFORMANCE, current performance of the rule.
Description: Calculates the performance of the current rule on the
learn or test partition.
**********************************************************************/
PERFORMANCE RULE::CalculatePerformance() {
#ifdef DEBUG_TIMING
    clock_t Start, End;
  Start = clock();
#endif

    if (!FPCPOptimization || (GetTestMode()==VALIDATION) || (GetTestMode()==TRAIN)){
        for (unsigned int i=0; i<Conjunctions.size(); i++) {
            if (Conjunctions[i].Changed) {
                if (i==0) {
                    Conjunctions[i].CumulativeSet = Conjunctions[i].CalculatePerformance();
                } else {
                    Conjunctions[i].CumulativeSet = Conjunctions[i-1].CumulativeSet;
                    Conjunctions[i].CumulativeSet = Conjunctions[i].CumulativeSet | Conjunctions[i].CalculatePerformance();
                    Conjunctions[i].CumulativeSet.SummariseBags();
                }
                Conjunctions[i].Changed = false;
            }
        }
        RuleSet = Conjunctions[Conjunctions.size()-1].CumulativeSet;
    } else {
        // Cumulative Set have already been calculated in FP CP Optimization
        RuleSet = Conjunctions[Conjunctions.size()-1].CumulativeSet;
    }

// !!!
 // ROCCurves[CurrentLength-MinLength].AddEntry(RuleSet.GetSpecificity(), RuleSet.GetSensitivity());  // Add entry to ROCCurve

#ifdef DEBUG_TIMING
    End = clock();
  ExploreTiming.AddTime("RULE::CalculatePerformance", Start, End);
#endif

    return GetPerformance();                                                      // Return a struct! Not an object!
}

/**********************************************************************
Function: GetPerformance()
Category: Selectors
Scope: public
In: -
Out: PerformanceStruct, all performance measures as percentages
Description: Returns the performance as percentages only
(no PosObservations, NegObservations or Classes)
**********************************************************************/
PERFORMANCE RULE::GetPerformance() {
#ifdef DEBUG_TIMING
    clock_t Start, End;
  Start = clock();
#endif

    PERFORMANCE Dummy;

    Dummy.Sensitivity.Value = RuleSet.GetSensitivity();
    Dummy.Specificity.Value = RuleSet.GetSpecificity();
    Dummy.Accuracy.Value = RuleSet.GetAccuracy();
    Dummy.NPV.Value = RuleSet.GetNPV();
    Dummy.PPV.Value = RuleSet.GetPPV();
    Dummy.BalancedAccuracy.Value = RuleSet.GetBalancedAccuracy();
    Dummy.F1score.Value = RuleSet.GetF1score();

#ifdef DEBUG_TIMING
    End = clock();
  ExploreTiming.AddTime("RULE::GetPerformance", Start, End);
#endif

    return Dummy;
}

/**********************************************************************
Function: SetMinLength()
Category: Modifiers
Scope: public
In: unsigned int, minimum length.
Out: -
Description: Assign minimum length of the rule.
**********************************************************************/
void RULE::SetMinLength(unsigned int RMinLength) {
    MinLength = RMinLength;
}

/**********************************************************************
Function: SetMaxLength()
Category: Modifiers
Scope: public


In: unsigned int, minimum length.
Out: -
Description: Assign minimum length of the rule.
**********************************************************************/
void RULE::SetMaxLength(unsigned int RMaxLength) {
    MaxLength = RMaxLength;
}

/**********************************************************************
Function: SetNoFeatures()
Category: Modifiers
Scope: public
In: unsigned int, number of features.
Out: -
Description: Assign maximum number of features of the rule.
**********************************************************************/
void RULE::SetNoFeatures(unsigned int RNoFeatures) {
    NoFeatures = RNoFeatures;
}

/**********************************************************************
Function: SetCurrentLength()
Category: Modifiers
Scope: public
In: unsigned int, new rulelength
Out: -
Description: Sets the length of the rule manually.
**********************************************************************/
void RULE::SetCurrentLength(unsigned int NRuleLength) {
    if (NRuleLength>0) {
        CurrentLength = NRuleLength;
    }
}

/**********************************************************************
Function: SetBranchBound()
Category: Modifiers
Scope: public
In: bool, optimize or not
Out: -
Description: Sets whether explore should use optimization when
generating rules.
**********************************************************************/
void RULE::SetBranchBound(bool Optimize) {
    BranchBound = Optimize;
}

/**********************************************************************
Function: SetBinaryReduction()
Category: Modifiers
Scope: public
In: bool, optimize or not
Out: -
Description: Sets whether explore should use binary optimization when
generating rules.
**********************************************************************/
void RULE::SetBinaryReduction(bool Value) {
    BinaryReduction = Value;
}

/**********************************************************************
Function: AddConjunction()
Category: Modifiers
Scope: public
In: unsigned int, maximum length.
Out: -
Description: Adds a subset to the end of the rule.
**********************************************************************/
void RULE::AddConjunction(unsigned int CSize) {
#ifdef DEBUG_TIMING
    clock_t Start, End;
  Start = clock();
#endif

    if (CSize>0) {
        Conjunctions.push_back(CONJUNCTION(CSize));
    }

#ifdef DEBUG_TIMING
    End = clock();
  ExploreTiming.AddTime("RULE::AddConjunction", Start, End);
#endif
}

/**********************************************************************
Function: RemoveConjunction()
Category: Modifiers
Scope: public
In: -
Out: -
Description: Remove the last subset of the rule.
**********************************************************************/
void RULE::RemoveConjunction() {                                                // Remove last conjunction from the rule
#ifdef DEBUG_TIMING
    clock_t Start, End;
  Start = clock();
#endif

    Conjunctions.pop_back();

#ifdef DEBUG_TIMING
    End = clock();
  ExploreTiming.AddTime("RULE::RemoveConjunction", Start, End);
#endif

}

/**********************************************************************
Function: ClearConjunctions()
Category: Modifiers
Scope: public
In: -
Out: -
Description: Clear all conditions associated with every conjunction.
**********************************************************************/
void RULE::ClearConjunctions() {
    for (unsigned int i=0; i<Conjunctions.size(); i++) {
        Conjunctions[i].Conditions.clear();
    }
}

/**********************************************************************
Function: Reset()
Category: Modifiers
Scope: public
In: -
Out: -
Description: Sets all features to 1 in all subsets of the rule.
**********************************************************************/
void RULE::Reset() {                                                            // Reset all subsets of this rule
#ifdef DEBUG_TIMING
    clock_t Start, End;
  Start = clock();
#endif

    Conjunctions.clear();

    CombinationGenerated = false;
    FeatureSetGenerated = false;
    CutoffSetGenerated = false;

    LastCombination = false;

    CurrentLength = MinLength;

    CPBest = 0;
    CTBest = 0;

#ifdef DEBUG_TIMING
    End = clock();
  ExploreTiming.AddTime("RULE::Reset", Start, End);
#endif

}

/**********************************************************************
Function: Initialise()
Category: Modifiers
Scope: public
In: vector<FEATURE>*, pointer to a list of features
Out: -
Description: Initialises the rule by adding a subset of current length.
**********************************************************************/
void RULE::Initialise(vector<FEATURE>* RFeatures) {
#ifdef DEBUG_TIMING
    clock_t Start, End;
  Start = clock();
#endif

    Features = RFeatures;                                                         // Set pointer to original features

    InitialiseRanges();                                                           // Remove any cutoffs for specific features if they don't match the range
    InitialiseMandatory();                                                        // Create Mandatory class if needed
//  InitialiseROC();                                                              // Create ROC structures for all rulelengths which will be processed

    CreateFeatureOperators();                                                     // Create all FeatureOperators from Features

    Reset();                                                                      // Make sure rule in starting state (one conjunction of minlength)

    CombinationsGenerated = 0;
    FeatureSetsGenerated = 0;
    CutoffSetsGenerated = 0;

    CountCandidates = 0;

    CombinationGenerated = false;

#ifdef DEBUG_TIMING
    End = clock();
  ExploreTiming.AddTime("RULE::Initialise", Start, End);
#endif
}

/**********************************************************************
Function: LimitedInitialise()
Category: Modifiers
Scope: public
In:
Out: -
Description: Initialises the rule by adding a subset of current length.
**********************************************************************/
void RULE::LimitedInitialise() {
#ifdef DEBUG_TIMING
    clock_t Start, End;
  Start = clock();
#endif

    Reset();                                                                      // Make sure rule in starting state (one conjunction of minlength)

    CombinationsGenerated = 0;
    FeatureSetsGenerated = 0;
    CutoffSetsGenerated = 0;

    CountCandidates = 0;

    CombinationGenerated = false;

#ifdef DEBUG_TIMING
    End = clock();
  ExploreTiming.AddTime("RULE::Initialise", Start, End);
#endif
}
/**********************************************************************
Function: SetReset(vector<FEATURE>* RFeatures)
Category: Modifiers
Scope: public
In: vector<FEATURE>*, pointer to a list of features
Out: -
Description: Initialises the rule for a new run or partition
**********************************************************************/
void RULE::SetReset(vector<FEATURE>* RFeatures) {
    Features = RFeatures;                                                         // Set pointer to original features

    InitialiseRanges();                                                           // Remove any cutoffs for specific features if they don't match the range
    InitialiseMandatory();                                                        // Create Mandatory class if needed

    CreateFeatureOperators();                                                     // Create all FeatureOperators from Features
    SetTestMode(LEARN);
}

/**********************************************************************
Function: SetRerun(vector<FEATURE>* RFeatures)
Category: Modifiers
Scope: public
In: vector<FEATURE>*, pointer to a list of features
Out: -
Description: Initialises the rule for a new run or partition
**********************************************************************/
void RULE::SetRerun(vector<FEATURE>* RFeatures) {
#ifdef DEBUG_TIMING
    clock_t Start, End;
  Start = clock();
#endif

    Features = RFeatures;                                                         // Set pointer to original features

    InitialiseRanges();                                                           // Remove any cutoffs for specific features if they don't match the range
    InitialiseMandatory();                                                        // Create Mandatory class if needed
//  InitialiseROC();                                                              // Create ROC structures for all rulelengths which will be processed

    CreateFeatureOperators();                                                     // Create all FeatureOperators from Features
    SetTestMode(LEARN);
    Reset();                                                                      // Make sure rule in starting state (one conjunction of minlength)

#ifdef DEBUG_TIMING
    End = clock();
  ExploreTiming.AddTime("RULE::Initialise", Start, End);
#endif
}


/**********************************************************************
Function: InitialiseROC()
Category: Modifiers
Scope: private
In: -
Out: -
Description: Create ROC structures for all individual rulelengths
that will be traversed by Rule.
**********************************************************************/
/*
void RULE::InitialiseROC() {
  for (unsigned int i=MinLength; i<=MaxLength; i++) {                           // Create ROC curves
    ROC Dummy(i);
    ROCCurves.push_back(Dummy);
  }
} */

/**********************************************************************
Function: CreateFeatureOperators()
Category: Modifiers
Scope: private
In: -
Out: -
Description: Create all conditions needed by explore and calculate
their performances.
**********************************************************************/
void RULE::CreateFeatureOperators() {
    unsigned int FOperator=0;

    FeatureOperators.clear();

    for (unsigned int i=0; i<(*Features).size(); i++) {
        if (!(*Features)[i].IsClassFeature()){                                       //skip the class feature
            if ((*Features)[i].GetOperator()==GREATERLESS) {
                CONDITION GreaterCondition(i, (*Features)[i].GetName(), (*Features)[i].GetCutoffs(), GREATER, FOperator);
                GreaterCondition.NoOperators=1;
                FeatureOperators.push_back(GreaterCondition);
                FOperator++;
                CONDITION LessCondition(i, (*Features)[i].GetName(), (*Features)[i].GetCutoffs(), LESS, FOperator);
                LessCondition.NoOperators=1;
                FeatureOperators.push_back(LessCondition);
                FOperator++;
            } else {
                if ((*Features)[i].GetOperator()==EQUAL) {
                    CONDITION EqualCondition(i, (*Features)[i].GetName(), (*Features)[i].GetCutoffs(), EQUAL, FOperator);
                    EqualCondition.NoOperators=1;
                    FeatureOperators.push_back(EqualCondition);
                }
                if ((*Features)[i].GetOperator()==GREATER) {
                    CONDITION GreaterCondition(i, (*Features)[i].GetName(), (*Features)[i].GetCutoffs(), GREATER, FOperator);
                    GreaterCondition.NoOperators=1;
                    FeatureOperators.push_back(GreaterCondition);
                }
                if ((*Features)[i].GetOperator()==LESS) {
                    CONDITION LessCondition(i, (*Features)[i].GetName(), (*Features)[i].GetCutoffs(), LESS, FOperator);
                    LessCondition.NoOperators=1;
                    FeatureOperators.push_back(LessCondition);
                }

                FOperator++;
            }
        }
    }

    NoFeatureOperators = FeatureOperators.size()-1;
}

/**********************************************************************
Function: InitialiseRanges()
Category: Modifiers
Scope: public
In: -
Out: -
Description: Set the range for each feature (disable any cutoffs that
are not in range).
**********************************************************************/
void RULE::InitialiseRanges() {
#ifdef DEBUG_TIMING
    clock_t Start, End;
  Start = clock();
#endif

    vector<FEATURE>::iterator CurrentFeature((*Features).begin());
    vector<FEATURE>::iterator LastFeature((*Features).end());

    for (; CurrentFeature != LastFeature; CurrentFeature++) {
        (*CurrentFeature).InitialiseRanges();
    }

#ifdef DEBUG_TIMING
    End = clock();
  ExploreTiming.AddTime("RULE::InitialiseRanges", Start, End);
#endif

}

/**********************************************************************
Function: InitialiseMandatory()
Category: Modifiers
Scope: public
In: -
Out: -
Description: See whether a feature is mandatory.
**********************************************************************/
void RULE::InitialiseMandatory() {
#ifdef DEBUG_TIMING
    clock_t Start, End;
  Start = clock();
#endif

    vector<FEATURE>::iterator CurrentFeature((*Features).begin());
    vector<FEATURE>::iterator LastFeature((*Features).end());

    for (; CurrentFeature != LastFeature; CurrentFeature++) {
        if ((*CurrentFeature).IsMandatory()) {
            MandatorySet = true;
        }
    }

#ifdef DEBUG_TIMING
    End = clock();
  ExploreTiming.AddTime("RULE::InitialiseMandatory", Start, End);
#endif

}

/**********************************************************************
Function: NextCombination()
Category: Modifiers
Scope: public
In: -
Out: bool, succeeded or not.
Description: Generates the next partition of the rule.
**********************************************************************/
bool RULE::NextCombination() {
#ifdef DEBUG_TIMING
    clock_t Start, End;
  Start = clock();
#endif
    // Has a combination been generated before?
    FeatureSetGenerated = false;                                                  // No featureset has been generated yet for this combination
    CutoffSetGenerated = false;                                                   // No conditionset has been generated yet for a featureset of this combination
    bool Incremented = false;

    if (LastCombination) return false;

    do {
        if (CombinationGenerated) {
            unsigned int Stack = 0;
            for (int i=Conjunctions.size()-1; i>=0 && !Incremented; i--) {
                if (Conjunctions[i].Size>1) {                                           // Conjunction size > 1
                    Conjunctions[i].Size--;                                               // Decrement conjunction size
                    Stack++;
                    while (Stack>0){
                        if (Stack>Conjunctions[i].Size) {                                   // Number of conjunctions of size 1 > current conjunction size?
                            AddConjunction(Conjunctions[i].Size);                             // Add a conjunction of size 1
                            Stack -= Conjunctions[i].Size;
                        } else {                                                            // Stack < CurrentConjunction.size
                            AddConjunction(Stack);                                            // Add single conjunction of size stack
                            Stack = 0;
                        }
                    }
                    Incremented = true;
                } else if (Conjunctions[i].Size==1) {                                   // Remember any conjunction of size 1
                    Stack++;                                                              // Increment the stack
                    vector<CONJUNCTION>::iterator CurrentConjunction(Conjunctions.begin()+i);
                    Conjunctions.erase(CurrentConjunction);                               // Erase current conjunction of size 1
                }
            }
        } else {
            CurrentLength = MinLength;
            AddConjunction(CurrentLength);                                            // Create first combination
            Incremented = true;
        }

        if (Conjunctions.size()==0) {
            if (CurrentLength+1<=MaxLength) {                                         // Create first combination for next rulelength
                CurrentLength++;
                Conjunctions.clear();
                AddConjunction(CurrentLength);                                          // Create the new combination
                Incremented = true;
            } else {
                LastCombination = true;                                                 // Can't generate new combination
            }
        }
    } while (!LastCombination && !Incremented);

    if (Incremented) {
        CombinationsGenerated++;
        CombinationGenerated = true;
        ClearConjunctions();
        LastCombination = false;
    } else {
        CombinationGenerated = false;
        LastCombination = true;
        Conjunctions.clear();
    }

#ifdef DEBUG_TIMING
    End = clock();
  ExploreTiming.AddTime("RULE::NextCombination", Start, End);
#endif

    return CombinationGenerated;
}

/**********************************************************************
Function: StartRuleLength()
Category: Modifiers
Scope: public
In: -
Out: bool, succeeded or not.
Description: Generates the first combination for rule length i
**********************************************************************/
bool RULE::StartRuleLength(int i) {
    if (i<=MaxLength) {                                                       // Create first combination for next rulelength
        CurrentLength=i;
        Conjunctions.clear();
        AddConjunction(CurrentLength);                                             // Create the new combination
        CombinationGenerated = true;
        FeatureSetGenerated = false;
        CutoffSetGenerated = false;
        ClearConjunctions();
        LastCombination = false;
        return true;
    } else {
        return false;
    }
}


/**********************************************************************
Function: NextFeatureSet()
Category: Modifiers
Scope: public
In: -
Out: bool, succeeded or not.
Description: Generates the next featureset of the rule.
**********************************************************************/
bool RULE::NextFeatureSet(int FOperatorNr_start, int FOperatorNr_end) {
#ifdef DEBUG_TIMING
    clock_t Start, End;
  Start = clock();
#endif

    // int FOperatorNr_start = 1;
    // int FOperatorNr_end = FOperatorNr_start+1;

    // Counters as reference
    int ConjunctionSize, ConjunctionNr, ConditionNr, FOperatorNr, MaxFOperator;
    CONDITION* Condition;
    CONJUNCTION* PreviousConjunction;

    // Whatever happens (increment possible or not), ConditionSet has not been generated yet
    CutoffSetGenerated = false;
    bool Incremented = false;

    // Check if Combination exist.
    if (!CombinationGenerated) return false;

    // If the size of the Conjunction is greater than the number of FeatureOperators we cannot generate FeatureSets
    int ConjunctionSizeB = Conjunctions[0].Size;
    int NoFeatureOperatorsB = FeatureOperators.size();
    int NoConjunctionsB = Conjunctions.size();
    int NumRepeats=0;                                                 // Number of Repeats of nominal FOP in the rule
    int i;                                                      // Counter
    int Left;                                                       // Number of available FOPS to instantiate

    if (ConjunctionSizeB > NoFeatureOperatorsB) return false;
    if (NoConjunctionsB > NoFeatureOperatorsB) return false;

    if (FeatureSetGenerated) {

        // AM: Algorithm 7 -> 8

        // Iterate through conjunctions (from back to front)
        for (ConjunctionNr = Conjunctions.size()-1; ConjunctionNr>=0 && !Incremented; ) {

            // Maximum FeatureOperator that is allowed decreases by condition and conjunction through the rule
            if (ConjunctionNr<(int)Conjunctions.size()-1 && Conjunctions[ConjunctionNr].Size==1) {
                //allow multiple occurences of nominal features
                if (FeatureOperators[Conjunctions[ConjunctionNr].Conditions[0].FeatureOperator].Operator!=EQUAL){
                    MaxFOperator = NoFeatureOperators-((Conjunctions.size()-1)-ConjunctionNr);
                } else MaxFOperator = NoFeatureOperators;
            } else {
                MaxFOperator = NoFeatureOperators;
            }

            // Iterate through conditions (from back to front)
            for (ConditionNr=Conjunctions[ConjunctionNr].Size-1; ConditionNr>=0 && !Incremented; ) {

                Condition = &Conjunctions[ConjunctionNr].Conditions[ConditionNr];       // Save reference to current condition
                FOperatorNr = (*Condition).FeatureOperator+1;                           // Calculate next FeatureOperator to be used (current FeatureOperator +1)

                if (ConditionNr == 0 && ConjunctionNr ==0) {
                    MaxFOperator = std::min(FOperatorNr_end, MaxFOperator);
                }

                if (ConjunctionNr > 0 && BinaryReduction && Conjunctions[ConjunctionNr].Size==1) {
                    // No repeats of feature operators for binary reduction
                    for (unsigned int j=0; j<Conjunctions[0].Conditions.size(); j++) { // Go per condition
                        for (i=0; i<=ConjunctionNr-1; i++) { // Go through all previous conjunctions (front of rule)
                            if (j<Conjunctions[i].Size) {
                                if (FOperatorNr == Conjunctions[i].Conditions[j].FeatureOperator) {
                                    FOperatorNr++;
                                    i=0;
                                    j=0;
                                }
                            }
                        }
                    }
                }

                // Iterate from Current FeatureOperator of condition to Max. FeatureOperator
                if (FOperatorNr <= MaxFOperator) {
                    //  if (FOperatorNr <= MaxFOperator && (!(ConditionNr == 0 && ConjunctionNr ==0) || FOperatorNr <= FOperatorNr_end)) {                                      // Make sure we obey universal conjunction rules

                    (*Condition) = FeatureOperators[FOperatorNr];                         // Set condition to current FeatureOperator
                    Incremented = true;
                    //check total number of solo occurencee of nominal FOPs until incremented position
                    if (Conjunctions[ConjunctionNr].Size==1 && FeatureOperators[FOperatorNr].Operator == EQUAL){
                        NumRepeats = 0;
                        for (i=0; i<=ConjunctionNr;i++){
                            if (Conjunctions[i].Conditions[0].FeatureOperator ==
                                FOperatorNr && Conjunctions[i].Size==1){
                                NumRepeats++;
                            }
                        }
                        //check if there are enough FOPs left
                        Left = FeatureOperators[FOperatorNr].Cutoffs.size()-1-NumRepeats;
                        for (i=FOperatorNr+1; i<=MaxFOperator;i++){
                            if (FeatureOperators[i].Operator==EQUAL){
                                //Nominals can be insert 1 less than the number of values
                                Left += FeatureOperators[i].Cutoffs.size()-1;
                            }
                            else {
                                //Others can be inserted only once
                                Left++;
                            }
                        }
                        if (Left<(Conjunctions.size()-1-ConjunctionNr)){
                            Incremented=false;
                        }
                    }
                } else {
                    ConditionNr--;
                }
                // Moving left in conjunction, so maximum FeatureOperator that can be used is one less
                MaxFOperator--;
            }

            if (!Incremented) {
                ConjunctionNr--;
            }
        }
    } else {
        // AM: Algorithm 6
        Incremented = true;                                                         // First FeatureSet generated for current Combination
        // FOperatorNr = 0;
        FOperatorNr = FOperatorNr_start;
        // Iterate through conjunctions (from front to back)
        for (ConjunctionNr=0; ConjunctionNr<=(int)Conjunctions.size()-1; ConjunctionNr++) {

            // Reset FeatureOperator if we're not in the first conjunction and conjunction size is not equal to 1
            if (ConjunctionNr>0) {
                if (BinaryReduction && Conjunctions[ConjunctionNr].Size==1) {
                    // Simply go to next FeatureOperator, no repeats
                } else if (Conjunctions[ConjunctionNr-1].Size>1) {
                    FOperatorNr=0;
                    NumRepeats = 0;
                } else {//allow multiple occurences of nominal features
                    if (FeatureOperators[Conjunctions[ConjunctionNr - 1].Conditions[0].FeatureOperator].Operator ==
                        EQUAL && !BinaryReduction) {

                    //allow A=b or A=c in rules if feature is nominal!!
                    //Check if number of nominal values-1 > number of repeats
                    //e.g., two nominal values A=a or A=b means is all data and thus
                    //redundant
                    if (NumRepeats < Conjunctions[ConjunctionNr - 1].Conditions[0].Cutoffs.size() - 1) {
                        FOperatorNr = Conjunctions[ConjunctionNr -
                                                   1].Conditions[0].FeatureOperator; // AM: copy previous term
                    } else {
                        //fill in next FOP
                        FOperatorNr = Conjunctions[ConjunctionNr - 1].Conditions[0].FeatureOperator +
                                      1; // AM: take next feature-operator
                    }

                } else {
                        FOperatorNr = Conjunctions[ConjunctionNr -
                                                   1].Conditions[0].FeatureOperator + 1; // AM: take next feature-operator

                    }
                }
            }

            // Iterate through conditions (from front to back)
            for (ConditionNr=0; ConditionNr<(int)Conjunctions[ConjunctionNr].Size; ConditionNr++) {

                if (FOperatorNr > NoFeatureOperators) {
                    // Check if in correct range// if(FOperatorNr > NoFeatureOperators || ((ConditionNr == 0 && ConjunctionNr ==0) && FOperatorNr > FOperatorNr_end))
                // if (FOperatorNr > NoFeatureOperators || FOperatorNr > FOperatorNr_end) { // Check if in correct range
                    FeatureSetGenerated = false;
                    CutoffSetGenerated = false;

                    return FeatureSetGenerated;
                }

                Conjunctions[ConjunctionNr].Conditions.push_back(FeatureOperators[FOperatorNr]);
                // Add FeatureOperators to conjunction
                FOperatorNr++;                                                          // Increment FeatureOperator

                if (FeatureOperators[Conjunctions[ConjunctionNr].Conditions[0].FeatureOperator].Operator==EQUAL
                    && Conjunctions[ConjunctionNr].Size==1 && !BinaryReduction){
                    NumRepeats++; // ?
                } else {
                    NumRepeats=0;
                }
            }

        }

    }

    // Reset any conditions/conjunctions after incremented conditionset
    if (Incremented) {                                                            // Has a FeatureSet been incremented?
        // AM: Algorithm 7 -> 6

        ConditionNr++;                                                              // First condition after incremented condition
        int StartConjunctionNr = ConjunctionNr;                                     // Remember conjunction where we begin to reset the rule
        CONDITION* PreviousCondition;                                               // Reference to condition in previous conjunction (if conjunction sizes are the same)
        for (; ConjunctionNr<=(int)Conjunctions.size()-1; ConjunctionNr++) {        // Traverse conjunctions

            // Not the conjunction at which we started, conjunction sizes match
            if (ConjunctionNr!=StartConjunctionNr && Conjunctions[ConjunctionNr].Size==Conjunctions[ConjunctionNr-1].Size) {

                // Copy this conjunction from previous conjunction
                for (; ConditionNr<(int)Conjunctions[ConjunctionNr].Size; ConditionNr++) {   // Traverse conditions within conjunction while resetting FOperatorNr

                    Condition = &Conjunctions[ConjunctionNr].Conditions[ConditionNr];    // Save reference to condition

                    if (Conjunctions[ConjunctionNr].Size>1 ) {
                        PreviousCondition = &FeatureOperators[Conjunctions[ConjunctionNr-1].Conditions[ConditionNr].FeatureOperator]; // AM: copy previous term
                        NumRepeats = 0;
                    } else {
                        // If  conjunction size is 1 and check if nominal
                        if (BinaryReduction) {
                            int FONext = Conjunctions[ConjunctionNr-1].Conditions[ConditionNr].FeatureOperator+1; // AM: next feature-operator

                            // No repeats of feature operators for binary reduction
                            for (i=0; i<=ConjunctionNr-1; i++) { // Go through all previous conjunctions (front of rule)
                                PreviousConjunction = &Conjunctions[i];
                                for (unsigned int j = 0; j < PreviousConjunction->Conditions.size(); j++) {
                                    if (FONext == PreviousConjunction->Conditions[j].FeatureOperator) {
                                        FONext++;
                                        i=0;
                                        j=0;
                                    }
                                }
                            }
                            if (FONext<FeatureOperators.size()) {
                                PreviousCondition = &FeatureOperators[FONext];
                            } else { // FONext == FeatureOperators.size()-1
                                return NextFeatureSet(FOperatorNr_start, FOperatorNr_end); // Next feature set
                                // break;
                            }
                        } else if (FeatureOperators[Conjunctions[ConjunctionNr-1].Conditions[ConditionNr].FeatureOperator].Operator==EQUAL){
                            //allow A=b or A=c in rules if feature is nominal!!
                            //Check if number of nominal values-1 > number of repeats
                            //e.g., two nominal values A=a or A=b means is all data and thus
                            //redundant
                            if (NumRepeats < Conjunctions[ConjunctionNr-1].Conditions[ConditionNr].Cutoffs.size()-1){
                                PreviousCondition = &FeatureOperators[Conjunctions[ConjunctionNr-1].Conditions[ConditionNr].FeatureOperator]; // AM: copy previous term
                                NumRepeats++;
                            } else    {
                                PreviousCondition = &FeatureOperators[Conjunctions[ConjunctionNr-1].Conditions[ConditionNr].FeatureOperator+1]; // AM: take next feature-operator
                                NumRepeats = 1;
                            }
                        } else {
                            //other operator can occur solo only once
                            if (Conjunctions[ConjunctionNr-1].Conditions[ConditionNr].FeatureOperator+1<FeatureOperators.size()) {
                                PreviousCondition = &FeatureOperators[Conjunctions[ConjunctionNr-1].Conditions[ConditionNr].FeatureOperator+1]; // AM: take next feature-operator
                            } else { // out of bounds
                                return NextFeatureSet(FOperatorNr_start, FOperatorNr_end); // Next feature set
                            }
                        }

                    }
                    (*Condition) = (*PreviousCondition);                                 // Set condition to condition in same position of previous conjunction


                }

            }


            // Not the conjunction at which we started, conjunction sizes do not match
            if (ConjunctionNr!=StartConjunctionNr && Conjunctions[ConjunctionNr].Size!=Conjunctions[ConjunctionNr-1].Size) {

                // Reset this conjunction
                for (; ConditionNr<(int)Conjunctions[ConjunctionNr].Size; ConditionNr++) {   // Traverse conditions within conjunction while resetting FOperatorNr

                    Condition = &Conjunctions[ConjunctionNr].Conditions[ConditionNr];    // Save reference to condition

                    if (ConjunctionNr > 0 && BinaryReduction && Conjunctions[ConjunctionNr].Size==1) { // TODO: ConjunctionNr always 0?
                        // No repeats of feature operators for binary reduction
                        for (unsigned int j=0; j<Conjunctions[0].Conditions.size(); j++) { // Go per condition
                            for (i=0; i<=ConjunctionNr-1; i++) { // Go through all previous conjunctions (front of rule)
                                if (j<Conjunctions[i].Size) {
                                    if (FOperatorNr == Conjunctions[i].Conditions[j].FeatureOperator) {
                                        FOperatorNr++;
                                        i=0;
                                        j=0;
                                    }
                                }
                            }
                        }

                    }

                    if (FOperatorNr<FeatureOperators.size()) {
                        (*Condition) = FeatureOperators[FOperatorNr];                        // Set condition to current FeatureOperator
                    } else { // FONext == FeatureOperators.size()-1
                        return NextFeatureSet(FOperatorNr_start, FOperatorNr_end); // Next feature set
                    }

                    FOperatorNr++;                                                       // Within conjunction increase FOperator by 1 each time a condition is traversed (reset conditions)

                }
                if (Conjunctions[ConjunctionNr].Size==1 &&
                    FeatureOperators[Conjunctions[ConjunctionNr].Conditions[0].FeatureOperator].Operator==EQUAL && !BinaryReduction) {
                    NumRepeats++;
                }
            }

            // Conjunction at which we started
            if (ConjunctionNr==StartConjunctionNr) {

                // Increment remainder of conjunction sequentially
                for (; ConditionNr<(int)Conjunctions[ConjunctionNr].Size; ConditionNr++) {   // Traverse conditions within conjunction while resetting FOperatorNr

                    Condition = &Conjunctions[ConjunctionNr].Conditions[ConditionNr];    // Save reference to condition
                    (*Condition) = FeatureOperators[Conjunctions[ConjunctionNr].Conditions[ConditionNr-1].FeatureOperator+1];                        // Set condition to current FeatureOperator

                }

            }

            ConditionNr=0;                                                            // First condition of next condition
            FOperatorNr=0;                                                            // Reset FeatureOperator

        }
    }

    if (Incremented){
        FeatureSetsGenerated++;
        FeatureSetGenerated = true;
    }
    else {
        FeatureSetGenerated = false;
        CutoffSetGenerated = false;
    }

#ifdef DEBUG_TIMING
    End = clock();
  ExploreTiming.AddTime("RULE::NextFeatureSet", Start, End);
#endif

    return FeatureSetGenerated;
}

/**********************************************************************
Function: NextCutoffSet()
Category: Modifiers
Scope: public
In: bool, should we optimize on sensitivity/specificity measure and constraint?
Out: bool, succeeded or not.
Description: Generates the next instance of the rule.
**********************************************************************/
bool RULE::NextCutoffSet() {
#ifdef DEBUG_TIMING
    clock_t Start, End;
  Start = clock();
#endif

    if (!FeatureSetGenerated) return false;                                       //Check if FeatureSets are generated

    if ((*MaximizeMeasure)==SENSITIVITY && !RestrictionSet && BranchBound) {
        return NextCandidateCutoffSet_Rule(SENSITIVITY);
    }
    if ((*MaximizeMeasure)==ACCURACY && !RestrictionSet && BranchBound) {
        return NextCandidateCutoffSet_Rule(ACCURACY);
    }

    int ConjunctionNr, ConditionNr;
    unsigned int MaxCutoff;
    bool Incremented = false;
    bool AtFirst = false;
    bool ResetFirst = false;

    CONJUNCTION* CurrentConjunction;
    CONJUNCTION* PreviousConjunction;
    CONDITION* CurrentCondition;
    CONDITION* PreviousCondition;
    CONDITION* CurrentFeatureOperator;

    if (CutoffSetGenerated) {
        // Reset changed for each conjunction
        for (int i=Conjunctions.size()-1; i>=0 && Conjunctions[i].Changed; i--) {
            Conjunctions[i].Changed = false;
        }

        // Increment existing cutoff set
        for (ConjunctionNr = Conjunctions.size()-1; ConjunctionNr>=0 && !Incremented; ) {
            CurrentConjunction = &Conjunctions[ConjunctionNr];
            // Iterate through conditions (from back to front)
            for (ConditionNr=Conjunctions[ConjunctionNr].Size-1; ConditionNr>=0 && !Incremented; ) {
                CurrentCondition = &Conjunctions[ConjunctionNr].Conditions[ConditionNr];
                CurrentFeatureOperator = &FeatureOperators[CurrentCondition->FeatureOperator];

                MaxCutoff = CurrentCondition->Cutoffs.size();

                if (CurrentConjunction->Size==1 && Conjunctions.size()>1) {                         // More than one conjunction and current conjunction size = 1
                    if (CurrentCondition-> Operator==EQUAL){
                        if (CurrentCondition->CutoffNumber+1 < MaxCutoff) {
                            CurrentCondition->CutoffNumber++;
                            Incremented = true;

                            // Activate stop for term tuples like 1 1 1
                            // Current conjunction = size 1, so all higher conjunctions also size 1
                            if (ConjunctionNr<Conjunctions.size()-1) {
                                PreviousConjunction = &Conjunctions[ConjunctionNr+1]; // Next conjunction but using existing object

                                // TODO: check how many repeats? is this important?
                                PreviousCondition = &PreviousConjunction->Conditions[0]; // Next condition but using existing object

                                if (PreviousCondition->FeatureOperator == CurrentCondition->FeatureOperator &&
                                    CurrentCondition->CutoffNumber == CurrentCondition->Cutoffs.size() - 1) {
                                    // TODO: 1 difference is specific for this case (for skipping last threshold)
                                    Incremented = false;
                                    ConditionNr--;
                                    CurrentCondition->StopNext = true;
                                }
                            }

                            // Correction binary / categorical features
                            if (ConjunctionNr > 0) {
                                for (int i=ConjunctionNr-1; i>=0; i--) { // Go through all previous conjunctions (front of rule)
                                    PreviousConjunction = &Conjunctions[i]; // i = ConjunctionNr - 1

                                    if (CurrentFeatureOperator->NonSoloIncluded && PreviousConjunction->Size > 1) {
                                        for (unsigned int j = 0; j < PreviousConjunction->Conditions.size(); j++) {
                                            PreviousCondition = &PreviousConjunction->Conditions[j];

                                            if (PreviousCondition->FeatureOperator ==
                                                CurrentCondition->FeatureOperator &&
                                                PreviousCondition->CutoffNumber == CurrentCondition->CutoffNumber) {
                                                if (MaxCutoff == 2) { // For binary variables
                                                    PreviousCondition->CutoffNumber = 0;
                                                } else { // For categorical variables
                                                    if (CurrentCondition->CutoffNumber + 1 < MaxCutoff) {
                                                        CurrentCondition->CutoffNumber++;
                                                    } else {
                                                        // If ALL later cutoffs in rule are at last value -> set PreviousCondition to first value (this has been skipped in the beginning)
                                                        if (CutoffsAtMax(ConjunctionNr - 1, j)) {
                                                            PreviousCondition->CutoffNumber = 0; // Reset cutoff
                                                            ConjunctionNr = ConjunctionNr - 1;
                                                            ConditionNr = j;
                                                            PreviousCondition->StopNext = true;
                                                        } else {
                                                            Incremented = false;
                                                            ConditionNr--;
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }

                            // If stop, go to previous
                            // TODO: check if can do more efficiently?
                            if (CurrentCondition->StopNext) {
                                ConditionNr--;
                                Incremented = false;

                                if (CurrentCondition->StopNext) {
                                    CurrentCondition->StopNext=false;// Reset value
                                }
                            }

                        } else {
                            ConditionNr--;
                        }
                    } else if (CurrentCondition->Operator==LESS) {
                        MaxCutoff = GetMinCutoff(CurrentCondition->FeatureNumber);
                        if (CurrentCondition->CutoffNumber+1 < MaxCutoff) {
                            CurrentCondition->CutoffNumber++;
                            Incremented = true;
                        } else {
                            ConditionNr--;
                        }
                    } else if (CurrentCondition-> Operator==GREATER){
                        MaxCutoff = GetMaxCutoff(CurrentCondition->FeatureOperator);
                        if (CurrentCondition->CutoffNumber+1 > MaxCutoff && CurrentCondition->CutoffNumber+1<CurrentCondition->Cutoffs.size()) {
                            CurrentCondition->CutoffNumber++;
                            Incremented = true;
                        } else {
                            ConditionNr--;
                        }
                    }
                } else {
                    if (CurrentFeatureOperator->Operator==EQUAL && MaxCutoff==2){ // Needed for binary,should be removed for categorical
                        if (CurrentFeatureOperator->RepeatedFeature && CurrentConjunction->Size>1) {MaxCutoff--;}
                    } else if (CurrentFeatureOperator->NonSoloIncluded){
                        if (CurrentFeatureOperator->Operator==GREATER) {MaxCutoff--;}
                    } else if (CurrentFeatureOperator->RepeatedFeature){
                         if (CurrentFeatureOperator->Operator==LESS && CurrentConjunction->Size>1) {MaxCutoff--;}
                    }
                    if (CurrentCondition->NextSame) { // For greater, also for equal or less?
                        MaxCutoff--;
                    }
                    if (CurrentCondition->CutoffNumber+1 < MaxCutoff && !CurrentCondition->StopNext) { // StopNext is to stop cutoff generation for categorical variables
                        CurrentCondition->CutoffNumber++;
                        Incremented = true;
                    } else {
                        ConditionNr--;

                        if (CurrentCondition->StopNext) {
                            CurrentCondition->StopNext=false;// Reset value
                        }
                    }
                }
            }
            if (!Incremented) {
                ConjunctionNr--;
            }
        }

        if (Incremented) {
            // First cutoff after incremented cutoff in rule : reset any cutoff to the right

            ConditionNr++;
            for (; ConjunctionNr<(int)Conjunctions.size(); ConjunctionNr++) {
                CurrentConjunction = &Conjunctions[ConjunctionNr];

                CurrentConjunction->Changed = true;

                for (; ConditionNr<(int)CurrentConjunction->Size; ConditionNr++) {
                    CurrentCondition = &CurrentConjunction->Conditions[ConditionNr];
                    CurrentFeatureOperator = &FeatureOperators[CurrentCondition->FeatureOperator];

                    if (CurrentConjunction->Size==1 && Conjunctions.size()>1) {             // More than one conjunction and current conjunction size = 1
                        if (CurrentCondition->Operator==EQUAL) {
                            // If the previous FOP is the same nominal FOP, increase the condition
                            if ((ConjunctionNr > 0) && ((&Conjunctions[ConjunctionNr-1])->Size==1)
                            && (PreviousCondition->FeatureNumber == CurrentCondition->FeatureNumber)
                            && PreviousCondition->CutoffNumber+1<PreviousCondition->Cutoffs.size()){
                                CurrentCondition->CutoffNumber = PreviousCondition->CutoffNumber+1;
                            } else {
                                CurrentCondition->CutoffNumber = 0;

                                // Correction categorical variables
                                if (CurrentFeatureOperator->NonSoloIncluded && ConjunctionNr > 0) {
                                    if (ConjunctionNr > 0) {
                                        for (int i = ConjunctionNr - 1;
                                             i >= 0; i--) { // Go through all previous conjunctions (front of rule)
                                            PreviousConjunction = &Conjunctions[i]; // i = ConjunctionNr - 1

                                            // PreviousConjunction = &Conjunctions[ConjunctionNr-1];
                                            if (PreviousConjunction->Size > 1) {
                                                for (unsigned int j = 0;
                                                     j < PreviousConjunction->Conditions.size(); j++) {
                                                    PreviousCondition = &PreviousConjunction->Conditions[j];

                                                    if (PreviousCondition->FeatureOperator ==
                                                        CurrentCondition->FeatureOperator &&
                                                        PreviousCondition->CutoffNumber == 0) {
                                                        if (PreviousCondition->CutoffNumber + 1 < MaxCutoff) {
                                                            if (PreviousCondition->Cutoffs.size() == 2) { // BINARY
                                                                PreviousCondition->CutoffNumber++;
                                                            } else {
                                                                CurrentCondition->CutoffNumber++; // CATEGORICAL
                                                            }
                                                        }

                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        } else if (CurrentCondition->Operator==LESS) {
                            CurrentCondition->CutoffNumber = 0;
                        } else if (CurrentCondition->Operator==GREATER) {
                            // Reset to next cutoff
                            CurrentCondition->CutoffNumber = GetMinCutoff(CurrentCondition->FeatureNumber)+1; // TODO: check if correct

                            // Or first if maximum reached
                            if (CurrentCondition->CutoffNumber > CurrentCondition->Cutoffs.size()-1) {
                                CurrentCondition->CutoffNumber = 0;
                            }
                        }
                    } else {
                        if (CurrentCondition->PreviousSame) {                                 // Increase cutoff for the same features within a conjunction
                            CurrentCondition->CutoffNumber = CurrentConjunction->Conditions[ConditionNr-1].CutoffNumber+1;
                        } else {
                            CurrentCondition->CutoffNumber = 0;                                 // Not the same feature

                            // If the previous FOP is the same nominal FOP, increase the condition
                            if ((ConjunctionNr > 0) &&  (CurrentCondition->Operator==EQUAL) && CurrentConjunction->Size==1){
                                if  ((&Conjunctions[ConjunctionNr-1])->Size==1) {
                                    if  (PreviousCondition->FeatureNumber == CurrentCondition->FeatureNumber && PreviousCondition->CutoffNumber == CurrentCondition->CutoffNumber){
                                        if (PreviousCondition->CutoffNumber + 1 < MaxCutoff) {
                                            CurrentCondition->CutoffNumber = PreviousCondition->CutoffNumber + 1;
                                        } else {
                                            CutoffSetGenerated = false; // TODO: check if this is needed
                                        }
                                    }
                                }
                            }

                            // Reset to next cutoff
                            if (!(CurrentCondition->Operator==LESS) && (CurrentFeatureOperator->IsSolo || CurrentFeatureOperator->RepeatedFeature) && CurrentConjunction->Size>1) {
                                CurrentCondition->CutoffNumber = GetMinCutoff(CurrentCondition->FeatureNumber)+1; // TODO: check if correct
                            }
                        }
                    }

                }
                ConditionNr = 0;
                PreviousCondition = CurrentCondition;
            }
            CutoffSetsGenerated++;
        } else {
            CutoffSetGenerated = false;
        }
        return Incremented;

    } else {
        // Create first cutoff set for this featureset

        // Reset all FeatureOperators
        vector<CONDITION>::iterator CFOperator(FeatureOperators.begin());
        vector<CONDITION>::iterator LFOperator(FeatureOperators.end());
        for (; CFOperator != LFOperator; CFOperator++) {
            CFOperator->IsSolo = false;
            CFOperator->IsRepeated = false;
            CFOperator->NonSoloIncluded = false;
            CFOperator->RepeatedFeature = false;
        }

        // Reset cutoffs of Conditions in rule and find equal features within conjunctions
        vector<CONJUNCTION>::iterator CConjunction(Conjunctions.begin());
        vector<CONJUNCTION>::iterator LConjunction(Conjunctions.end());
        vector<CONDITION>::iterator CCondition;                                     // Current condition
        vector<CONDITION>::iterator PCondition;                                     // Previous condition
        vector<CONDITION>::iterator LCondition;                                     // Last condition

        for (; CConjunction != LConjunction; CConjunction++) {

            CCondition = CConjunction->Conditions.begin();
            PCondition = CConjunction->Conditions.begin();
            LCondition = CConjunction->Conditions.end();
            AtFirst = true;

            PCondition--;

            CConjunction->Changed = true;

            for (; CCondition != LCondition; CCondition++) {
                CCondition->CutoffNumber = 0;
                if (!AtFirst) {                                                         // If we aren't at the first condition of the conjunction
                    if (CCondition->FeatureNumber==PCondition->FeatureNumber) {           // If the feature of the current condition is equal to the feature of the previous condition
                        CCondition->PreviousSame = true;                                    // Set PreviousSame of current condition
                        PCondition->NextSame = true;                                        // Set NextSame of previous condition
                    } else {
                        CCondition->PreviousSame = false;                                   // Set PreviousSame of current condition
                        PCondition->NextSame = false;                                       // Set NextSame of previous condition
                    }
                } else {                                                                // We aren't at the first condition of the conjunction anymore
                    AtFirst = false;
                    CCondition->PreviousSame = false;
                }
                PCondition++;
            }
            PCondition->NextSame = false;
        }

        // Find FeatureOperators within conjunctions of size 1
        for (ConjunctionNr = Conjunctions.size()-1; ConjunctionNr>=0; ConjunctionNr--) {
            if (Conjunctions[ConjunctionNr].Size==1) {
                FeatureOperators[Conjunctions[ConjunctionNr].Conditions[0].FeatureOperator].IsSolo=true;

                // Identify occurences of that feature in other terms (of size 1 or more than 1)
                for (int C=ConjunctionNr-1; C>=0; C--) {
                    CurrentConjunction = &Conjunctions[C];
                    for (ConditionNr=0; ConditionNr<(int)CurrentConjunction->Size; ConditionNr++) {                // Iterate through conditions
                        if (CurrentConjunction->Conditions[ConditionNr].FeatureNumber ==
                            Conjunctions[ConjunctionNr].Conditions[0].FeatureNumber) {
                                FeatureOperators[CurrentConjunction->Conditions[ConditionNr].FeatureOperator].RepeatedFeature = true;

                            FeatureOperators[Conjunctions[ConjunctionNr].Conditions[0].FeatureOperator].IsRepeated=true;

                            if (CurrentConjunction->Size > 1 && CurrentConjunction->Conditions[ConditionNr].FeatureOperator ==
                                    Conjunctions[ConjunctionNr].Conditions[0].FeatureOperator) {
                                    FeatureOperators[CurrentConjunction->Conditions[ConditionNr].FeatureOperator].NonSoloIncluded = true;
                                }
                        }
                    }
                }
            }
        }

        for (ConjunctionNr = 0; ConjunctionNr<(int)Conjunctions.size(); ConjunctionNr++) {               // Iterate through conjunctions
            CurrentConjunction = &Conjunctions[ConjunctionNr];                                             // Create reference to current conjunction
            for (ConditionNr=0; ConditionNr<(int)CurrentConjunction->Size; ConditionNr++) {                // Iterate through conditions
                CurrentCondition = &Conjunctions[ConjunctionNr].Conditions[ConditionNr];
                CurrentFeatureOperator = &FeatureOperators[CurrentCondition->FeatureOperator];

                if (CurrentFeatureOperator->IsSolo || CurrentFeatureOperator->RepeatedFeature) {                                                  // Is current condition solo?
                    if (CurrentConjunction->Size>1) {
                        if (!(CurrentCondition->Operator==GREATER && CurrentFeatureOperator->NonSoloIncluded) && !(CurrentCondition->Operator==LESS && CurrentFeatureOperator->RepeatedFeature)) { // If operator = greater or equal, start from next cutoff
                            if (CurrentCondition->Cutoffs.size()>1) {
                                CurrentCondition->CutoffNumber = 1;
                            } else {
                                CutoffSetGenerated = false;
                                return false;
                            }

                        }
                    } else {
                        CurrentCondition->CutoffNumber = 0; // TODO: unneccesary?

                        if ((CurrentFeatureOperator->NonSoloIncluded || CurrentFeatureOperator->RepeatedFeature || CurrentFeatureOperator->IsRepeated) && CurrentCondition->Operator == GREATER) {
                            if (CurrentCondition->Cutoffs.size()>1) {
                                CurrentCondition->CutoffNumber = 1;
                            } else {
                                CutoffSetGenerated = false;
                                return false;
                            }
                        }

                        // Compare FeatureNumbers between conjunctions
                        // If the previous FOP is the same nominal FOP, increase the condition
                        if ((ConjunctionNr > 0) &&  (CurrentCondition->Operator==EQUAL)){
                            if  ((&Conjunctions[ConjunctionNr-1])->Size==1) {
                                if  (PreviousCondition->FeatureNumber == CurrentCondition->FeatureNumber){
                                    if (PreviousCondition->CutoffNumber + 1 < CurrentCondition->Cutoffs.size()) {
                                        CurrentCondition->CutoffNumber = PreviousCondition->CutoffNumber + 1;
                                    } else {
                                        CutoffSetGenerated = false; // TODO: check this!!
                                    }
                                }
                            }
                        }
                    }
                }

                if (CurrentConjunction->Size>1 && ConditionNr>0) {                                           // Compare FeatureNumbers within a conjunction
                    if (CurrentCondition->FeatureNumber==PreviousCondition->FeatureNumber) {
                        if (CurrentCondition->Cutoffs.size()>1) {
                            CurrentCondition->CutoffNumber = PreviousCondition->CutoffNumber + 1;
                            // TODO: need to differentiate between operators?
                        } else {
                            CutoffSetGenerated = false;
                            return false;
                        }
                    }
                }

                PreviousCondition = CurrentCondition;                                                        // Save reference to previous condition of conjunction
            }
        }
        CutoffSetGenerated = true;
        CutoffSetsGenerated++;
        return true;
    }

#ifdef DEBUG_TIMING
    End = clock();
  ExploreTiming.AddTime("RULE::NextConditionSet", Start, End);
#endif

}

/**********************************************************************
Function: NextCandidateCutoffSet_Rule()
Category: Modifiers
Scope: public
In: Optimze, on sensitivity or accuracy
Out: bool, succeeded or not.
Description: Generates the next CutoffSet for the rule with
sensitivity and specificity optimization on CP en FP.
**********************************************************************/
bool RULE::NextCandidateCutoffSet_Rule(PERFORMANCE_MEASURE Optimize) {

    bool Incremented = false;
    int ConjunctionNo;

    if (!CutoffSetGenerated){
        /* if first time for this featureset then walk from left to right because of
           correct cumulative sets generation */
        for (ConjunctionNo=0;ConjunctionNo<Conjunctions.size();ConjunctionNo++) {
            if (NextCandidateCutoffSet_Conjunction(ConjunctionNo,Optimize,true)) {
                Incremented = true;
            } else {
                CutoffSetGenerated = false;
                return false;
            }
        }
        CutoffSetGenerated = true;
        CutoffSetsGenerated++;
        return true;
    }

    for (ConjunctionNo=Conjunctions.size()-1; ConjunctionNo>=0 && !Incremented; ) {
        if (NextCandidateCutoffSet_Conjunction(ConjunctionNo,Optimize,false)) {
            Incremented = true;
        } else {
            ConjunctionNo--;
        }
    }

    // Is a new cutoffset found for a conjunction?
    if (Incremented) {
        // Reset all conjunctions to the right
        for (ConjunctionNo++; ConjunctionNo<(int)Conjunctions.size(); ConjunctionNo++) {
            if (!NextCandidateCutoffSet_Conjunction(ConjunctionNo,Optimize,true)) {
                CutoffSetGenerated = false;
                return false;
            }
        }
        CutoffSetGenerated = true;
        CutoffSetsGenerated++;
        return true;
    } else {
        CutoffSetGenerated = false;
        return false;
    }
}

/**********************************************************************
Function: NextCandidateCutoffSet_Conjunction()
Category: Modifiers
Scope: public
In: ConjunctionNo, number of current conjunction
    Optimize, on sensitivity of accuracy
    Reset, should the conjunction be reset to first cutoffset
Out: bool, succeeded or not.
Description: Generates the next CutoffSet for a conjunction with
FP and CP optimization.
**********************************************************************/
bool RULE::NextCandidateCutoffSet_Conjunction(int ConjunctionNo, PERFORMANCE_MEASURE Optimize, bool Reset) {

    int ConditionNo;
    int CPMin;
    bool CPOK = false;
    bool FPOK = false;
    bool Found = false;

    unsigned int NrNegObservations = Conjunctions[0].Conditions[0].Cutoffs[0].Set.NegativeBag.size();
    unsigned int NrPosObservations = Conjunctions[0].Conditions[0].Cutoffs[0].Set.PositiveBag.size();

    CONJUNCTION* CurrentConjunction;
    CONDITION* CurrentCondition;
    CurrentConjunction = &Conjunctions[ConjunctionNo];
    CurrentConjunction->Changed = true;

    //Calculate CPMin and FPConstraint

    //Make CPMIN default -1 for any conjunctions for which no CPMin can be used
    CPMin = -1;

    if (Optimize == SENSITIVITY){
        //SENSITIVITY
        if (Conjunctions.size()==1) {
            CPMin = CPBest;
        } else {
            if (ConjunctionNo==(int)Conjunctions.size()-1) {
                CPMin = CPBest - Conjunctions[ConjunctionNo-1].CumulativeSet.CorrectPositive;
            } else {
                if (Conjunctions[ConjunctionNo+1].Size==1) {
                    if (ConjunctionNo-1>=0) {
                        CPMin = CPBest - Conjunctions[ConjunctionNo-1].CumulativeSet.CorrectPositive;
                    } else {
                        CPMin = CPBest;
                    }
                    int r=Conjunctions.size();
                    for (unsigned int j=ConjunctionNo+1; j < Conjunctions.size(); j++) {
                        // subtract CPmax also in conjunctions larger than 1 not on now
                        //      int CPMax=0;
                        //determine CPmax
                        //      for (int m=0;m<Conjunctions[ConjunctionNo].Conditions.size();m++) {
                        //         CurrentCondition = &CurrentConjunction->Conditions[m];
                        //         if (CurrentCondition->Cutoffs[0].Set.CorrectPositive>CPMax){
                        //          CPMax=CurrentCondition->Cutoffs[0].Set.CorrectPositive;
                        //         }
                        //      }
                        CPMin -= Conjunctions[j].Conditions[0].Cutoffs[0].Set.CorrectPositive;
                        //      CPMin -= CPMax;
                    }
                }
            }
        }
        // FPConstraint is constant in this case and depends on specificity constraint
    } else {
        //ACCURACY
        if (Conjunctions.size()==1) {
            CPMin = CTBest - NrNegObservations;
        } else {
            if (ConjunctionNo==(int)Conjunctions.size()-1) {
                CPMin = CTBest - NrNegObservations - Conjunctions[ConjunctionNo-1].CumulativeSet.CorrectPositive;
            } else {
                if (Conjunctions[ConjunctionNo+1].Size==1) {
                    if (ConjunctionNo-1>=0) {
                        CPMin = CTBest - NrNegObservations - Conjunctions[ConjunctionNo-1].CumulativeSet.CorrectPositive;
                    } else {
                        CPMin = CTBest - NrNegObservations;
                    }
                    // substract all max CPs of conjunctions that follow
                    for (unsigned int j=ConjunctionNo+1; j < Conjunctions.size(); j++) {
                        CPMin -= Conjunctions[j].Conditions[0].Cutoffs[0].Set.CorrectPositive;
                    }
                }
            }
        }
        /* FP Constraint now dependents on CTBest
        suppose CTbest = 100 and NrNegObservations = 80, NrPosObservations=60
        that means that at least 20 PosObservations should correct
        zo FPConstraint = 40 *. The same holds voor CPMin.*/

        FPConstraint = NrNegObservations - CTBest + NrPosObservations;
    }

    // check if this is the first cutoffset for this conjunction
    if (Reset) {
        // Set all cutoffs to 0 (max CP)
        for (unsigned int j=0; j<Conjunctions[ConjunctionNo].Conditions.size(); j++) {
            CurrentConjunction->Conditions[j].CutoffNumber=0;
        }
        // check FP Constraint
        Conjunctions[ConjunctionNo].CalculatePerformance();
        if (Conjunctions[ConjunctionNo].ConjunctionSet.FalsePositive<=FPConstraint) {
            FPOK = true;
        }

        // check CP Constraint
        if (Conjunctions[ConjunctionNo].ConjunctionSet.CorrectPositive>CPMin) {
            CPOK = true;
        } else CPOK = false;
        Found = CPOK && FPOK;
    }


    // Determine next cutoff set that fullfils both CPMin en FPContraint
    for (int j= Conjunctions[ConjunctionNo].Conditions.size()-1; j>=0 && !Found; ) {
        CurrentCondition = &CurrentConjunction->Conditions[j];
        // Condition can be incremented?
        if (CurrentCondition->CutoffNumber+1<CurrentCondition->Cutoffs.size()) {
            // Condition can be incremented on CP?
            if (CurrentCondition->Cutoffs[CurrentCondition->CutoffNumber+1].Set.CorrectPositive>CPMin) {

                CPOK = true;
                CurrentCondition->CutoffNumber++;
                // Reset any conditions after incremented condition
                for (unsigned int k=j+1; k<CurrentConjunction->Conditions.size(); k++) {
                    CurrentConjunction->Conditions[k].CutoffNumber = 0;
                }
                j=CurrentConjunction->Conditions.size()-1;

                // Calculate and compare FP
                CurrentConjunction->CalculatePerformance();
                if (CurrentConjunction->ConjunctionSet.FalsePositive<=FPConstraint) {
                    FPOK = true;
                }

            } else {
                CPOK = false;
                if (CurrentCondition->CutoffNumber==0) {
                    // one of the FeatureOperators does not have any CP>CPMin
                    return false;
                }
                // go one condition back
                j--;
            }
        } else {
            // go one condition back
            j--;
        }
        Found = CPOK && FPOK;
    }

    // return result
    if (!Found) {
        return false;
    } else {
        //Update CumulativeSets
        if (ConjunctionNo>0){
            CurrentConjunction->CumulativeSet = Conjunctions[ConjunctionNo-1].CumulativeSet;
            CurrentConjunction->CumulativeSet = CurrentConjunction->CumulativeSet |
                                                CurrentConjunction->ConjunctionSet;
        } else {
            CurrentConjunction->CumulativeSet = CurrentConjunction->ConjunctionSet;
        }
        CurrentConjunction->CumulativeSet.SummariseBags();

        return true;
    }
}

/**********************************************************************
Function: SetRule()
Category: Modifiers
Scope: public
In: CANDIDATE, a candidate rule
Out: bool, function succeeded/did not succeed in setting the complete rule
Description: Sets the rule as far as possible. The function will check to see if the
lists given as input are correct.
**********************************************************************/
bool RULE::SetRule(CANDIDATE Candidate) {
#ifdef DEBUG_TIMING
    clock_t Start, End;
  Start = clock();
#endif

    unsigned int CandidateLength=0;

    for (unsigned int i=0; i < Candidate.Conjunctions.size(); i++) {
        CandidateLength += Candidate.Conjunctions[i];
    }

    // Set RuleLength
    Conjunctions.clear();
    SetCurrentLength(CandidateLength);

    unsigned int ConditionNo=0;
    CONJUNCTION* CurrentConjunction;
    CONDITION*   CurrentCondition;
    bool FeatureOperatorFound = true;

    CombinationGenerated = true;
    FeatureSetGenerated = false;
    CutoffSetGenerated = false;

    for (unsigned int i=0; i<Candidate.Conjunctions.size(); i++) {                // Create Conjunctions
        AddConjunction(Candidate.Conjunctions[i]);
        CurrentConjunction = &Conjunctions[Conjunctions.size()-1];
        CurrentConjunction->Changed = true;
        for (unsigned int j=0; j < Candidate.Conjunctions[i]; j++) {                // Traverse conditions in current conjunction
            // Are at least for all featureoperators values set?
            if (Candidate.Features.size()==CandidateLength && Candidate.Operators.size()==CandidateLength) {
                FeatureOperatorFound = false;
                for (unsigned int k=0; k < FeatureOperators.size(); k++) {              // Traverse FeatureOperators and push_back the right one to the current conjunction
                    // If FeatureNumber and Operator are the same
                    if (FeatureOperators[k].FeatureNumber==Candidate.Features[ConditionNo] && FeatureOperators[k].Operator==Candidate.Operators[ConditionNo]) {
                        FeatureOperatorFound = true;
                        CurrentConjunction->Conditions.push_back(FeatureOperators[k]);      // Push FeatureOperator to the back of current conjunction
                        if (CandidateLength==Candidate.Cutoffs.size()) {                    // Are at least for all cutoffs values set?
                            CurrentCondition = &CurrentConjunction->Conditions[CurrentConjunction->Conditions.size()-1];
                            // Set cutoff for current condition
                            for (unsigned int l=0; l < CurrentCondition->Cutoffs.size(); l++) {
                                if (CurrentCondition->Cutoffs[l].Value == Candidate.Cutoffs[ConditionNo]) {
                                    CurrentCondition->CutoffNumber = l;
                                }
                            }
                            CutoffSetGenerated = true;
                        }
                    }

                }
                if (!FeatureOperatorFound) {
                    return false;
                }
                FeatureSetGenerated = true;
            }
            ConditionNo++;
        }
    }
    // Check which part is correctly set
#ifdef DEBUG_TIMING
    End = clock();
  ExploreTiming.AddTime("RULE::SetRule", Start, End);
#endif

    return true;
}

/**********************************************************************
Function: SetRestriction()
Category: Modifiers
Scope: public
In: string, string representing a rule
Out: bool, rule is set/not set
Description: Interprets a string representing a rule and sets the rule.
**********************************************************************/
bool RULE::SetRestriction(string StringRule) {
    Restriction.Clear();
    if (Restriction.Interpret(StringRule) && Restriction.IsValid()) {
        RestrictionSet = true;
        return true;
    }
    return false;
}

/**********************************************************************
Function: RemoveRestriction()
Category: Modifiers
Scope: public
In: -
Out: -
Description: Removes any restriction on the rule.
**********************************************************************/
void RULE::RemoveRestriction() {
    Restriction.Clear();
    RestrictionSet = false;
}

/**********************************************************************
Function: SetPrintCombinations()
Category: Modifiers
Scope: public
In: bool, yes or no
Out: -
Description: Rule must cout combinations that it generates.
**********************************************************************/
void RULE::SetPrintCombinations(bool Setting) {
    IsPrintCombinations = Setting;
}

/**********************************************************************
Function: SetPrintFeatureSets()
Category: Modifiers
Scope: public
In: bool, yes or no
Out: -
Description: Rule must cout featuresets that it generates.
**********************************************************************/
void RULE::SetPrintFeatureSets(bool Setting) {
    IsPrintFeatureSets = Setting;
}

/**********************************************************************
Function: SetPrintCutoffSets()
Category: Modifiers
Scope: public
In: bool, yes or no
Out: -
Description: Rule must cout conditionsets that it generates.
**********************************************************************/
void RULE::SetPrintCutoffSets(bool Setting) {
    IsPrintCutoffSets = Setting;
}

/**********************************************************************
Function: SetPrintCutoffSets()
Category: Modifiers
Scope: public
In: bool, yes or no
Out: -
Description: Rule must cout conditionsets that it generates.
**********************************************************************/
void RULE::SetPrintCutoffSetsBestLength(bool Setting) {
    IsPrintCutoffSetsBestLength = Setting;
}


/**********************************************************************
Function: SetTestMode()
Category: Modifiers
Scope: public
In: PARTITION_TYPE, partition type rules should be tested on
Out: -
Description: Selects the correct partitiontype.
**********************************************************************/
void RULE::SetTestMode(PARTITION_TYPE PType) {
    vector<CLASSES>* PartitionClasses;

    TestMode= PType;

    for (unsigned int j=0; j<FeatureOperators.size(); j++) {
        switch (PType) {
            case LEARN:
                PartitionClasses = (*Features)[FeatureOperators[j].FeatureNumber].GetLearnClasses();
                break;
            case VALIDATION:
                PartitionClasses = (*Features)[FeatureOperators[j].FeatureNumber].GetValidationClasses();
                break;
            case TRAIN: // both learn and validation set
                PartitionClasses = (*Features)[FeatureOperators[j].FeatureNumber].GetTrainClasses();
                break;
        }
        FeatureOperators[j].InitialiseSets(PartitionClasses);
    }
}

/**********************************************************************
Function: Prune()
Category: Modifiers
Scope: public
In: -
Out: -
Description: Prunes the cutoffs of continuous-valued features
          based on subsumption pruning
**********************************************************************/
void RULE::Prune() {
    vector<CONDITION>::iterator CurrentFeatureOperator(FeatureOperators.begin());
    vector<CONDITION>::iterator LastFeatureOperator(FeatureOperators.end());

    for (; CurrentFeatureOperator != LastFeatureOperator; CurrentFeatureOperator++) {
        if ((*CurrentFeatureOperator).Operator != EQUAL) {
            (*CurrentFeatureOperator).Prune();
        }
    }
}


/**********************************************************************
Function: SetFPConstraint()
Category: Modifiers
Scope: public
In: float, specificity constraint value
Out: -
Description: Calculates FPConstraint based on a specificity (between
0 and 1) and the number of negative observations.
**********************************************************************/
void RULE::SetFPConstraint(float Specificity) {

    int NegBagSize = FeatureOperators[0].Cutoffs[0].Set.NegativeBag.size();
    //cout << "RULE::SetFPConstraint" << endl;
    //cout << "1 - SPECIFICITY: " << (1-Specificity) << endl;
    //cout << "NegBagSize: " << NegBagSize << endl;
    if (((1-Specificity) * NegBagSize)>=0) {
        FPConstraint = (1-Specificity) * NegBagSize;
    }
}

/**********************************************************************
Function: SetCPBest()
Category: Modifiers
Scope: public
In: float, sensitivity constraint value
Out: -
Description: Calculates CPBest
**********************************************************************/
void RULE::SetCPBest(float Sensitivity) {

    int PosBagSize = FeatureOperators[0].Cutoffs[0].Set.PositiveBag.size();
    //cout << "RULE::SetCPBest" << endl;
    //cout << "Sensitivity: " << Sensitivity << endl;
    //cout << "PosBagSize: " << PosBagSize << endl;
    if ((Sensitivity) * PosBagSize>=0) {
        CPBest = (Sensitivity) * PosBagSize;
    }
}

/**********************************************************************
Function: SetCTBest()
Category: Modifiers
Scope: public
In: float, accuracy constraint value
Out: -
Description: Calculates CTBest
**********************************************************************/
void RULE::SetCTBest(float Accuracy) {
    // ???
    int PosBagSize = FeatureOperators[0].Cutoffs[0].Set.PositiveBag.size();
    int NegBagSize = FeatureOperators[0].Cutoffs[0].Set.NegativeBag.size();
//  cout << "RULE::SetCTBest" << endl;
//  cout << "Accuracy: " << Accuracy << endl;
//  cout << "PosBagSize: " << PosBagSize << endl;
//  cout << "NegBagSize: " << NegBagSize << endl;
    if ((Accuracy * (PosBagSize + NegBagSize))>=0) {
        CTBest = (Accuracy) * (PosBagSize + NegBagSize);
    }
}

/**********************************************************************
Function: SetMaximizeMeasure()
Category: Modifiers
Scope: public
In: PERFORMANCE_MEASURE*, pointer to a performance measure
Out: -
Description: Sets a pointer to the original performance measure,
needed for rule-optimisation.
**********************************************************************/
void RULE::SetMaximizeMeasure(PERFORMANCE_MEASURE* Measure) {
    MaximizeMeasure = Measure;
}

/**********************************************************************
Function: SortCutoffsByCP()
Category: Modifiers
Scope: public
In: -
Out: -
Description: Sorts the cutoffs of each FeatureOperator by CP (number
of Correct-Positive observations).
**********************************************************************/
void RULE::SortCutoffsByCP() {
    vector<CONDITION>::iterator CurrentFeatureOperator(FeatureOperators.begin());
    vector<CONDITION>::iterator LastFeatureOperator(FeatureOperators.end());

    for (; CurrentFeatureOperator != LastFeatureOperator; CurrentFeatureOperator++) {
        (*CurrentFeatureOperator).SortCutoffsByCP();
    }
}

/**********************************************************************
Function: SortCutoffsByCT()
Category: Modifiers
Scope: public
In: -
Out: -
Description: Sorts the cutoffs of each FeatureOperator by CP (number
of Correct-Positive observations).
**********************************************************************/
void RULE::SortCutoffsByCT() {
    vector<CONDITION>::iterator CurrentFeatureOperator(FeatureOperators.begin());
    vector<CONDITION>::iterator LastFeatureOperator(FeatureOperators.end());

    for (; CurrentFeatureOperator != LastFeatureOperator; CurrentFeatureOperator++) {
        (*CurrentFeatureOperator).SortCutoffsByCT();
    }
}

/**********************************************************************
Function: IsMandatorySet()
Category: Modifiers
Scope: public
In: -
Out: bool, mandatory features are/aren't included in the rule
Description: Checks whether mandatory features occur in the rule.
**********************************************************************/
bool RULE::IsMandatorySet() {
    return MandatorySet;
}

/**********************************************************************
Function: CheckFeatureSetMandatory()
Category: Modifiers
Scope: public
In: -
Out: bool, mandatory features are/aren't included in the rule
Description: Checks whether all mandatory features occur in the rule.
**********************************************************************/
bool RULE::CheckFeatureSetMandatory() {
    vector<CONJUNCTION>::iterator CurrentConjunction;
    vector<CONJUNCTION>::iterator LastConjunction(Conjunctions.end());
    vector<CONDITION>::iterator CurrentCondition;
    vector<CONDITION>::iterator LastCondition;

    vector<FEATURE>::iterator CurrentFeature((*Features).begin());
    vector<FEATURE>::iterator LastFeature((*Features).end());

    bool Found = false;

    for (; CurrentFeature != LastFeature; CurrentFeature++) {
        if ((*CurrentFeature).IsMandatory()){
            Found = false;
            for (CurrentConjunction = Conjunctions.begin(); CurrentConjunction != LastConjunction && !Found; CurrentConjunction++) {
                CurrentCondition = (*CurrentConjunction).Conditions.begin();
                LastCondition = (*CurrentConjunction).Conditions.end();
                for (; CurrentCondition != LastCondition; CurrentCondition++) {
                    if ((*CurrentCondition).FeatureNumber==(*CurrentFeature).GetOrder())
                        Found = true;
                }
            }
            if (!Found) {
                return false;
            }
        }
    }
    return true;
}

/**********************************************************************
Function: CheckCombinationRestriction()
Category: Modifiers
Scope: public
In: -
Out: bool, conjunction sizes all occur in the rule
Description: Checks whether all conjunction sizes of the restriction
are present in the current combination.
**********************************************************************/
bool RULE::CheckCombinationRestriction() {
    if (RestrictionSet) {
        vector<unsigned int>::iterator CurrentConjunction(Restriction.Conjunctions.begin());
        vector<unsigned int>::iterator LastConjunction(Restriction.Conjunctions.end());
        bool Found;

        if (Restriction.Conjunctions.size() == Conjunctions.size()) {
            for (; CurrentConjunction != LastConjunction; CurrentConjunction++) {
                Found = false;
                for (unsigned int i=0; i<Conjunctions.size() && !Found; i++) {
                    if ((*CurrentConjunction) == Conjunctions[i].Size) {                     // Are these two conjunctions the same size?
                        Found = true;                                                         // One restriction combination has been found
                    }
                }
                if (!Found) {
                    return false;                                                           // At least one restriction combination was not found
                }
            }
        } else return false;
    }
    return true;                                                                  // All clear
}


/**********************************************************************
Function: CheckStopFeature(CANDIDATE StopRule)
Category: Modifiers
Scope: public
In: -
Out: bool, conjunction sizes all occur in the rule
Description: Checks whether the stoprule has been reached. This can be a
combination or featureoperatorset.
**********************************************************************/
bool RULE::CheckStopFeature(CANDIDATE StopRule) {
    CONJUNCTION* CurrentConjunction;
    CONDITION*   CurrentCondition;
    unsigned int ConditionNo = 0;

    // If no features set in restriction, then ignore restriction
    if (StopRule.Features.size()==0) return false;

    // Are at least for all featureoperators values set?
    if ((CurrentLength==StopRule.Features.size()) && (CurrentLength==StopRule.Operators.size())) {
        // Traverse all conjunctions of candidate
        for (unsigned int k=0; k<Conjunctions.size(); k++){
            CurrentConjunction = &Conjunctions[k];
            // Check conjunction sizes first before comparing FeatureOperators
            if ((*CurrentConjunction).Size==StopRule.Conjunctions[k]) {
                for (unsigned int i=0; i<(*CurrentConjunction).Size; i++) {
                    if ((*CurrentConjunction).Conditions[i].FeatureNumber!=StopRule.Features[ConditionNo] ||
                        (*CurrentConjunction).Conditions[i].Operator!=StopRule.Operators[ConditionNo]) {
                        return false;
                    }
                    ConditionNo++;
                }
            } else return false;
        }
    } else return false;
    return true;
}

/**********************************************************************
Function: CheckStopCombination(CANDIDATE StopRule)
Category: Modifiers
Scope: public
In: -
Out: bool, conjunction sizes all occur in the rule
Description: Checks whether the stopcombination has been reached.
**********************************************************************/
bool RULE::CheckStopCombination(CANDIDATE StopRule) {
    if (StopRule.Features.size()>0) return false;
    if (StopRule.Conjunctions.size() != Conjunctions.size()) return false;

    for (unsigned int j=0; j<StopRule.Conjunctions.size(); j++) {
        if (Conjunctions[j].Size != StopRule.Conjunctions[j]) {                     // Are these two conjunctions the same size?
            return false;                                                             // One restriction combination has been found
        }
    }
    return true;
}

/**********************************************************************
Function: CheckFeatureSetRestriction()
Category: Modifiers
Scope: public
In: -
Out: bool, features all occur in the rule, in the right conjunctions
Description: Checks whether all features of the restriction occur
in the rule.
**********************************************************************/
bool RULE::CheckFeatureSetRestriction() {
    if (RestrictionSet && Restriction.Features.size()>0) {
        vector<unsigned int>::iterator CurrentConjunction(Restriction.Conjunctions.begin()); // Iterator for restriction conjunctions
        vector<unsigned int>::iterator LastConjunction(Restriction.Conjunctions.end());      // Last iterator for restriction conjunctions
        bool Found;
        unsigned int FeatureIndex=0;

        for (; CurrentConjunction != LastConjunction; CurrentConjunction++) {       // Iterate through restriction conjunctions
            Found = false;                                                            // Try to find current conjunction in the rule somewhere
            for (unsigned int i=0; i<Conjunctions.size() && !Found; i++) {
                if (Conjunctions[i].Size == (*CurrentConjunction)) {                    // Are these two conjunctions the same size?
                    if (MatchConjunctionFeatureSet(FeatureIndex, i)) {
                        Found = true;
                    }
                }
            }
            FeatureIndex += (*CurrentConjunction);

            if (!Found) {
                return false;                                                           // At least one restriction combination was not found
            }
        }
    }
    return true;                                                                  // All clear
}

/**********************************************************************
Function: CheckCutoffSetRestriction()
Category: Modifiers
Scope: public
In: -
Out: bool, current rule matches restriction
Description: Checks whether all conjunction sizes features, cutoffs
and operators occur somewhere in the rule.
**********************************************************************/
bool RULE::CheckCutoffSetRestriction() {
    if (RestrictionSet && Restriction.Features.size()>0 && Restriction.Operators.size()>0) {
        vector<unsigned int>::iterator CurrentConjunction(Restriction.Conjunctions.begin());
        vector<unsigned int>::iterator LastConjunction(Restriction.Conjunctions.end());
        bool Found;
        unsigned int FeatureIndex=0;

        for (; CurrentConjunction != LastConjunction; CurrentConjunction++) {       // Iterate through restriction conjunctions
            Found = false;                                                            // Try to find current conjunction in the rule somewhere
            for (unsigned int i=0; i<Conjunctions.size() && !Found; i++) {
                if (Conjunctions[i].Size == (*CurrentConjunction)) {                    // Are these two conjunctions the same size?
                    if (MatchConjunctionCutoffSet(FeatureIndex, i)) {
                        Found = true;
                    }
                }
            }
            FeatureIndex += (*CurrentConjunction);

            if (!Found) {
                return false;                                                           // At least one restriction combination was not found
            }
        }
    }
    return true;                                                                  // All clear
}

/**********************************************************************
Function: MatchConjunctionFeatureSet()
Category: Modifiers
Scope: private
In: unsigned int, index where features can be found of the restriction
    unsigned int, index of the conjunction to be checked in the current rule
Out: bool, current conjunction current restriction conjunction
Description: Checks whether the features of a specific conjunction of
the restriction matches a conjunction of the current rule.
**********************************************************************/
bool RULE::MatchConjunctionFeatureSet(unsigned int RestrictionIndex, unsigned int ConjunctionNumber) {
    vector<CONDITION>::iterator CurrentCondition;
    vector<CONDITION>::iterator LastCondition(Conjunctions[ConjunctionNumber].Conditions.end());
    bool Found;

    unsigned int ConjunctionSize = Conjunctions[ConjunctionNumber].Size;
    unsigned int CompareFeature;
    OPERATOR CompareOperator;

    for (unsigned int i=0; i<ConjunctionSize; i++) {

        Found = false;
        CompareFeature = Restriction.Features[RestrictionIndex]-1;
        CompareOperator = Restriction.Operators[RestrictionIndex];
        CurrentCondition = Conjunctions[ConjunctionNumber].Conditions.begin();

        for (; CurrentCondition != LastCondition && !Found; CurrentCondition++) {
            if (CurrentCondition->FeatureNumber==CompareFeature) {
                if (CurrentCondition->Operator==CompareOperator || CompareOperator==NONE) {
                    Found = true;
                }
            }
        }

        if (!Found) {
            return false;
        }

        RestrictionIndex++;

    }
    return true;
}

/**********************************************************************
Function: MatchConjunctionCutoffSet()
Category: Modifiers
Scope: private
In: unsigned int, index where features can be found of the restriction
    unsigned int, index of the conjunction to be checked in the current rule
Out: bool, current conjunction current restriction conjunction
Description: Checks whether the features and optionally any cutoffs and
operators of a specific conjunction of the restriction matches a conjunction
of the current rule.
**********************************************************************/
bool RULE::MatchConjunctionCutoffSet(unsigned int RestrictionIndex, unsigned int ConjunctionNumber) {
    vector<CONDITION>::iterator CurrentCondition;
    vector<CONDITION>::iterator LastCondition(Conjunctions[ConjunctionNumber].Conditions.end());
    bool Found;

    unsigned int ConjunctionSize = Conjunctions[ConjunctionNumber].Size;
    unsigned int CompareFeature;
    OPERATOR CompareOperator;
    string CompareCutoff, CutoffValue;
    double Precision = 1e-6;

    for (unsigned int i=0; i<ConjunctionSize; i++) {

        Found = false;
        CompareFeature = Restriction.Features[RestrictionIndex]-1;
        CompareOperator = Restriction.Operators[RestrictionIndex];
        CompareCutoff = Restriction.Cutoffs[RestrictionIndex];
        CurrentCondition = Conjunctions[ConjunctionNumber].Conditions.begin();

        for (; CurrentCondition != LastCondition && !Found; CurrentCondition++) {
            if (CurrentCondition->FeatureNumber==CompareFeature) {
                if (CurrentCondition->Operator==CompareOperator || CompareOperator==NONE) {
                    CutoffValue = CurrentCondition->Cutoffs[CurrentCondition->CutoffNumber].Value;
                    if ((CutoffValue == CompareCutoff)|| CompareCutoff=="") {
                        Found = true;
                    }
                }
            }
        }

        if (!Found) {
            return false;
        } else {
            RestrictionIndex++;
        }

    }
    return true;
}

/**********************************************************************
Function: NextCombinationRestriction()
Category: Modifiers
Scope: public
In: -
Out: bool, succeeded or not.
Description: Generates the next partition of the rule, keeping into
account any restrictions that were set previously.
**********************************************************************/
bool RULE::NextCombinationGenerator() {
    if (RestrictionSet || MandatorySet) { // || Rule.IsMandatorySet()
        do {

            if (!NextCombination()) {
                return false;
            }

        } while (!CheckCombinationRestriction());
        return true;
    } else {
        return NextCombination();
    }
}

/**********************************************************************
Function: NextFeatureSetRestriction()
Category: Modifiers
Scope: public
In: -
Out: bool, succeeded or not.
Description: Generates the next featureset of the rule, keeping into
account any restrictions and Mandatory that were set previously.
**********************************************************************/
bool RULE::NextFeatureSetGenerator(int FOperatorNr_start, int FOperatorNr_end) {

    if (RestrictionSet || MandatorySet) {
        do {
            if (!NextFeatureSet(FOperatorNr_start, FOperatorNr_end)) {
                return false;
            }
        } while (!CheckFeatureSetRestriction() || !CheckFeatureSetMandatory());
        return true;
    } else {
        return NextFeatureSet(FOperatorNr_start, FOperatorNr_end);
    }

}

/**********************************************************************
Function: NextCutoffSetRestriction()
Category: Modifiers
Scope: public
In: -
Out: bool, succeeded or not.
Description: Generates the next instance of the rule, keeping into
account any restrictions that were set previously.
**********************************************************************/
bool RULE::NextCutoffSetGenerator() {
    if (RestrictionSet || MandatorySet) {
        do {
            if (!NextCutoffSet()) {
                return false;
            }
        } while (!CheckCutoffSetRestriction());
        return true;
    } else {
        return NextCutoffSet();
    }
}


float RULE::gammln(float xx) {
    double x,y,tmp,ser;
    static double cof[6] = {76.18009172947146,
                            -86.50532032941677,
                            24.01409824083091,
                            -1.231739572450155,
                            0.1208650973866179e-2,
                            -0.5395239384953e-5};
    int j;

    y=x=xx;
    tmp=x+5.5;
    tmp -= (x+0.5)*log(tmp);
    ser=1.000000000190015;
    for (j=0;j<=5;j++) ser += cof[j]/++y;
    return -tmp+log(2.5066282746310005*ser/x);
}

float RULE::factrl(int n) {
    static int ntop=4;
    static float a[33]={1.0,1.0,2.0,6.0,24.0};
    int j;

    if (n > 32) return exp(gammln(n+1.0));

    while (ntop<n) {
        j=ntop++;
        a[ntop]=a[j]*ntop;
    }
    return a[n];
}

float RULE::factln(int n) {
    static float a[101];

    if (n <= 1) return 0.0;
    if (n <= 100) return a[n] ? a[n] : (a[n]=gammln(n+1.0));
    else return gammln(n+1.0);
}

float RULE::bico(int n, int k) {
    return floor(0.5+exp(factln(n)-factln(k)-factln(n-k)));
}

/**********************************************************************
Function: Reset()
Category: Modifiers
Scope: public
In: -
Out: -
Description: Resets complexity measures.
**********************************************************************/
void RULE::ResetComplexity() {
#ifdef DEBUG_TIMING
    clock_t Start, End;
  Start = clock();
#endif

    CombinationGenerated = false;
    FeatureSetGenerated = false;
    CutoffSetGenerated = false;

    CombinationsGenerated = 0;
    FeatureSetsGenerated = 0;
    CutoffSetsGenerated = 0;

    CountCandidates = 0;

}

/**********************************************************************
Function: Reset()
Category: Modifiers
Scope: public
In: -
Out: -
Description: Resets complexity measures.
**********************************************************************/
void RULE::ResetCountCandidates() {
#ifdef DEBUG_TIMING
    clock_t Start, End;
  Start = clock();
#endif
    CountCandidates = 0;
}

// BELOW functions are moved from explore.cpp.

/**********************************************************************
Function: TestRule()
Category: Modifiers
Scope: public
In: -
Out: -
Description: Test a rule against the current learning partition.
**********************************************************************/
bool RULE::TestRule(bool Initialised, vector<CONSTRAINT> Constraints, float CandidatePerformance, PERFORMANCE_MEASURE MaximizeMeasure, bool RestrictionSet,
                    RULE_OUTPUT_METHOD RuleOutputMethod, bool IsPrintPerformance, bool IsPrintSets) {
#ifdef DEBUG_TIMING
    clock_t Start, End;
  Start = clock();
#endif
    bool Found = false;
    bool Candidate = false;

    if (Initialised) {
        PERFORMANCE CurrentPerformance = CalculatePerformance();

        // Does rule satisfy constraints?
        if (CompareConstraints(CurrentPerformance, Initialised, Constraints)) {

            // Are there any candidates?
            if (CandidatePerformance != -1) {

                // Is rule-performance best until now?
                Candidate = true;
                CountCandidates++;

                if (CompareBestCandidate(CurrentPerformance, Initialised, CandidatePerformance, MaximizeMeasure)) {
                    // PartitionCandidates = SaveCandidate(CurrentPerformance, PartitionCandidates, MaximizeMeasure, RestrictionSet);
                    Found = true;
                }
            } else {
                // PartitionCandidates = SaveCandidate(CurrentPerformance, PartitionCandidates, MaximizeMeasure, RestrictionSet);

                Candidate = true;
                CountCandidates++;
                Found = true;
            }
        }
    }

    switch (RuleOutputMethod) {
        case EVERY:
            PrintCutoffSet();
            if (IsPrintPerformance) {
                PrintPerformance();
            }
            if (IsPrintSets) {
                PrintSets();
            }
            break;
        case INCREMENT:
            if (Found) {
                PrintCutoffSet();
                if (IsPrintPerformance) {
                    PrintPerformance();
                }
                if (IsPrintSets) {
                    PrintSets();
                }
            }
            break;
    }

#ifdef DEBUG_TIMING
    End = clock();
    ExploreTiming.AddTime("EXPLORE::TestRule", Start, End);
#endif
    // TODO: indicate when partition candidates NOT updated or return Candidate instead?
    // return Candidate;
    return Found;
}


/**********************************************************************
Function: CompareConstraints()
Category: Modifiers
Scope: public
In: -
Out: bool, performance is/is not above prerequisites
Description: Compares current performance with any prerequisites.
**********************************************************************/
bool RULE::CompareConstraints(PERFORMANCE CurrentPerformance, bool Initialised, vector<CONSTRAINT> Constraints) {
#ifdef DEBUG_TIMING
    clock_t Start, End;
  Start = clock();
#endif

    if (Initialised) {
        vector<CONSTRAINT>::iterator CurrentConstraint(Constraints.begin());
        vector<CONSTRAINT>::iterator LastConstraint(Constraints.end());

        float RuleValue,ConstraintValue;

        for (; CurrentConstraint != LastConstraint; CurrentConstraint++) {
            if ((*CurrentConstraint).Value>0) {
                switch ((*CurrentConstraint).Measure) {
                    case SENSITIVITY:
                        RuleValue = CurrentPerformance.Sensitivity.Value;
                        break;
                    case SPECIFICITY:
                        RuleValue = CurrentPerformance.Specificity.Value;
                        break;
                    case NPV:
                        RuleValue = CurrentPerformance.NPV.Value;
                        break;
                    case PPV:
                        RuleValue = CurrentPerformance.PPV.Value;
                        break;
                    case ACCURACY:
                        RuleValue = CurrentPerformance.Accuracy.Value;
                }
                ConstraintValue = (*CurrentConstraint).Value;
                if (ConstraintValue > RuleValue) {
#ifdef DEBUG_TIMING
                    End = clock();
            ExploreTiming.AddTime("EXPLORE::CompareConstraints", Start, End);
#endif
                    return false;
                }
            }
        }
#ifdef DEBUG_TIMING
        End = clock();
      ExploreTiming.AddTime("EXPLORE::CompareConstraints", Start, End);
#endif
        return true;
    }
#ifdef DEBUG_TIMING
    End = clock();
    ExploreTiming.AddTime("EXPLORE::CompareConstraints", Start, End);
#endif
    return false;
}

/**********************************************************************
Function: CompareBestCandidate()
Category: Modifiers
Scope: public
In: -
Out: bool, performance is/is not above best performance.
Description: Compares the current performance with the best performing
rule found by then.
**********************************************************************/
bool RULE::CompareBestCandidate(PERFORMANCE CurrentPerformance, bool Initialised, float CandidatePerformance, PERFORMANCE_MEASURE MaximizeMeasure) {
#ifdef DEBUG_TIMING
    clock_t Start, End;
  Start = clock();
#endif

    if (Initialised) {
        float RuleValue;
        float CandidateValue;

        CandidateValue = CandidatePerformance;

        switch (MaximizeMeasure) {
            case SENSITIVITY:
                RuleValue = CurrentPerformance.Sensitivity.Value;
                break;
            case SPECIFICITY:
                RuleValue = CurrentPerformance.Specificity.Value;
                break;
            case NPV:
                RuleValue = CurrentPerformance.NPV.Value;
                break;
            case PPV:
                RuleValue = CurrentPerformance.PPV.Value;
                break;
            case ACCURACY:
                RuleValue = CurrentPerformance.Accuracy.Value;
                break;
            case BALANCEDACCURACY:
                RuleValue = CurrentPerformance.BalancedAccuracy.Value;
                break;
            case F1SCORE:
                RuleValue = CurrentPerformance.F1score.Value;
                break;
        }

        if (CandidateValue<=RuleValue) { // TODO: why = included here?
#ifdef DEBUG_TIMING
            End = clock();
        ExploreTiming.AddTime("EXPLORE::CompareBestCandidate", Start, End);
#endif
            return true;
        }

        if (CandidateValue>RuleValue) {
#ifdef DEBUG_TIMING
            End = clock();
        ExploreTiming.AddTime("EXPLORE::CompareBestCandidate", Start, End);
#endif
            return false;
        }
    }
#ifdef DEBUG_TIMING
    End = clock();
    ExploreTiming.AddTime("EXPLORE::CompareBestCandidate", Start, End);
#endif
    return false;
}


/**********************************************************************
Function: SaveCandidate()
Category: Modifiers
Scope: public
In: -
Out: -
Description: Saves the current rule and performance.
**********************************************************************/
CANDIDATE RULE::SaveCandidate(PERFORMANCE_MEASURE MaximizeMeasure, bool RestrictionSet) {
#ifdef DEBUG_TIMING
    clock_t Start, End;
  Start = clock();
#endif

    CANDIDATE Dummy;

    Dummy.Performance = GetPerformance();                                       // Save the performance of the rule

    Dummy.FeatureNames = GetFeatureNames();
    Dummy.Conjunctions = GetConjunctions();                                  // Save a list of partitions (subset sizes)
    Dummy.Features = GetFeatures();                                          // Save a list of current features used
    Dummy.Cutoffs = GetCutoffs();                                            // Save a list of current cutoffs used
    Dummy.Operators = GetOperators();                                        // Save a list of current operators used

    // PartitionCandidates.push_back(Dummy);                                         // Save the performance + rule

    if (MaximizeMeasure==SENSITIVITY && !RestrictionSet) {
        if (RuleSet.CorrectPositive>CPBest) {
            CPBest = RuleSet.CorrectPositive;
        }
    }

    if (MaximizeMeasure==ACCURACY && !RestrictionSet) {
        if ((RuleSet.CorrectPositive + RuleSet.CorrectNegative) > CTBest) {
            CTBest = RuleSet.CorrectPositive + RuleSet.CorrectNegative;
        }
    }

    return Dummy;

#ifdef DEBUG_TIMING
    End = clock();
    ExploreTiming.AddTime("EXPLORE::SaveCandidate", Start, End);
#endif
}

/**********************************************************************
Function: BestLength()
Category: Modifiers
Scope: public
In: -
Out: -
Description: is stop criterium met?
**********************************************************************/
int RULE::FindBestLength(bool Initialised, CANDIDATE PartitionCandidates, PARTITION_METHOD PartitionMethod,PERFORMANCE_MEASURE MaximizeMeasure) {
    float best;
    float current;
    int Opt=0;

    CANDIDATE BestCandidate;

    if (Initialised) {
        SetTestMode(VALIDATION);

        if (PartitionCandidates.IsValid()) {
            // if (PartitionCandidates.size()>0) {
            for (unsigned int i=GetMinRuleLength(); i<=GetMaxRuleLength(); i++){
                BestCandidate = ChooseBestCandidate(i, Initialised, PartitionCandidates, MaximizeMeasure);

                if (BestCandidate.Performance.Accuracy.Value != 0) {  // Check if BestCandidate not empty
                    if (SetRule(BestCandidate))
                    {
                        cout << "RULELENGTH " << i << endl << endl;
                        cout << "Best candidate (within this partition): ";
                        PrintCutoffSet();
                        cout << endl;
                        cout << "Learn-set: ";
                        BestCandidate.Performance.Print();
                        cout << endl;

                        if (!(PartitionMethod==RESUBSTITUTION)){
                            BestCandidate.Performance = CalculatePerformance();          // Test BestCandidate on validation partition
                            cout << "Validation-set: ";
                            BestCandidate.Performance.Print();
                            cout << endl;
                        }
                        switch (MaximizeMeasure){

                            case ACCURACY:
                                current = BestCandidate.Performance.Accuracy.Value;
                                break;
                            case SENSITIVITY:
                                current = BestCandidate.Performance.Sensitivity.Value;
                                break;
                            case SPECIFICITY:
                                current = BestCandidate.Performance.Specificity.Value;
                                break;
                            case NPV:
                                current = BestCandidate.Performance.NPV.Value;
                                break;
                            case PPV:
                                current = BestCandidate.Performance.PPV.Value;
                                break;
                            case BALANCEDACCURACY:
                                current = BestCandidate.Performance.BalancedAccuracy.Value;
                                break;
                            case F1SCORE:
                                current = BestCandidate.Performance.F1score.Value;
                                break;
                        }
                        if (i==1) {
                            best = current;
                            Opt = 1;
                        }
                        else {
                            if (current > best) {
                                best = current;
                                Opt = i;
                            }
                        }
                    }
                }
            }
            return Opt;
        } else {
#if defined(EXPLORE_MPI_DEBUG)
            cout << "--> No Candidates" << endl;
#endif
        }
    }
    return 0;
}



/**********************************************************************
Function: ChooseBestCandidate()
Category: Modifiers
Scope: public
In: insigned int, rule length
Out: -
Description: Retrieves the best candidate and puts it in
BestCandidate.
**********************************************************************/
CANDIDATE RULE::ChooseBestCandidate(unsigned int RuleLength, bool Initialised, CANDIDATE PartitionCandidates, PERFORMANCE_MEASURE MaximizeMeasure) {
#ifdef DEBUG_TIMING
    clock_t Start, End;
  Start = clock();
#endif
//    bool Found = false;
//    CANDIDATE BestCandidate;
//
//    if (Initialised) {
//        tbb::concurrent_vector<CANDIDATE>::iterator CurrentCandidate(PartitionCandidates.begin());
//        tbb::concurrent_vector<CANDIDATE>::iterator LastCandidate(PartitionCandidates.end());
//
//        // TODO: check if better place to create variable
//        BestCandidate = (*CurrentCandidate);
//
//        float CurrentValue;
//        float BestValue;
//
//        while (CurrentCandidate != LastCandidate) {
//            CurrentValue = 0;
//            if ((*CurrentCandidate).Features.size()==RuleLength){
//                switch (MaximizeMeasure) {
//                    case SENSITIVITY:
//                        CurrentValue = (*CurrentCandidate).Performance.Sensitivity.Value;
//                        break;
//                    case SPECIFICITY:
//                        CurrentValue = (*CurrentCandidate).Performance.Specificity.Value;
//                        break;
//                    case NPV:
//                        CurrentValue = (*CurrentCandidate).Performance.NPV.Value;
//                        break;
//                    case PPV:
//                        CurrentValue = (*CurrentCandidate).Performance.PPV.Value;
//                        break;
//                    case ACCURACY:
//                        CurrentValue = (*CurrentCandidate).Performance.Accuracy.Value;
//                        break;
//                }
//
//                if (BestValue<=CurrentValue) {
//                    BestCandidate = (*CurrentCandidate);
//                    BestValue = CurrentValue;
//                    Found = true;
//                }
//            }
//            CurrentCandidate++;
//        }
//    }
//
//#ifdef DEBUG_TIMING
//    End = clock();
//  ExploreTiming.AddTime("EXPLORE::ChooseBestCandidate", Start, End);
//#endif
//
//  if (Found) {
//      return BestCandidate;
//  } else {
//      return CANDIDATE();
//  }

    return PartitionCandidates;
}

/**********************************************************************
Function: GetFeatureOperatorSize()
Category: .
Scope: public
In: -
Out: unsigned
Description: Returns the total number of feature operator pairs -1
**********************************************************************/
unsigned int RULE::GetFeatureOperatorSize() {
    return FeatureOperators.size() - 1;
}
