//----------------------------------------------------------------------------//
// $Header: //depot/Explore/Explore/explore.cpp#16 $
// $Author: keulers $
// $DateTime: 2004/07/09 17:07:22 $
// $Change: 57 $
//----------------------------------------------------------------------------//

#include "explore.h"
#include "../common.h"
#include <math.h>
#include <iostream>
#include <ostream>
#include <mutex>
#include <tbb/concurrent_vector.h>
#include <tbb/parallel_for.h>

std::mutex m0;
std::mutex m1;
std::mutex m2;
std::mutex m3;


#define COMMANDVERSION

const double PROGRESS_INTERVAL=0.1;
const unsigned int BREATHE_INTERVAL=1000;

#ifdef DEBUG_TIMING
extern TIMING ExploreTiming;
#endif

/**********************************************************************
Function: EXPLORE()
Category: Constructors and Destructors
Scope: public
In: -
Out: -
Description: Default constructor for an Explore application.
**********************************************************************/
Explore::Explore() {
    LearnRatio = 0.0;
    PositiveClass = 0;
    NoPartitions = 0;

    NewCandidate = false;
    NewFeatureSet = false;

    NoPartitionsDone = 0;
    Population.SetLearnRatio(&LearnRatio);

    // Internal
    Randomize                   = true;
    PartitionMethodSet          = false;
    NoPartitionsSet             = false;
	LearnTestRatioSet           = false;
    CutoffMethodSet             = false;
    OperatorMethodSet           = false;
    PositiveClassSet            = false;
    RuleOutputMethodSet         = false;
    MaximizeMeasureSet          = false;
    RestrictionSet              = false;
    MandatorySet                = false;
    Initialised                 = false;

    BranchBound                 = true;
	SubSumption                 = true;
	Parallel                    = false;

    // Output
    IsUpdateRealtime            = false;

    IsSavePartitions            = false; // added
    IsPrintSettings             = false;
    IsPrintPartitions           = false;

    IsPrintCombinations         = false;
    IsPrintFeatureSets          = false;
    IsPrintCutoffSets           = false;
    IsPrintPerformance          = false;
    IsPrintSets                 = false;
    IsPrintOperatorMethod       = false;
    IsPrintCutoffMethod         = false;
    IsPrintOperatorValues       = false;
    IsPrintCutoffValues         = false;
    IsPrintFeatureOperators     = false;

}

/**********************************************************************
Function: ~Explore()
Category: Constructors and Destructors
Scope: public
In: -
Out: -
Description: Default destructor for an Explore application.
**********************************************************************/
Explore::~Explore() {
}

/**********************************************************************
Function: RuleComplexity()
Category: Selectors
Scope: public
In: -
Out: double, number of featuresets that can be generated in this rule
Description: Calculate the number of featuresets within a (rule).
**********************************************************************/
double Explore::RuleComplexity() {
  if (Initialised) {
    switch (PartitionMethod) {
      case RESUBSTITUTION:
        NoPartitions = 1;
        return Rule.RuleComplexity(1);
      case HOLDOUT:
        NoPartitions = 1;
        return Rule.RuleComplexity(1);
      case CROSS_VALIDATION:
        return Rule.RuleComplexity(NoPartitions);
    }
  }
  return -1;
}

/**********************************************************************
Function: CombinationComplexity()
Category: Selectors
Scope: public
In: -
Out: double, number of featuresets that can be generated for the
             current combination.
Description: Calculate the number of featuresets within a (rule).
**********************************************************************/
double Explore::CombinationComplexity() {
  if (Initialised) {
    return Rule.CombinationComplexity();
  } else {
    return -1;
  }
}

/**********************************************************************
Function: FeatureSetComplexityLimit()
Category: Selectors
Scope: public
In: -
Out: double, upper limit for the number of cutoffsets that can be
             generated for the current featureset.
Description: Calculate the number of featuresets within a (rule).
**********************************************************************/
double Explore::FeatureSetComplexityLimit() {
  if (Initialised) {
    return Rule.FeatureSetComplexityLimit();
  } else {
    return 0;
  }
}


/**********************************************************************
Function: GetNoObservations()
Category: Selectors
Scope: public
In: -
Out: unsigned int, the number of observations
Description: Get the number of observations in the population.
**********************************************************************/
unsigned int Explore::GetNoObservations() {
  return Population.GetNoObservations();
}

/**********************************************************************
Function: GetClassValueByName()
Category: Selectors
Scope: public
In: string, classvalue
Out: unsigned int, index of classvalue
Description: Get the index of a classvalue
**********************************************************************/
int Explore::GetClassValueByName(string ClassValue){
  return Population.GetClassValueByName(ClassValue);
}

/**********************************************************************
Function: GetFeatureNumber()
Category: Selectors
Scope: public
In: string, featurename
Out: unsigned int, index of feature
Description: Get the index of a feature
**********************************************************************/
int Explore::GetFeatureNumber(string FeatureName){
  return Population.GetFeatureNumber(FeatureName);
}

/**********************************************************************
Function: GetNoClasses()
Category: Selectors
Scope: public
In: -
Out: unsigned int, the number of original classes
Description: Get the number of classes in the population.
**********************************************************************/
unsigned int Explore::GetNoClasses() {
  return Population.GetNoClasses();
}

/**********************************************************************
Function: GetClassNames()
Category: Selectors
Scope: public
In: -
Out: -
Description: Returns a pointer to a <list> of classnames.
**********************************************************************/
list<string>* Explore::GetClassNames() {
  return Population.GetClassNames();
}

/**********************************************************************
Function: ValidateBestCandidate()
Category: Modifiers
Scope: public
In: -
Out: -
Description: Retrieves the best candidate, sets up the rule and tests
performance of the candidate on the validationpartition.
**********************************************************************/
void Explore::ValidateBestCandidate() {
#ifdef DEBUG_TIMING
  clock_t Start, End;
  Start = clock();
#endif

  if (Initialised) { // TODO: adjust this to train instead of split validation/learn

	if (!Final) {
        Rule.SetTestMode(VALIDATION);
	} else {
        Rule.SetTestMode(LEARN); // TODO: don't need new type train, remove?
	}

	// cout << endl << "BEST RULES (" << PartitionCandidates.size() << " candidates)" << endl << endl;
    cout << endl << "BEST RULES" << endl << endl;

    if(PartitionCandidates.IsValid()) {
        CANDIDATE BestCandidate = PartitionCandidates;

	//  for (unsigned int i=GetMinRuleLength(); i<=GetMaxRuleLength(); i++){
		cout << "RULELENGTH " << BestCandidate.Size() << endl << endl;
		// if (ChooseBestCandidate(i)){
		  if (Rule.SetRule(BestCandidate)) {
			cout << "Best candidate (overall): ";
			Rule.PrintCutoffSet();
			cout << endl;
			cout << "Learn-set: ";
			BestCandidate.Performance.Print();
			cout << endl;

			if (!(GetPartitionMethod()==RESUBSTITUTION)){
			  BestCandidate.Performance = Rule.CalculatePerformance();          // Test BestCandidate on validation partition
			  cout << "Validation-set: ";
			  BestCandidate.Performance.Print();
			  cout << endl;
			}
			ProjectCandidates.push_back(BestCandidate);

		  }
//		} else {
//		  cout << "None." << endl << endl;
//		}
	//  }
      PartitionCandidates.Clear();
	} else {
	  #if defined(EXPLORE_MPI_DEBUG)
	  cout << "--> No Candidates" << endl;
	  #endif
	}
  }

#ifdef DEBUG_TIMING
  End = clock();
  ExploreTiming.AddTime("EXPLORE::TestBestCandidate", Start, End);
#endif
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
//bool Explore::ChooseBestCandidate(unsigned int RuleLength) {
//#ifdef DEBUG_TIMING
//  clock_t Start, End;
//  Start = clock();
//#endif
//  bool Found = false;
//
//  if (Initialised) {
//       tbb::concurrent_vector<CANDIDATE>::iterator CurrentCandidate(PartitionCandidates.begin());
//       tbb::concurrent_vector<CANDIDATE>::iterator LastCandidate(PartitionCandidates.end());
//
//	BestCandidate = (*CurrentCandidate);
//
//    float CurrentValue;
//    float BestValue;
//
//    while (CurrentCandidate != LastCandidate) {
//      CurrentValue = 0;
//      if ((*CurrentCandidate).Features.size()==RuleLength){
//        switch (MaximizeMeasure) {
//          case SENSITIVITY:
//            CurrentValue = (*CurrentCandidate).Performance.Sensitivity.Value;
//            break;
//          case SPECIFICITY:
//            CurrentValue = (*CurrentCandidate).Performance.Specificity.Value;
//            break;
//          case NPV:
//            CurrentValue = (*CurrentCandidate).Performance.NPV.Value;
//            break;
//          case PPV:
//            CurrentValue = (*CurrentCandidate).Performance.PPV.Value;
//            break;
//          case ACCURACY:
//            CurrentValue = (*CurrentCandidate).Performance.Accuracy.Value;
//            break;
//        }
//
//        if (BestValue<=CurrentValue) {
//          BestCandidate = (*CurrentCandidate);
//          BestValue = CurrentValue;
//          Found = true;
//		}
//	  }
//	  CurrentCandidate++;
//    }
//  }
//
//#ifdef DEBUG_TIMING
//  End = clock();
//  ExploreTiming.AddTime("EXPLORE::ChooseBestCandidate", Start, End);
//#endif
//
//  return Found;
//}

/**********************************************************************
Function: SummarisePerformance()
Category: Modifiers
Scope: public
In: -
Out: -
Description: Calculates minimum, maximum and average performance
of all candidates for measure MAXIMIZE_MEASURE.
**********************************************************************/
void Explore::SummarisePerformance() {

  string  PerfStr;
  stringstream Result;
  stringstream ResultShort;
  cout << endl << "PERFORMANCE SUMMARY (" << ProjectCandidates.size() << " best candidates)" << endl << endl;
  float OverallPerformance=0;													//when unsing internal testing
  for (unsigned int i=Rule.GetMinRuleLength(); i<=Rule.GetMaxRuleLength(); i++) {
    vector<CANDIDATE>::iterator CurrentCandidate(ProjectCandidates.begin());
	vector<CANDIDATE>::iterator LastCandidate(ProjectCandidates.end());
	cout << "RULELENGTH " << i << endl;
    float MinimumPerformance=1;
    float MaximumPerformance=0;
	float AveragePerformance=0;
    float MeanDifSum=0;
    float StandardDeviation=0;
    unsigned int NrRules=0;                                                // Number of rules used to average AveragePerformance
    float CurrentValue;

	// Iterate through project candidates (best rules for each rulelengths with test performances)
    int part=0;

    cout << "NR\tSE\tSP\tACC\tPPV\tNPV\tRule\n";
    for (; CurrentCandidate != LastCandidate; CurrentCandidate++) {
       // Rulelength of candidate is equal to current rulelength being examined?
	  if ((*CurrentCandidate).Features.size()==i) {

        part++;
        cout << part << "\t";
        cout << (*CurrentCandidate).PrintPerformance(false) << "\t";
        cout << (*CurrentCandidate).PrintRule() << endl;

        switch (MaximizeMeasure) {
          case SENSITIVITY:
            CurrentValue = (*CurrentCandidate).Performance.Sensitivity.Value;
            break;
          case SPECIFICITY:
			CurrentValue = (*CurrentCandidate).Performance.Specificity.Value;
            break;
          case NPV:
            CurrentValue = (*CurrentCandidate).Performance.NPV.Value;
            break;
          case PPV:
			CurrentValue = (*CurrentCandidate).Performance.PPV.Value;
            break;
          case ACCURACY:
            CurrentValue = (*CurrentCandidate).Performance.Accuracy.Value;
            break;
            case BALANCEDACCURACY:
                CurrentValue = (*CurrentCandidate).Performance.BalancedAccuracy.Value;
                break;
            case F1SCORE:
                CurrentValue = (*CurrentCandidate).Performance.F1score.Value;
                break;
        }
		// Calculate minimum performance
        if (CurrentValue<MinimumPerformance) {
          MinimumPerformance = CurrentValue;
        }
        // Calculate maximum performance
        if (CurrentValue>MaximumPerformance) {
		  MaximumPerformance = CurrentValue;
        }
        // Add performance to average
        AveragePerformance += CurrentValue;
		OverallPerformance += CurrentValue;
		NrRules++;
	  }
	}
	cout << endl;
	// Average project performances for best rules
	if (AveragePerformance>0 && NrRules>0) {
	  AveragePerformance /= (float)NrRules;
	}

    // Calculate Standard deviation
    CurrentCandidate = ProjectCandidates.begin();
    for (; CurrentCandidate != LastCandidate; CurrentCandidate++) {
      // Rulelength of candidate is equal to current rulelength being examined?
	  if ((*CurrentCandidate).Features.size()==i) {
        switch (MaximizeMeasure) {
          case SENSITIVITY:
            CurrentValue = (*CurrentCandidate).Performance.Sensitivity.Value;
            break;
          case SPECIFICITY:
			CurrentValue = (*CurrentCandidate).Performance.Specificity.Value;
            break;
          case NPV:
            CurrentValue = (*CurrentCandidate).Performance.NPV.Value;
            break;
          case PPV:
			CurrentValue = (*CurrentCandidate).Performance.PPV.Value;
            break;
          case ACCURACY:
            CurrentValue = (*CurrentCandidate).Performance.Accuracy.Value;
            break;
            case BALANCEDACCURACY:
                CurrentValue = (*CurrentCandidate).Performance.BalancedAccuracy.Value;
                break;
            case F1SCORE:
                CurrentValue = (*CurrentCandidate).Performance.F1score.Value;
                break;
        }
		// Sum sqr difference with mean
		MeanDifSum += pow((AveragePerformance-CurrentValue),2);
      }
	}
	if (NrRules>1) {
	  StandardDeviation=sqrt(MeanDifSum/= (float) (NrRules-1));
	}
	else  StandardDeviation=0;


	Result << (float)(100*MinimumPerformance) << "\t" <<
			(float)(100*MaximumPerformance) << "\t" <<
			(float)(100*AveragePerformance) << "\t" <<
			(float)(100*StandardDeviation)  << "\t";

	ResultShort << (float)(100*AveragePerformance) << "\t";
  }
  // Display summary
  for (unsigned int i=Rule.GetMinRuleLength(); i<=Rule.GetMaxRuleLength(); i++) {
	cout << "Min" << i << "\tMax" << i << "\tAve" << i << "\tStd" << i << "\t";
  }
  cout << endl;
  cout << Result.str();
  cout << endl << endl;

  for (unsigned int i=Rule.GetMinRuleLength(); i<=Rule.GetMaxRuleLength(); i++) {
	cout << "\tAve" << i ;
  }
  cout << endl;
  OverallPerformance /= (float)ProjectCandidates.size();
  cout << "Short: " << ResultShort.str() << endl;
  cout << "Overall: " << 100*OverallPerformance << endl;
}

/**********************************************************************
Function: PrintBestCandidate()
Category: Modifiers
Scope: public
In: -
Out: -
Description:
**********************************************************************/
void Explore::PrintBestCandidate() {
  if (Initialised) {
    Rule.PrintCutoffSet();
  }
}

/**********************************************************************
Function: GetBestCandidate()
Category: Modifiers
Scope: public
In: -
Out: -
Description:
**********************************************************************/
CANDIDATE Explore::GetBestCandidate() {
  if (Initialised) {
    NewCandidate = false;
    return BestCandidate;
  }
  CANDIDATE Dummy;
  return Dummy;
}

/**********************************************************************
Function: GetProjectCandidate()
Category: Selectors
Scope: public
In: unsigned int, order of the candidate
Out: -
Description: Returns a candidate from the list of project-candidates.
**********************************************************************/
CANDIDATE Explore::GetProjectCandidate(unsigned int COrder) {
  if (COrder<GetNoProjectCandidates()) {
    return ProjectCandidates.at(COrder);
  }
  CANDIDATE Dummy;
  return Dummy;
}

/**********************************************************************
Function: GetPartitionCandidate()
Category: Selectors
Scope: public
In: unsigned int, order of the candidate
Out: -
Description: Returns a candidate from the list of partition-candidates.
**********************************************************************/
//CANDIDATE Explore::GetPartitionCandidate(unsigned int COrder) {
//  if (COrder<GetNoPartitionCandidates()) {
//    return PartitionCandidates.at(COrder);
//  }
//  CANDIDATE Dummy;
//  return Dummy;
//}

/**********************************************************************
Function: GetNoProjectCandidates()
Category: Selectors
Scope: public
In: -
Out: unsigned int, the number of rules
Description: Get the number of rules which outperform the prerequisites.
**********************************************************************/
unsigned int Explore::GetNoProjectCandidates() {
  if (Initialised) {
    return ProjectCandidates.size();
  }
  return 0;
}

/**********************************************************************
Function: GetNoPartitionCandidates()
Category: Selectors
Scope: public
In: -
Out: unsigned int, the number of rules
Description: Get the number of rules which outperform the prerequisites.
**********************************************************************/
unsigned int Explore::GetNoPartitionCandidates() {
  if (Initialised) {
   // return PartitionCandidates.size();
      return 1;
  }
  return 0;
}

/**********************************************************************
Function: GetCurrentRuleLength()
Category: Selectors
Scope: public
In: -
Out: unsigned int, the current length
Description: Get the current length of the rule (number of features).
**********************************************************************/
unsigned int Explore::GetCurrentRuleLength() {
  return Rule.GetCurrentRuleLength();
}

/**********************************************************************
Function: GetMaxRuleLength()
Category: Selectors
Scope: public
In: -
Out: unsigned int, the max length
Description: Get the max length of the rule (number of features).
**********************************************************************/
unsigned int Explore::GetMaxRuleLength() {
  return Rule.GetMaxRuleLength();
}

/**********************************************************************
Function: GetMinRuleLength()
Category: Selectors
Scope: public
In: -
Out: unsigned int, the min length
Description: Get the min length of the rule (number of features).
**********************************************************************/
unsigned int Explore::GetMinRuleLength() {
  return Rule.GetMinRuleLength();
}

/**********************************************************************
Function: GetNoConjunctions()
Category: Selectors
Scope: public
In: -
Out: unsigned int, the number of subsets
Description: Get the number of subsets in the rule.
**********************************************************************/
unsigned int Explore::GetNoConjunctions() {
  return Rule.Size();
}

/**********************************************************************
Function: GetNoFeatures()
Category: Selectors
Scope: public
In: -
Out: unsigned int, the number of features
Description: Get the number of features in the population.
**********************************************************************/
unsigned int Explore::GetNoFeatures() {
  return Population.GetNoFeatures();
}

/**********************************************************************
Function: PrintSettings()
Category: Selectors
Scope: public
In: -
Out: -
Description: Print summary information of the current rule and
population.
**********************************************************************/
void Explore::PrintSettings() {
  unsigned int Selected;

  cout << "SETTINGS" << endl;

  cout << endl << "PROJECT" << endl << endl;
  cout << "Name: ";  
  if (strlen(Name.c_str())>0) {
    cout << Name << endl;
  } else {
    cout << "-" << endl;
  }
  cout << "Remarks: ";
  if (strlen(Description.c_str())>0) {
    cout << Description << endl;
  } else {
    cout << "-" << endl;
  }

  cout << "Branch and Bound optimization: ";
  if (BranchBound){
    cout << "Yes" << endl;
  }
  else {
    cout << "No" << endl;
  }

  cout << "Subsumption Pruning: ";
  if (SubSumption){
    cout << "Yes" << endl;
  }
  else {
    cout << "No" << endl;
  }
    cout << "Parallel: ";
    if (Parallel){
        cout << "Yes" << endl;
    }
    else {
        cout << "No" << endl;
    }


  Population.PrintSettings();

  Rule.PrintSettings();

  cout << endl << "FEATURE SETTINGS" << endl << endl;
  if (CutoffMethodSet) {
    cout << "Cutoff method: ";
    switch (CutoffMethod) {
      case USER:
        cout << "Manual";
        break;
      case ALL:
        cout << "Exhaustive";
        break;
      case RVAC:
        cout << "Referent Value Analysis";
        break;
    }
  }
  cout << endl;

  if (OperatorMethodSet) {
    cout << "Operator method: ";
    switch (OperatorMethod) {
      case MANUAL:
        cout << "Manual";
        break;
      case EXHAUSTIVE:
        cout << "Exhaustive";
        break;
      case MEDIAN:
        cout << "Median";
        break;
      case ROCAREA:
        cout << "Area under ROC";
        break;
      case ROCDIAGONAL:
        cout << "ROC Diagonal";
        break;
    }
    cout << endl;
  }
}

/**********************************************************************
Function: PrintCutoffs()
Category: Selectors
Scope: public
In: -
Out: -
Description: Print the current cutoffs found in the current learning
partition.
**********************************************************************/
void Explore::PrintCutoffs() {
  Population.PrintCutoffs();
}

/**********************************************************************
Function: PrintOperators()
Category: Selectors
Scope: public
In: -
Out: -
Description: Print the current operators found in the current learning
partition.
**********************************************************************/
void Explore::PrintOperators() {
  Population.PrintOperators();
}

/**********************************************************************
Function: PrintPartitions()
Category: Selectors
Scope: public
In: -
Out: -
Description: Print the current partitions of the population.
**********************************************************************/
void Explore::PrintPartitions() {
  Population.PrintPartitions();
}

/**********************************************************************
Function: PrintRulePerformance()
Category: Selectors
Scope: public
In: -
Out: -
Description: Print the total performance of a rule (after testing
all partitions).
**********************************************************************/
void Explore::PrintPerformance() {
#ifdef DEBUG_TIMING
  clock_t Start, End;
  Start = clock();
#endif

  if (Initialised) {
    CurrentPerformance.Print();
  }
  
#ifdef DEBUG_TIMING
  End = clock();
  ExploreTiming.AddTime("EXPLORE::PrintPerformance", Start, End);
#endif
}

/**********************************************************************
Function: PrintConstraints()
Category: Selectors
Scope: public
In: -
Out: -
Description: Print all constraints and their values.
**********************************************************************/
void Explore::PrintConstraints() {
  vector<CONSTRAINT>::iterator CurrentConstraint(Constraints.begin());
  vector<CONSTRAINT>::iterator LastConstraint(Constraints.end());

  cout << endl << "CONSTRAINTS" << endl << endl;
  cout << "Optimize: ";
  switch (MaximizeMeasure) {
    case SENSITIVITY:
      cout << "Sensitivity";
      break;
    case SPECIFICITY:
      cout << "Specificity";
      break;
    case NPV:
      cout << "Negative Predictive Value";
      break;
    case PPV:
      cout << "Positive Predictive Value";
      break;
    case ACCURACY:
      cout << "Accuracy";
      break;
      case BALANCEDACCURACY:
          cout << "Balanced Accuracy";
          break;
      case F1SCORE:
          cout << "F1 score";
          break;
  }
  cout << endl;

  for (; CurrentConstraint != LastConstraint; CurrentConstraint++) {
    switch ((*CurrentConstraint).Measure) {
      case SENSITIVITY:
        cout << "Sensitivity >= ";
        break;
      case SPECIFICITY:
        cout << "Specificity >= ";
        break;
      case NPV:
        cout << "Negative Predictive Value >= ";
        break;
      case PPV:
        cout << "Positive Predictive Value >= ";
        break;
      case ACCURACY:
        cout << "Accuracy >= ";
        break;
        case BALANCEDACCURACY:
            cout << "Balanced Accuracy >= ";
            break;
        case F1SCORE:
            cout << "F1 score >= ";
            break;
    }
    cout << (*CurrentConstraint).Value << endl;
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
void Explore::PrintFeatureOperators() {
  Rule.PrintFeatureOperators();
}

/**********************************************************************
Function: PrintSets
Category: Selectors
Scope: public
In: -
Out: -
Description: Print debug information about rule performance.
**********************************************************************/
void Explore::PrintSets() {
  if (Initialised) {
    Rule.PrintSets();
  }
}

/**********************************************************************
Function: GetRuleCombination()
Category: Selectors
Scope: public
In: -
Out: bool
Description: Print the current combination of the rule.
**********************************************************************/
void Explore::PrintCombination() {
  if (Initialised) {
     Rule.PrintCombination();
  }
}

/**********************************************************************
Function: PrintFeatu()
Category: Selectors
Scope: public
In: -
Out: -
Description: Print all current featuresets of the rule.
**********************************************************************/
void Explore::PrintFeatureSet() {
  if (Initialised) {
    Rule.PrintFeatureSet();
  }
}

/**********************************************************************
Function: PrintCutoffSet()
Category: Selectors
Scope: public
In: -
Out: -
Description: Print all current conditionsets of the rule.
**********************************************************************/
void Explore::PrintCutoffSet() {
  if (Initialised) {
    Rule.PrintCutoffSet();
  }
}

/**********************************************************************
Function: GetCombinationsGenerated()
Category: Selectors
Scope: public
In: -
Out: unsigned long, number of combinations
Description: Returns the number of combinations that have been
generated until now.
**********************************************************************/
unsigned int Explore::GetCombinationsGenerated() {
  return Rule.GetCombinationsGenerated();
}

/**********************************************************************
Function: GetFeatureSetsGenerated()
Category: Selectors
Scope: public
In: -
Out: unsigned long, number of featuresets
Description: Returns the number of featuresets that have been
generated until now.
**********************************************************************/
long Explore::GetFeatureSetsGenerated() {
  return Rule.GetFeatureSetsGenerated();
}

/**********************************************************************
Function: GetCutoffSetsGenerated()
Category: Selectors
Scope: public
In: -
Out: unsigned long, number of conditionSets
Description: Return the number of conditionsets that have been
generated until now.
**********************************************************************/
long Explore::GetCutoffSetsGenerated() {
  return Rule.GetCutoffSetsGenerated();
}

/**********************************************************************
Function: SetRestriction()
Category: Modifiers
Scope: public
In: string, a rule
Out: bool, is the rule valid
Description: Interpret and set the rule from a String representing a
rule.
**********************************************************************/
bool Explore::SetRestriction(string StringRule) {
  if (!Initialised) {
    if (Rule.SetRestriction(StringRule)) {
      RestrictionString = StringRule;
      RestrictionSet = true;
      return true;
    }
  }
  return false;
}

/**********************************************************************
Function: RemoveRestriction()
Category: Modifiers
Scope: public
In: -
Out: -
Description: Clear any existing rule-restriction.
**********************************************************************/
void Explore::RemoveRestriction() {
  if (!Initialised) {
    Rule.RemoveRestriction();
  }
}

/**********************************************************************
Function: ClearCandidates()
Category: Modifiers
Scope: public
In: -
Out: -
Description: Clears all rules and their corresponding performances
that have been saved previously.
**********************************************************************/
void Explore::ClearCandidates() {
  // PartitionCandidates.clear();
    PartitionCandidates.Clear();
}

/**********************************************************************
Function: ClearConstraints()
Category: Modifiers
Scope: public
In: -
Out: -
Description: Clears all constraints.
**********************************************************************/
void Explore::ClearConstraints() {
  Constraints.clear();
}

/**********************************************************************
Function: ClearPopulation()
Category: Modifiers
Scope: public
In: -
Out: -
Description: Clears the population.
**********************************************************************/
void Explore::ClearPopulation() {
  Population.Clear();
}

/**********************************************************************
Function: SetName()
Category: Modifiers
Scope: public
In: -
Out: -
Description: Sets Explore project name.
**********************************************************************/
void Explore::SetName(string ProjectName) {
  if (ProjectName.size()>0) {
    Name = ProjectName;
  }
}

/**********************************************************************
Function: SetDescription()
Category: Modifiers
Scope: public
In: -
Out: -
Description: Sets Explore project description.
**********************************************************************/
void Explore::SetDescription(string ProjectDescription) {
  if (ProjectDescription.size()>0) {
    Description = ProjectDescription;
  }
}


/**********************************************************************
Function: CheckSettings()
Category: Modifiers
Scope: public
In: -
Out: bool, settings are/are not correct
Description: Check explore settings.
**********************************************************************/
bool Explore::CheckSettings() {

  if (PartitionMethodSet && CutoffMethodSet && OperatorMethodSet && PositiveClassSet && MaximizeMeasureSet) {

    if (PartitionMethod==HOLDOUT && !LearnTestRatioSet) return false;           // If HOLDOUT, LEARNRATIO set?
    if (PartitionMethod==CROSS_VALIDATION && !NoPartitionsSet) return false;
                                                                                // Number of partitions set?

    if (Rule.GetMinRuleLength() <= 0 || Rule.GetMinRuleLength() > Rule.GetMaxRuleLength()) {
      return false;                                                             // Are minimum and maximum rulelength set?
    }

    if (CutoffMethod==USER) {                                                   // At least 1 cutoff for every feature?
      unsigned int NoFeatures = Population.GetNoFeatures();
      for (unsigned int i=0; i < NoFeatures; i++) {
        if (Population.GetNoCutoffs(i)==0) {
          return false;
        }
      }
    }

    if (OperatorMethod==MANUAL) {                                               // Operator set for each feature?
      unsigned int NoFeatures = Population.GetNoFeatures();
      for (unsigned int i=0; i < NoFeatures; i++) {
        if (Population.GetNoOperators(i)==0) {
          return false;
        }
      }
    }

    if (Population.GetNoObservations() <= MINIMUM_OBSERVATIONS) {               // At least MINIMUM_OBSERVATIONS?
      return false;
    }

    unsigned int NoFeatures = Population.GetNoFeatures();
    if (NoFeatures <= MINIMUM_FEATURES) {                                       // At least MINIMUM_FEATURES?
      return false;
    }

    // Number of classes?
    // Output method?
	// Progress method?
    // Close method?

    return true;
  }
  return false;
}

/**********************************************************************
Function: Partition()
Category: Modifiers
Scope: public
In: -
Out: -
Description: Partition the population of explore.
**********************************************************************/
bool Explore::Partition() {
   // ValidateBestCandidate();                                                          // Do not remove! Is needed for summarising best candidates at the end of projects (ie. HOLDOUT)

  if (Population.Partition()) {                                                 // Will return false with holdout on second call!
    SetRerun();                                                                 // Reset rule (findcutoffs etc)
	NoPartitionsDone++;
    return true;
  } else {
	SetRerun();                                                                 // Prepare for new Run
    NoPartitionsDone++;                                                         // Increment number of partitions covered
	//if (PartitionMethod == CROSS_VALIDATION) {                                  // In case of cross-validation
	//   SummarisePerformance();                                                   // Summarise performances of candidates that were found
	// }
  }
  return false;
}

/**********************************************************************
Function: ClearPartitions()
Category: Modifiers
Scope: public
In: -
Out: -
Description: Clear all partitions of the population (without removing
the actual features or their observations).
**********************************************************************/
void Explore::ClearPartitions() {
  Population.ClearPartitions();
}

/**********************************************************************
Function: Initialise()
Category: Modifiers
Scope: public
In: -
Out: -
Description: Initialise the rule of explore so we can start
generating rules.
**********************************************************************/
bool Explore::Initialise() {
#ifdef DEBUG_TIMING
  clock_t Start, End;
  Start = clock();
#endif

  cout.precision(4);
  cout.width(4);
   if (CheckSettings()) {

    Initialised = true;                                                         // Explore is now initialised

	ProjectCandidates.clear();                                                  // Clear any candidates
	RuleLengthTiming.Clear();                                                   // Clear any timing info

    Rule.Initialise(Population.GetFeatures());                                  // Initialise the rule

    Rule.SetTestMode(LEARN);

	if (SubSumption){
	   Rule.Prune();                                                            // Remove unnecessary cutoffs by SubSumption
    }

    NoPartitionsDone = 0;
	// PR ExploreComplexity = RuleComplexity();                                  // Calculate complexity for progress indication
	if (PartitionMethod == CROSS_VALIDATION) {
		ExploreComplexity = Population.GetNoPartitions()*10; 							// make progressbar count the folds
	}
	InitialiseCPFP();

	// Print project settings
    if (IsPrintSettings) {
      PrintSettings();
    }
    if (IsPrintPerformance) {
      PrintConstraints();
    }

    if (IsPrintFeatureOperators) {                                              // Print FeatureOperators
      Rule.PrintFeatureOperators();
    }
  }

#ifdef DEBUG_TIMING
  End = clock();
  ExploreTiming.AddTime("EXPLORE::Initialise", Start, End);
#endif
  
  return Initialised;
}

/**********************************************************************
Function: LimitedInitialise()
Category: Modifiers
Scope: public
In: -
Out: -
Description: Initialise the rule of explore so we can start
generating rules without setting features (already set) or rebuilding
vector of FeatureOperatorSets.
**********************************************************************/
bool Explore::LimitedInitialise() {
#ifdef DEBUG_TIMING
  clock_t Start, End;
  Start = clock();
#endif

  cout.precision(4);
  cout.width(4);

  if (CheckSettings()) {

    Initialised = true;                                                         // Explore is now initialised

    ProjectCandidates.clear();                                                  // Clear any candidates

    Rule.LimitedInitialise();                                                   // Initialise the rule

    Rule.SetTestMode(LEARN);

    NoPartitionsDone = 0;
    RulesProcessed = 0;
    FeatureSetsProcessed = 0;    
    //PR ExploreComplexity = RuleComplexity();                                   // Calculate complexity for progress indication
	ExploreComplexity = Population.GetNoPartitions()*10;
    InitialiseCPFP();

    // Print any settings if needed
	if (IsPrintSettings) PrintSettings();
    if (IsPrintPerformance) PrintConstraints();
    if (IsPrintFeatureOperators) Rule.PrintFeatureOperators();
  }

#ifdef DEBUG_TIMING
  End = clock();
  ExploreTiming.AddTime("EXPLORE::Initialise", Start, End);
#endif
  
  return Initialised;
}

/**********************************************************************
Function: InitialiseCPFP()
Category: Modifiers
Scope: public
In: -
Out: -
Description: Initialise the rule of explore for the CPFP method
**********************************************************************/
void Explore::InitialiseCPFP() {

  // Rule optimization
  if ((MaximizeMeasure==SENSITIVITY || MaximizeMeasure==ACCURACY) && !RestrictionSet && BranchBound) {
    Rule.SortCutoffsByCP();
    Rule.FPCPOptimization = true;
  } else {
    Rule.FPCPOptimization = false;
  }

  // Set FPConstraint, CPBest, CTBest
  vector<CONSTRAINT>::iterator CurrentConstraint(Constraints.begin());
  vector<CONSTRAINT>::iterator LastConstraint(Constraints.end());

  Rule.SetFPConstraint(0);
  Rule.SetCPBest(0);
  Rule.SetCTBest(0);

  for (; CurrentConstraint != LastConstraint; CurrentConstraint++) {
    if ((*CurrentConstraint).Measure==SPECIFICITY) {
      Rule.SetFPConstraint((*CurrentConstraint).Value);
    }
    if ((*CurrentConstraint).Measure==SENSITIVITY) {
      Rule.SetCPBest((*CurrentConstraint).Value);
    }
    if ((*CurrentConstraint).Measure==ACCURACY) {
      Rule.SetCTBest((*CurrentConstraint).Value);
    }
  }
}

/**********************************************************************
Function: Uninitialise()
Category: Modifiers
Scope: public
In: -
Out: -
Description: Uninitialise the rule. Make settings modifiable.
**********************************************************************/
void Explore::Uninitialise() {
  Initialised = false;

  ClearCandidates();
  BestCandidate.Clear();
  Rule.Reset();

  PreviousProgressCount = 0;
  CurrentProgressCount = 0;

  NoPartitionsDone = 0;
}



/**********************************************************************
Function: SetMaxRuleLength()
Category: Modifiers
Scope: public
In: const unsigned int, rule length
Out: bool, could/could not set maximum rule length
Description: Set the maximum rule length. Used as a boundary for
generating rules.
**********************************************************************/
bool Explore::SetMaxRuleLength(const unsigned int ERuleLength) {
  if (ERuleLength>0) {
    Rule.SetMaxLength(ERuleLength);
    return true;
  }
  return false;
}

/**********************************************************************
Function: SetMinRuleLength()
Category: Modifiers
Scope: public
In: const unsigned int, rule length
In: const unsigned int, rule length
Out: bool, could/could not set maximum rule length
Description: Set the minimum rule length. Used as a boundary for
generating rules.
**********************************************************************/
bool Explore::SetMinRuleLength(const unsigned int ERuleLength) {
  if (ERuleLength>0) {
	Rule.SetMinLength(ERuleLength);
    return true;
  }
  return false;
}

/**********************************************************************
Function: SetRandomize()
Category: Modifiers
Scope: public
In: bool, randomize yes/no
Out: bool, could/could not set randomization
Description: Set the option of randomizing the partitions or not.
**********************************************************************/
bool Explore::SetRandomize(bool Random) {
  if (Random) {
    Randomize = true;
  } else {
    Randomize = false;
  }
  Population.SetRandomize(&Randomize);
  return true;
}

/**********************************************************************
Function: GetSeed()
Category: Selectors
Scope: public
In: -
Out: long double, the seed which is a cast from time_t 
Description: Get the seed used to randomize the population.
**********************************************************************/
long double Explore::GetSeed() {
  return Population.GetSeed();
}

/**********************************************************************
Function: SetSeed()
Category: Modifiers
Scope: public
In: long double, the seed which is a cast from time_t
Out: bool, setting the seed succeeded/did not succeed
Description: Set the seed used to randomize the population.
**********************************************************************/
bool Explore::SetSeed(long double Seed) {
  if (!Initialised) {
    Population.SetSeed(Seed);
    return true;
  }
  return false;
}

/**********************************************************************
Function: ReSetSeed()
Category: Modifiers
Scope: public
In:
Out:
Description: Resets the SeedIsSet boolean in population
**********************************************************************/
void Explore::ResetSeed() {
	Population.ResetSeed();
}

/**********************************************************************
Function: SetMandatoryFeature()
Category: Modifiers
Scope: public
In: unsigned int, feature number
    bool, mandatory/or not
Out: -
Description: Make a feature mandatory/or not.
**********************************************************************/
void Explore::SetMandatoryFeature(unsigned int FeatureNumber, bool IsMandatory) {
  if (FeatureNumber <= Population.GetNoFeatures()) {
    Population.SetMandatoryFeature(FeatureNumber,IsMandatory);
  }
}

/**********************************************************************
Function: GetMandatoryFeature()
Category: Modifiers
Scope: public
In: unsigned int, feature number
    bool, mandatory/or not
Out: -
Description: Make a feature mandatory/or not.
**********************************************************************/
bool Explore::GetMandatoryFeature(unsigned int FeatureNumber) {
  if (FeatureNumber <= Population.GetNoFeatures()) {
    return Population.GetMandatoryFeature(FeatureNumber);
  }
  return false;
}

/**********************************************************************
Function: SetRuleOutputMethod()
Category: Modifiers
Scope: public
In: RULE_SAVE_METHOD, enum containing the types
Out: bool, could/could not set rule save method
Description: Set the way explore has to deal with new good
performing rules.
**********************************************************************/
bool Explore::SetRuleOutputMethod(RULE_OUTPUT_METHOD ROutputMethod) {
  RuleOutputMethod = ROutputMethod;
  RuleOutputMethodSet = true;
  return true;
}

/**********************************************************************
Function: SetPartitionMethod()
Category: Modifiers
Scope: public
In: const PARTITION_METHOD, partitioning method
Out: bool, could/could not set partitioning method
Description: Set the partition method used to partition the population.
**********************************************************************/
bool Explore::SetPartitionMethod(const PARTITION_METHOD PMethod) {
  PartitionMethod = PMethod;
  Population.SetPartitionMethod(&PartitionMethod);
  PartitionMethodSet = true;
  return true;
}

/**********************************************************************
Function: SetCutoffMethod()
Category: Modifiers
Scope: public
In: const CUTOFF_METHOD, cutoff method
Out: bool, could/could not set cutoff method
Description: Set the method used for finding cutoff values.
**********************************************************************/
bool Explore::SetCutoffMethod(const CUTOFF_METHOD CMethod) {
  CutoffMethod = CMethod;
  Population.SetCutoffMethod(&CutoffMethod);
  CutoffMethodSet = true;
  return true;
}

/**********************************************************************
Function: SetOperatorMethod()
Category: Modifiers
Scope: public
In: const OPERATOR_METHOD, operator method
Out: bool, could/could not set operator method
Description: Set the method used for finding operators.
**********************************************************************/
bool Explore::SetOperatorMethod(const OPERATOR_METHOD OMethod) {
  OperatorMethod = OMethod;
  Population.SetOperatorMethod(&OperatorMethod);
  OperatorMethodSet = true;
  return true;
}

/**********************************************************************
Function: SetLearnRatio()
Category: Modifiers
Scope: public
In: const float, ratio of population to be used for learning
Out: bool, could/could not set learn ratio
Description: Set the ratio used to divide a population into a Learn and
Test partition for Holdout partition method.
**********************************************************************/
bool Explore::SetLearnRatio(const float PLearnRatio) {
  if (PLearnRatio>0 && PLearnRatio<1) {
    LearnRatio = PLearnRatio;
    Population.SetLearnRatio(&LearnRatio);
    LearnTestRatioSet = true;
    return true;
  }
  return false;
}

/**********************************************************************
Function: SetNoPartitions()
Category: Modifiers
Scope: public
In: const unsigned int, number of partitions
Out: bool, could/could not set number of partitions
Description: Set the number of partitions the population has to be divided into
for Holdout Random-Subsampling  and Cross Validation partition methods.
**********************************************************************/
bool Explore::SetNoPartitions(const unsigned int PNoPartitions) {
  if (PNoPartitions>1) {
    NoPartitions = PNoPartitions;
    Population.SetNoPartitions(&NoPartitions);
    NoPartitionsSet = true;
    return true;
  }
  return false;
}

/**********************************************************************
Function: SetPositiveClass()
Category: Modifiers
Scope: public
In: unsigned int, positive class number
Out: bool, could/could not set class
Description: Set the class for which a classifier has to be generated by index.
**********************************************************************/
bool Explore::SetPositiveClass(unsigned int Feature, unsigned int Class) {
  PositiveClass = Class;
  Population.SetPositiveClass(&Feature,&PositiveClass);
  PositiveClassSet = true;
  return true;
}


/**********************************************************************
Function: SetClassNames()
Category: Modifiers
Scope: public
In: list<string>, the class-names
Out: -
Description: Set the classnames for a specific population.
**********************************************************************/
void Explore::SetClassNames(list<string> NClassNames) {
  Population.SetClassNames(NClassNames);
}

/**********************************************************************
Function: SetClassFName()
Category: Modifiers
Scope: public
In: string, the featurename
Out: -
Description: Set the classnames for a specific population based on a feature
**********************************************************************/
void Explore::SetClassNamesByFName(string ClassFName) {
  Population.SetClassNamesByFName(ClassFName);
}


/**********************************************************************
Function: SetConstraint()
Category: Modifiers
Scope: public
In: PERFORMANCE_MEASURE, the variable used for comparing performances
    float, the value of the constraint
Out: bool, could/could not set constraint
Description: Set the prerequisite constraint variable.
**********************************************************************/
bool Explore::SetConstraint(PERFORMANCE_MEASURE PMeasure, float PValue) {
  if ((PValue > 0) && (PValue <=1)) {
    vector<CONSTRAINT>::iterator CurrentConstraint(Constraints.begin());
    vector<CONSTRAINT>::iterator LastConstraint(Constraints.end());

    for (CurrentConstraint = Constraints.begin(); CurrentConstraint != LastConstraint ; CurrentConstraint++){
      if ((*CurrentConstraint).Measure == PMeasure) {
        Constraints.erase(CurrentConstraint);
      }
    }

    CONSTRAINT Dummy;
    Dummy.Measure = PMeasure;
    Dummy.Value = PValue;
    Constraints.push_back(Dummy);

    return true;
  }
  return false;
}

/**********************************************************************
Function: SetMaximizeMeasure()
Category: Modifiers
Scope: public
In: PERFORMANCE_MEASURE, the variable used for comparing performances
Out: bool, could/could not set performance maximizing measure
Description: Set the measure that has to be maximized.
**********************************************************************/
bool Explore::SetMaximizeMeasure(PERFORMANCE_MEASURE PMeasure) {
  MaximizeMeasure = PMeasure;
  MaximizeMeasureSet = true;

  Rule.SetMaximizeMeasure(&MaximizeMeasure);

  return false;
}

/**********************************************************************
Function: AddCutoff()
Category: Modifiers
Scope: public
In: unsigned int, feature number
    float, cutoff value
Out: bool, could/could not add cutoff manually
Description: Manually add a cutoff to a feature.
**********************************************************************/
bool Explore::AddCutoff(unsigned int FeatureNumber, string CutoffValue) {        // Population should be initialised
  if (FeatureNumber < GetNoFeatures() && CutoffMethod==USER) {                  // Make sure feature exists and cutoff method is set to user
    Population.AddCutoff(FeatureNumber, CutoffValue);                           // Add the cutoff
    return true;
  }

  return false;
}

/**********************************************************************
Function: RemoveCutoff()
Category: Modifiers
Scope: public
In: unsigned int, feature number
    float, cutoff value
Out: bool, could/could not remove cutoff manually
Description: Manually remove a cutoff from a feature (by number).
**********************************************************************/
bool Explore::RemoveCutoff(unsigned int FeatureNumber, unsigned int CutoffNumber) {
  if (FeatureNumber < GetNoFeatures() && CutoffMethod==USER) {                // Make sure feature exists and cutoff method is set to user
    Population.RemoveCutoff(FeatureNumber, CutoffNumber);                     // Remove the cutoff
    return true;
  }
  return false;
}

/**********************************************************************
Function: RemoveCutoff()
Category: Modifiers
Scope: public
In: unsigned int, feature number
    float, cutoff value
Out: bool, could/could not remove cutoff manually
Description: Manually remove a cutoff from a feature (by value).
**********************************************************************/
bool Explore::RemoveCutoff(unsigned int FeatureNumber, string CutoffValue) {
  if (FeatureNumber < GetNoFeatures() && CutoffMethod==USER) {                // Make sure feature exists and cutoff method is set to user
    Population.RemoveCutoff(FeatureNumber, CutoffValue);                      // Remove the cutoff
    return true;
  }
  
  return false;
}

/**********************************************************************
Function: RemoveCutoffRange()
Category: Modifiers
Scope: public
In: unsigned int, feature number
    float, cutoff value
Out: bool, could/could not remove cutoff manually
Description: Manually remove a cutoff from a feature (by value).
**********************************************************************/
bool Explore::RemoveCutoffRange(unsigned int FeatureNumber) {                   // Population should not be initialised
 if (FeatureNumber < GetNoFeatures()) {                                         // Make sure feature exists and cutoff method is set to user
   Population.RemoveCutoffRange(FeatureNumber);                                 // Remove the cutoff
   return true;
 }

 return false;
}

/**********************************************************************
Function: RemoveFeatureCutoffs()
Category: Modifiers
Scope: public
In: 
Out: bool, could/could not remove cutoffs for selected feature
Description: Remove all cutoffs belonging to a specific feature.
**********************************************************************/
bool Explore::RemoveFeatureCutoffs(unsigned int FeatureNumber) {
  if (FeatureNumber < GetNoFeatures()) {
    Population.RemoveFeatureCutoffs(FeatureNumber);
    return true;
  }
  
  return false;
}

/**********************************************************************
Function: RemoveAllCutoffs()
Category: Modifiers
Scope: public
In:
Out: bool, could/could not remove cutoffs for selected feature
Description: Remove all cutoffs belonging to all features.
**********************************************************************/
void Explore::RemoveAllCutoffs() {
  for (unsigned int i=0; i < GetNoFeatures(); i++) {
    Population.RemoveFeatureCutoffs(i);
  }
}

/**********************************************************************
Function: SetOperator()
Category: Modifiers
Scope: public
In: unsigned int, feature number
    OPERATOR, feature operator
Out: bool, could/could not set operator manually
Description: Manually set an operator for a feature.
**********************************************************************/
bool Explore::SetOperator(unsigned int FeatureNumber, OPERATOR FeatureOperator) {
 if (FeatureNumber < GetNoFeatures()) {                                         // Make sure feature exists
   Population.SetOperator(FeatureNumber, FeatureOperator);                      // Set the operator
   return true;
 }

 return false;
}

/**********************************************************************
Function: AddCutoffRange()
Category: Modifiers
Scope: public
In: unsigned int, feature number
    Range, cutoff-range
Out: -
Description: Manually set a cutoff-range for a specific feature.
**********************************************************************/
void Explore::AddCutoffRange(unsigned int FeatureNumber, Range NewRange) {
  if (FeatureNumber < GetNoFeatures()) {
    Population.AddCutoffRange(FeatureNumber,NewRange);
  }
}

/**********************************************************************
Function: GetCutoffRange()
Category: Modifiers
Scope: public
In: unsigned int, feature number
Out: Range, cutoff-range
Description: get a cutoff-range for a specific feature.
**********************************************************************/
vector<Range>* Explore::GetCutoffRanges(unsigned int FeatureNumber) {
  return Population.GetCutoffRanges(FeatureNumber);
}

/**********************************************************************
Function: GetOperator()
Category: Modifiers
Scope: public
In: unsigned int, feature number
Out: OPERATOR, operator associated with particular feature
Description: Returns the operator set for a particular feature.
**********************************************************************/
OPERATOR Explore::GetOperator(unsigned int FeatureNumber) {
  return Population.GetOperator(FeatureNumber);
}

/**********************************************************************
Function: IsContinuous()
Category: Modifiers
Scope: public
In: unsigned int, feature number
Out: bool, is continuous true or false
Description: Returns if a particular feature is continuous
**********************************************************************/
bool Explore::IsContinuous(unsigned int FeatureNumber) {
  return Population.IsContinuous(FeatureNumber);
}

/**********************************************************************
Function: GetName()
Category: Selectors
Scope: public
In: -
Out: string, project naem
Description: Returns the project name.
**********************************************************************/
string Explore::GetName() {
  return Name;
}

/**********************************************************************
Function: GetDescription()
Category: Selectors
Scope: public
In: -
Out: string, project description
Description: Returns the project description.
**********************************************************************/
string Explore::GetDescription() {
  return Description;
}


/**********************************************************************
Function: GetPartitionMethod()
Category: Selectors
Scope: public
In: -
Out: PARTITION_METHOD, partitioning method
Description: Returns the partition method of the project.
**********************************************************************/
PARTITION_METHOD Explore::GetPartitionMethod() {
  return PartitionMethod;
}

/**********************************************************************
Function: GetRandomize()
Category: Selectors
Scope: public
In: -
Out: bool, randomize or not
Description: Returns whether the population is randomized during
partitioning or not.
**********************************************************************/
bool Explore::GetRandomize() {
  if (Randomize) {
    return true;
  }
  return false;
}

/**********************************************************************
Function: GetNoPartitions()
Category: Selectors
Scope: public
In: -
Out: unsigned int, number of partitions
Description: Returns the number of partitions in the population.
**********************************************************************/
unsigned int Explore::GetNoPartitions() {
  return NoPartitions;
}

/**********************************************************************
Function: GetLearnRatio()
Category: Selectors
Scope: public
In: -
Out: float, learnratio
Description: Returns the learnratio of the population.
**********************************************************************/
float Explore::GetLearnRatio() {
  return LearnRatio;
}

/**********************************************************************
Function: GetOperatorMethod()
Category: Selectors
Scope: public
In: -
Out: OPERATOR_METHOD, the operator method 
Description: Returns the method to determine operators of each feature.
**********************************************************************/
OPERATOR_METHOD Explore::GetOperatorMethod() {
  return OperatorMethod;
}

/**********************************************************************
Function: GetCutoffMethod()
Category: Selectors
Scope: public
In: -
Out: CUTOFF_METHOD, the cutoff method
Description: Returns the method to determine cutoffs of each feature.
**********************************************************************/
CUTOFF_METHOD Explore::GetCutoffMethod() {
  return CutoffMethod;
}

/**********************************************************************
Function: GetPositiveClass()
Category: Selectors
Scope: public
In: -
Out: unsigned int, order of the positive class
Description: Returns the name of the positive class in the population.
**********************************************************************/
unsigned int Explore::GetPositiveClass() {
  return PositiveClass;
}

/**********************************************************************
Function: GetRestriction()
Category: Selectors
Scope: public
In: -
Out: string, restriction
Description: Returns the restriction.
**********************************************************************/
string Explore::GetRestriction() {
  return RestrictionString;
}

/**********************************************************************
Function: GetMaximizeMeasure()
Category: Selectors
Scope: public
In: -
Out: PERFORMANCE_MEASURE, performance measure
Description: Returns the performance measure that will be maximized.
**********************************************************************/
PERFORMANCE_MEASURE Explore::GetMaximizeMeasure() {
  return MaximizeMeasure;
}

/**********************************************************************
Function: GetConstraints()
Category: Selectors
Scope: public
In: -
Out: vector<CONSTRAINT>, constraints
Description: Returns a vector with constraints set for the project.
**********************************************************************/
vector<CONSTRAINT> Explore::GetConstraints() {
  return Constraints;
}

/**********************************************************************
Function: GetRuleOutputMethod()
Category: Selectors
Scope: public
In: -
Out: RULE_OUTPUT_METHOD, rule output method
Description: Returns the rule output method for this project.
**********************************************************************/
RULE_OUTPUT_METHOD Explore::GetRuleOutputMethod() {
  return RuleOutputMethod;
}

/**********************************************************************
Function: GetPrintSettings()
Category: Selectors
Scope: public
In: -
Out: bool
Description:
**********************************************************************/
bool Explore::GetPrintSettings() {
  return IsPrintSettings;
}

/**********************************************************************
Function: GetPrintPartitions()
Category: Selectors
Scope: public
In: -
Out: bool
Description:
**********************************************************************/
bool Explore::GetPrintPartitions() {
  return IsPrintPartitions;
}

/**********************************************************************
Function: GetPrintCombinations()
Category: Selectors
Scope: public
In: -
Out: bool
Description:
**********************************************************************/
bool Explore::GetPrintCombinations() {
  return IsPrintCombinations;
}

/**********************************************************************
Function: GetPrintFeatureSets()
Category: Selectors
Scope: public
In: -
Out: bool
Description:
**********************************************************************/
bool Explore::GetPrintFeatureSets() {
  return IsPrintFeatureSets;
}

/**********************************************************************
Function: GetPrintCutoffSets()
Category: Selectors
Scope: public
In: -
Out: bool
Description:
**********************************************************************/
bool Explore::GetPrintCutoffSets() {
  return IsPrintCutoffSets;
}

/**********************************************************************
Function: GetPrintCutoffSets()
Category: Selectors
Scope: public
In: -
Out: bool
Description:
**********************************************************************/
bool Explore::GetPrintCutoffSetsBestLength() {
    return IsPrintCutoffSetsBestLength;
}


/**********************************************************************
Function: GetPrintPerformance()
Category: Selectors
Scope: public
In: -
Out: bool
Description:
**********************************************************************/
bool Explore::GetPrintPerformance() {
  return IsPrintPerformance;
}

/**********************************************************************
Function: GetPrintSets()
Category: Selectors
Scope: public
In: -
Out: bool
Description:
**********************************************************************/
bool Explore::GetPrintSets() {
  return IsPrintSets;
}

/**********************************************************************
Function: GetPrintOperatorMethod()
Category: Selectors
Scope: public
In: -
Out: bool
Description:
**********************************************************************/
bool Explore::GetPrintOperatorMethod() {
  return IsPrintOperatorMethod;
}

/**********************************************************************
Function: GetPrintCutoffMethod()
Category: Selectors
Scope: public
In: -
Out: bool
Description:
**********************************************************************/
bool Explore::GetPrintCutoffMethod() {
  return IsPrintCutoffMethod;
}

/**********************************************************************
Function: GetPrintOperatorValues()
Category: Selectors
Scope: public
In: -
Out: bool
Description:
**********************************************************************/
bool Explore::GetPrintOperatorValues() {
  return IsPrintOperatorValues;
}

/**********************************************************************
Function: GetPrintCutoffValues()
Category: Selectors
Scope: public
In: -
Out: bool
Description:
**********************************************************************/
bool Explore::GetPrintCutoffValues() {
  return IsPrintCutoffValues;
}

/**********************************************************************
Function: GetPrintFeatureOperators()
Category: Selectors
Scope: public
In: -
Out: bool
Description:
**********************************************************************/
bool Explore::GetPrintFeatureOperators() {
  return IsPrintFeatureOperators;
}

/**********************************************************************
Function: GetSavePartitions()
Category: Selectors
Scope: public
In: -
Out: bool
Description:
**********************************************************************/
bool Explore::GetSavePartitions() {
  return IsSavePartitions;
}

bool Explore::IsCombinationGenerated() {
  return Rule.IsCombinationGenerated();
}

bool Explore::IsFeatureSetGenerated() {
  return Rule.IsFeatureSetGenerated();
}

bool Explore::IsCutoffSetGenerated() {
  return Rule.IsCutoffSetGenerated();
}

/**********************************************************************
Function: GetBranchBound()
Category: Selectors
Scope: public
In: -
Out: bool, rules will be optimized or not when possible
Description: Returns whether explore will optimize rule-generation
when possible.
**********************************************************************/
bool Explore::GetBranchBound() {
  return BranchBound;
}

/**********************************************************************
Function: GetSubSumption()
Category: Selectors
Scope: public
In: -
Out: bool, rules will be optimized or not when possible
Description: Returns whether explore will use subsumption
**********************************************************************/
bool Explore::GetSubSumption() {
  return SubSumption;
}

/**********************************************************************
Function: GetParallel()
Category: Selectors
Scope: public
In: -
Out: bool, rules will be optimized or not when possible
Description: Returns whether explore will use parallelization
**********************************************************************/
bool Explore::GetParallel() {
    return Parallel;
}

/**********************************************************************
Function: AddFeature()
Category: Modifiers
Scope: public
In: -
Out: bool, could/could add the feature to the population.
Description: Add a feature to the population.
**********************************************************************/
bool Explore::AddFeature(string FeatureName, unsigned int Nom) {
  Population.AddFeature(FeatureName,&CutoffMethod,&OperatorMethod,Nom);
  Rule.SetNoFeatures(Population.GetNoFeatures());
  return true;
}

/**********************************************************************
Function: AddObservation()
Category: Modifiers
Scope: public
In: unsigned int, feature number
    float, observation value
    unsigned int, class of the observation
Out: bool, could/could add observation to the selected feature
Description: Add an observation to a feature in the population.
**********************************************************************/
void Explore::AddObservation(list<string> Values, unsigned int Class) {
  unsigned int FeatureNo;

  list<string>::iterator CurrentValue(Values.begin());
  list<string>::iterator LastValue(Values.end());

  for (FeatureNo=0; CurrentValue != LastValue; CurrentValue++) {
    Population.AddObservation(FeatureNo,(*CurrentValue), Class);
    FeatureNo++;
  }
}

/**********************************************************************
Function: FindCutoffs()
Category: Modifiers
Scope: public
In: -
Out: -
Description: Find cutoffs for all features in the population.
**********************************************************************/
void Explore::FindCutoffs() {
  Population.FindCutoffs();
}

/**********************************************************************
Function: FindOperators()
Category: Modifiers
Scope: public
In: -
Out: -
Description: Find operators for all features in the population.
**********************************************************************/
void Explore::FindOperators() {
  Population.FindOperators();
}

/**********************************************************************
Function: SetBranchBound()
Category: Modifiers
Scope: public
In: bool, optimization should/should not be used if applicable
Out: -
Description: Determines whether Explore will use sensitivity/specificty or
accuracy optimisation when generating new rules.
**********************************************************************/
void Explore::SetBranchBound(bool Optimize) {
  BranchBound = Optimize;
  Rule.SetBranchBound(Optimize);
}

/**********************************************************************
Function: SetSubSumption()
Category: Modifiers
Scope: public
In: bool, Subsumption pruning should/should not be used if applicable
Out: -
Description: Determines whether Explore will use subsumption pruning to
remove unnecessary cutoffs
**********************************************************************/
void Explore::SetSubSumption(bool Value) {
  SubSumption = Value;
}

/**********************************************************************
Function: SetParallel()
Category: Modifiers
Scope: public
In: bool, Parallel should/should not be used if applicable
Out: -
Description: Determines whether Explore will use parallelization
**********************************************************************/
void Explore::SetParallel(bool Value) {
    Parallel = Value;
}

/**********************************************************************
Function: SetCPBest()
Category: Modifiers
Scope: public
In: float, value of Sensitivity constraint
Out: -
Description: Allows to define a start value of CPBest to speedup search
**********************************************************************/
void Explore::SetCPBest(float Sensitivity) {
   Rule.SetCPBest(Sensitivity);
}/**********************************************************************

Function: SetCTBest()
Category: Modifiers
Scope: public
In: float, value of Accuracy constraint
Out: -
Description: Allows to define a start value of CTBest to speedup search
**********************************************************************/
void Explore::SetCTBest(float Accuracy) {
  Rule.SetCTBest(Accuracy);
}

/**********************************************************************
Function: SetOutput()
Category: Modifiers
Scope: public
In: ostream*, an cout stream
Out: -
Description: Set the cout stream of Explore.
**********************************************************************/
void Explore::SetOutput(streambuf* OutputBuffer) {
  cout.rdbuf(OutputBuffer);
}

/**********************************************************************
Function: SetUpdateRealtime()
Category: Modifiers
Scope: public
In: bool, yes or no
Out: -
Description: Indicate whether explore has to cout summary information
in real-time.
**********************************************************************/
void Explore::SetUpdateRealtime(bool Setting) {
  IsUpdateRealtime = Setting;
}

/**********************************************************************
Function: SetPrintSettings()
Category: Modifiers
Scope: public
In: bool, yes or no
Out: -
Description: Indicate whether explore has to cout settings.
**********************************************************************/
void Explore::SetPrintSettings(bool Setting) {
  IsPrintSettings = Setting;
}

/**********************************************************************
Function: SetPrintPartitions()
Category: Modifiers
Scope: public
In: bool, yes or no
Out: -
Description: Indicate whether explore has to cout partition information.
**********************************************************************/
void Explore::SetPrintPartitions(bool Setting) {
  IsPrintPartitions = Setting;
}

/**********************************************************************
Function: SetSavePartitions()
Category: Modifiers
Scope: public
In: bool, yes or no
Out: -
Description: Indicate whether explore has to save partition information.
**********************************************************************/
void Explore::SetSavePartitions(bool Setting) {
  IsSavePartitions = Setting;
}

/**********************************************************************
Function: SetPrintCombinations()
Category: Modifiers
Scope: public
In: bool, yes or no
Out: -
Description: Indicate whether explore has to cout combinations.
**********************************************************************/
void Explore::SetPrintCombinations(bool Setting) {
  IsPrintCombinations = Setting;
  Rule.SetPrintCombinations(Setting);
}

/**********************************************************************
Function: SetPrintFeatureSets()
Category: Modifiers
Scope: public
In: bool, yes or no
Out: -
Description: Indicate whether explore has to cout featuresets.
**********************************************************************/
void Explore::SetPrintFeatureSets(bool Setting) {
  IsPrintFeatureSets = Setting;
  Rule.SetPrintFeatureSets(Setting);
}

/**********************************************************************
Function: SetPrintconditionSets()
Category: Modifiers
Scope: public
In: bool, yes or no
Out: -
Description: Indicate whether explore has to cout conditionsets.
**********************************************************************/
void Explore::SetPrintCutoffSets(bool Setting) {
  IsPrintCutoffSets = Setting;
  Rule.SetPrintCutoffSets(Setting);
}

/**********************************************************************
Function: SetPrintconditionSets()
Category: Modifiers
Scope: public
In: bool, yes or no
Out: -
Description: Indicate whether explore has to cout conditionsets.
**********************************************************************/
void Explore::SetPrintCutoffSetsBestLength(bool Setting) {
    IsPrintCutoffSetsBestLength = Setting;
    Rule.SetPrintCutoffSetsBestLength(Setting);
}

/**********************************************************************
Function: SetPrintPerformance()
Category: Modifiers
Scope: public
In: bool, yes or no
Out: -
Description: Indicate whether explore has to cout performances.
**********************************************************************/
void Explore::SetPrintPerformance(bool Setting) {
  IsPrintPerformance = Setting;
}

/**********************************************************************
Function: SetPrintSets()
Category: Modifiers
Scope: public
In: bool, yes or no
Out: -
Description: Indicate whether explore has to cout sets.
**********************************************************************/
void Explore::SetPrintSets(bool Setting) {
  IsPrintSets = Setting;
}

/**********************************************************************
Function: SetPrintOperatorMethod()
Category: Modifiers
Scope: public
In: bool, yes or no
Out: -
Description: Indicate whether explore has to cout operator method information.
**********************************************************************/
void Explore::SetPrintOperatorMethod(bool Setting) {
  IsPrintOperatorMethod = Setting;
  Population.SetPrintOperatorMethod(Setting);
}

/**********************************************************************
Function: SetPrintCutoffMethod()
Category: Modifiers
Scope: public
In: bool, yes or no
Out: -
Description: Indicate whether explore has to print cutoff method information.
**********************************************************************/
void Explore::SetPrintCutoffMethod(bool Setting) {
  IsPrintCutoffMethod = Setting;
  Population.SetPrintCutoffMethod(Setting);
}

/**********************************************************************
Function: SetPrintOperatorValues()
Category: Modifiers
Scope: public
In: bool, yes or no
Out: -
Description: Indicate whether explore has to print operator values.
**********************************************************************/
void Explore::SetPrintOperatorValues(bool Setting) {
  IsPrintOperatorValues = Setting;
}

/**********************************************************************
Function: SetPrintCutoffValues()
Category: Modifiers
Scope: public
In: bool, yes or no
Out: -
Description: Indicate whether explore has to print cutoff values.
**********************************************************************/
void Explore::SetPrintCutoffValues(bool Setting) {
  IsPrintCutoffValues = Setting;
}

/**********************************************************************
Function: SetPrintFeatureOperators()
Category: Modifiers
Scope: public
In: bool, yes or no
Out: -
Description: Indicate whether explore has to print featureoperators
and their corresponding cutoff values.
**********************************************************************/
void Explore::SetPrintFeatureOperators(bool Setting) {
  IsPrintFeatureOperators = Setting;
}

/**********************************************************************
Function: GetCutoffValues()
Category: Modifiers
Scope: public
In: unsigned int, featurenumber
Out: list<float>, cutoffs
Description: Return a <list> of cutoff values for the specified feature.
**********************************************************************/
list<string> Explore::GetCutoffValues(unsigned int FeatureNumber) {
  return Population.GetCutoffValues(FeatureNumber);
}

/**********************************************************************
Function: GetROC()
Category: Modifiers
Scope: public
In: unsigned int, rulelength
Out: vector<double>*, the ROC Curve
Description: Returns the ROC Curve for a given rulelength.
**********************************************************************/
/*
vector<double>* Explore::GetROC(unsigned int RuleLength) {
  if (RuleLength>=GetMinRuleLength() && RuleLength<=GetMaxRuleLength()) {
    return Rule.GetROC(RuleLength);
  }
  return NULL;
} */

/**********************************************************************
Function: Start()
Category: Modifiers
Scope: public
In: -
Out: -
Description: Start explore rule-generation.
**********************************************************************/
void Explore::Start() {
  bool Finished = false;
  time_t starttime,endtime;
  if (Initialised) {
	PreviousProgressCount = 0;                                                  // Initialise counters to small blue bar
	CurrentProgressCount = 2;

	StartRuleLength = clock();
	time(&starttime);
	cout << "Start at : " << ctime(&starttime);
	cout << endl << "RULES" << endl << endl;

	Finished = RunProject();
  }
  if (Finished) {

	PrintSummary();

	// TODO: decide which complexity to report (update for parallel options)
	cout << endl << "COMPLEXITY" << endl << endl;
	cout << "Combinations: " <<  Rule.GetCombinationsGenerated() << endl;
	cout << "Feature sets: " << Rule.GetFeatureSetsGenerated() << endl;
	cout << "Cutoff sets: " << Rule.GetCutoffSetsGenerated() << endl << endl;

	cout << endl << "TIMING" << endl << endl;
	time(&endtime);
	cout << "Project end: " << ctime(&endtime) << endl;
	
	std::stringstream sstr;
	sstr << "RuleLength:" << Rule.GetMaxRuleLength();
	RuleLengthTiming.Clear();
	RuleLengthTiming.AddTime(sstr.str(),StartRuleLength,clock());
	cout << RuleLengthTiming.PrintAll();


#ifdef DEBUG_TIMING
    cout << ExploreTiming.PrintAll();
#endif

#ifndef COMMANDVERSION
	CloseFunction();                                                            // Explore has ended
#endif
  }
}

/**********************************************************************
Function: Resume()
Category: Modifiers
Scope: public
In: -
Out: -
Description: Resume explore rule-generation.
**********************************************************************/
void Explore::Resume() {
  bool Finished = false;
  time_t dummy;

  if (Initialised) {
    Finished = ResumeProject();
  }
  if (Finished) {
	PrintSummary();

	cout << endl << "TIMING" << endl << endl;
	std::stringstream sstr;
	sstr << "RuleLength:" << Rule.GetCurrentRuleLength();
	RuleLengthTiming.AddTime(sstr.str(),StartRuleLength,clock());

	cout << RuleLengthTiming.PrintAll();
	//cout << "Project start: " << ctime(&StartTime) << endl;
    //cout << "Project end: " << ctime(&EndTime) << endl;

#ifdef DEBUG_TIMING
    cout << ExploreTiming.PrintAll();
#endif

#ifndef COMMANDVERSION
	CloseFunction();                                                            // Explore has ended
#endif
  }
}

/**********************************************************************
Function: Reset()
Category: Modifiers
Scope: public
In: -
Out: -
Description: Reset explore rule-generation.
**********************************************************************/
void Explore::Reset() {
  Initialised = false;

  ClearCandidates();
  ClearConstraints();

  Population.Clear();
  Rule.Reset();

  PreviousProgressCount = 0;
  CurrentProgressCount = 2;

  NoPartitionsDone = 0;
}

/**********************************************************************
Function: SetRerun()
Category: Modifiers
Scope: public
In: -
Out: -
Description: Sets explore for a rerun of the project or next partition
**********************************************************************/
void Explore::SetRerun() {

  Population.FindCutoffs();                                                     // Find the cutoffs and operators again
  Population.FindOperators();

  Rule.SetRerun(Population.GetFeatures());                                                              // Initialise the rule
  InitialiseCPFP();                                                             // order by cp etc
}

/**********************************************************************
Function: Stop()
Category: Modifiers
Scope: public
In: -
Out: -
Description: is stop criterium met?
**********************************************************************/
//int Explore::Stop() {
//	float best;
//	float current;
//	unsigned int last=0;
//	if (Initialised) {
//
//	Rule.SetTestMode(VALIDATION);
//	if (PartitionCandidates.size()>0) {
//	  cout << "Checking Stop Criterium" << endl;
//	  cout << endl << "BEST RULES (" << PartitionCandidates.size() << " candidates)" << endl << endl;
//	  for (unsigned int i=GetMinRuleLength(); i<=GetMaxRuleLength(); i++){
//		if (ChooseBestCandidate(i)){
//		  cout << "RULELENGTH " << i << endl << endl;
//		  if (Rule.SetRule(BestCandidate))
//		  {
//			last++;
//			cout << "Best candidate: ";
//			Rule.PrintCutoffSet();
//			cout << endl;
//			cout << "Learn-set: ";
//			BestCandidate.Performance.Print();
//			cout << endl;
//			BestCandidate.Performance = Rule.CalculatePerformance();          // Test BestCandidate on test partition
//			cout << "test-set: ";
//			BestCandidate.Performance.Print();
//
//			switch (MaximizeMeasure){
//
//			case ACCURACY:
//			  current = BestCandidate.Performance.Accuracy.Value;
//			  break;
//			case SENSITIVITY:
//			  current = BestCandidate.Performance.Sensitivity.Value;
//			  break;
//			case SPECIFICITY:
//			  current = BestCandidate.Performance.Specificity.Value;
//			  break;
//			}
//			if (i==1) {
//			  best = current;
//			}
//			else {
//			 if (current < best) {
//			   cout << "Stopped at " << i-1 << endl;
//			   return i-1;
//			 }
//			}
//		  }
//		}
//	  }
//
//	  if (last==GetMaxRuleLength()){
//		if (current > best) {
//		  return GetMaxRuleLength();
//		} else {
//		 return GetMaxRuleLength()-1;
//		}
//	  }
//	  //PartitionCandidates.clear();
//	} else {
//	  #if defined(EXPLORE_MPI_DEBUG)
//	  cout << "--> No Candidates" << endl;
//	  #endif
//	}
//  }
//   Rule.SetTestMode(LEARN);
//   return 0;
//}

/**********************************************************************
Function: BestLength()
Category: Modifiers
Scope: public
In: -
Out: -
Description: is stop criterium met?
**********************************************************************/
//int Explore::FindBestLength() {
//	float best;
//	float current;
//	int Opt=0;
//	if (Initialised) {
//        Rule.SetTestMode(VALIDATION);
//
//	if (PartitionCandidates.size()>0) {
//	  for (unsigned int i=GetMinRuleLength(); i<=GetMaxRuleLength(); i++){
//		if (ChooseBestCandidate(i)){
//		  if (Rule.SetRule(BestCandidate))
//		  {
//		    cout << "RULELENGTH " << i << endl << endl;
//			cout << "Best candidate (within this partition): ";
//			Rule.PrintCutoffSet();
//			cout << endl;
//			cout << "Learn-set: ";
//			BestCandidate.Performance.Print();
//			cout << endl;
//
//              if (!(GetPartitionMethod()==RESUBSTITUTION)){
//                  BestCandidate.Performance = Rule.CalculatePerformance();          // Test BestCandidate on validation partition
//                  cout << "Validation-set: ";
//                  BestCandidate.Performance.Print();
//                  cout << endl;
//              }
//			switch (MaximizeMeasure){
//
//			case ACCURACY:
//			  current = BestCandidate.Performance.Accuracy.Value;
//			  break;
//			case SENSITIVITY:
//			  current = BestCandidate.Performance.Sensitivity.Value;
//			  break;
//			case SPECIFICITY:
//			  current = BestCandidate.Performance.Specificity.Value;
//			  break;
//			}
//			if (i==1) {
//			  best = current;
//			  Opt = 1;
//			}
//			else {
//			 if (current > best) {
//			   best = current;
//			   Opt = i;
//			 }
//			}
//		  }
//		}
//	  }
//	  return Opt;
//	} else {
//	  #if defined(EXPLORE_MPI_DEBUG)
//	  cout << "--> No Candidates" << endl;
//	  #endif
//	}
//  }
//  return 0;
//}
//


/**********************************************************************
Function: RunProject()
Category: Modifiers
Scope: public
In: -
Out: bool, project finished or not
Description: Starts an Explore project.
**********************************************************************/
bool Explore::RunProject() {
  unsigned int BreatheCount = 0;
  unsigned int Partitionnr = 0;
  time_t dummy;
  unsigned int ActiveRuleLength;
  int CountCandidatesPartition;
  int CountFeatureOperatorPairs;
  int CountCutoffSets;
  int BestLengthPartition;
  int BestLengthFinal = 0;
  vector<int> BestLength(Rule.GetMaxRuleLength());
  int CPBest_global = 0;
  int CTBest_global = 0;

  TIMING TermTupleTiming;
  clock_t StartTimeTermTuple;

  #ifdef HYPER
   Jibu::Manager::initialize();
   cout << "There are " << Jibu::Manager::getProcessorCount()
      << " processors available to Jibu." << std::endl;
  #endif

	do {
	    CountCandidatesPartition = 0;
        CountFeatureOperatorPairs = 0;
        CountCutoffSets = 0;
	  BestLengthPartition = 0;
	  Final = false;

   	  // Print settings for this partition
	  Partitionnr++;
	  cout << "PARTITION: " << Partitionnr << endl;
	  cout << endl;
	  if (IsSavePartitions)      SavePartitions(Partitionnr);
      if (IsPrintPartitions)     Population.PrintPartitions();
	  if (IsPrintOperatorMethod) Population.PrintOperatorMethod();
      if (IsPrintOperatorValues) Population.PrintOperators();
      if (IsPrintCutoffMethod)   Population.PrintCutoffMethod();
	  if (IsPrintCutoffValues)   Population.PrintCutoffs();

if (!Parallel) {
    float CandidatePerformance;

    while (Rule.NextCombinationGenerator()) {
        if (IsPrintCombinations) Rule.PrintCombination();

        StartTimeTermTuple = clock();

       while (Rule.NextFeatureSetGenerator(0, Rule.GetFeatureOperatorSize())) {
            if (IsPrintFeatureSets) Rule.PrintFeatureSet();
            CountFeatureOperatorPairs++;
            // CalculateProgress();
            while (Rule.NextCutoffSetGenerator()) {

                switch (MaximizeMeasure) {
                    case SENSITIVITY:
                        CandidatePerformance = PartitionCandidates.Performance.Sensitivity.Value;
                        break;
                    case SPECIFICITY:
                        CandidatePerformance = PartitionCandidates.Performance.Specificity.Value;
                        break;
                    case NPV:
                        CandidatePerformance = PartitionCandidates.Performance.NPV.Value;
                        break;
                    case PPV:
                        CandidatePerformance = PartitionCandidates.Performance.PPV.Value;
                        break;
                    case ACCURACY:
                        CandidatePerformance = PartitionCandidates.Performance.Accuracy.Value;
                        break;
                    case BALANCEDACCURACY:
                        CandidatePerformance = PartitionCandidates.Performance.BalancedAccuracy.Value;
                        break;
                    case F1SCORE:
                        CandidatePerformance = PartitionCandidates.Performance.F1score.Value;
                        break;
                }

                if (Rule.TestRule(Initialised, Constraints,
                                  CandidatePerformance, MaximizeMeasure, RestrictionSet,
                                  RuleOutputMethod, IsPrintPerformance, IsPrintSets)) {

                    PartitionCandidates = Rule.SaveCandidate(MaximizeMeasure, RestrictionSet);
                }

                // TODO: check if inside or outside TestRule
                if (IsPrintCutoffSets) { // Calculate performance of current rule in learn set
                    cout << "Candidate model: ";
                    Rule.PrintCutoffSet();
                }

                CountCutoffSets++;

                // if (IsUpdateRealtime) CalculateProgress();

#ifndef COMMANDVERSION
                // BreatheCount++;// Increment breathe counter

                                        if (BreatheCount>BREATHE_INTERVAL) {
                                         if (PauseFunction()) {                                            // User paused the project
                                            PrintSummary();
                                            return false;
                                          }
                                          if (CancelFunction()) {                                           // User cancelled the project
                                            PrintSummary();
                                            CloseFunction();
                                            return false;
                                          }
                                          BreatheCount = 0;
                                        }
#endif
            }
        }

//        std::stringstream sstr;
//        TermTupleTiming.Clear();
//        TermTupleTiming.AddTime(sstr.str(), StartTimeTermTuple, clock());
//        Rule.PrintCombination();
//        cout << TermTupleTiming.PrintTotal();
//        cout << "Candidates: " <<  Rule.GetCountCandidates() << endl << endl;
        CountCandidatesPartition += Rule.GetCountCandidates();
        Rule.ResetCountCandidates();
    }

} else {

    string ParallelMethod = "Global double parallel";

    if (ParallelMethod == "Global single parallel") {

        vector<RULE> all_rules;
        while(Rule.NextCombinationGenerator()) {
            all_rules.push_back(this->Rule);
        }

        tbb::parallel_for(tbb::blocked_range<int>(0, Rule.GetCombinationsGenerated()), [&](tbb::blocked_range<int> r) {
            for (int i = r.begin(); i < r.end(); ++i) {
                StartTimeTermTuple = clock();

                // RULE Rule_i = all_rules.at(i);

                RULE Rule_i = RULE(all_rules[i]); // CREATE DEEP COPY

                float CandidatePerformance;
                CANDIDATE PotentialCandidate;

                m2.lock();

                Rule_i.CPBest = CPBest_global;
                Rule_i.CTBest = CTBest_global;

                switch (MaximizeMeasure) {
                    case SENSITIVITY:
                        CandidatePerformance = PartitionCandidates.Performance.Sensitivity.Value;
                        break;
                    case SPECIFICITY:
                        CandidatePerformance = PartitionCandidates.Performance.Specificity.Value;
                        break;
                    case NPV:
                        CandidatePerformance = PartitionCandidates.Performance.NPV.Value;
                        break;
                    case PPV:
                        CandidatePerformance = PartitionCandidates.Performance.PPV.Value;
                        break;
                    case ACCURACY:
                        CandidatePerformance = PartitionCandidates.Performance.Accuracy.Value;
                        break;
                    case BALANCEDACCURACY:
                        CandidatePerformance = PartitionCandidates.Performance.BalancedAccuracy.Value;
                        break;
                    case F1SCORE:
                        CandidatePerformance = PartitionCandidates.Performance.F1score.Value;
                        break;
                }
                m2.unlock();

                if (IsPrintCombinations) Rule_i.PrintCombination();

                // CANDIDATE PartitionCandidates_i; // doesn't need to be concurrent vector
                // printf("Combination %d and feature operators set %d and address %p and thread id %d \n", i, j, &Rule_i, tbb::this_task_arena::current_thread_index());

                while (Rule_i.NextFeatureSetGenerator(0, Rule_i.GetFeatureOperatorSize())) {
                    if (IsPrintFeatureSets) Rule_i.PrintFeatureSet_Thread();
                    // CalculateProgress();
                    m0.lock();
                    CountFeatureOperatorPairs++;
                    m0.unlock();

                    while (Rule_i.NextCutoffSetGenerator()) {

                        if (Rule_i.TestRule(Initialised, Constraints,
                                            CandidatePerformance, MaximizeMeasure, RestrictionSet,
                                            RuleOutputMethod, IsPrintPerformance, IsPrintSets)) {

                            PotentialCandidate = Rule_i.SaveCandidate(MaximizeMeasure,
                                                                       RestrictionSet);
                            m2.lock();

                            bool change;
                            switch (MaximizeMeasure) {
                                case SENSITIVITY:
                                    change = (PotentialCandidate.Performance.Sensitivity.Value > PartitionCandidates.Performance.Sensitivity.Value);
                                    break;
                                case SPECIFICITY:
                                    change = (PotentialCandidate.Performance.Specificity.Value > PartitionCandidates.Performance.Specificity.Value);
                                    break;
                                case NPV:
                                    change = (PotentialCandidate.Performance.NPV.Value > PartitionCandidates.Performance.NPV.Value);
                                    break;
                                case PPV:
                                    change = (PotentialCandidate.Performance.PPV.Value > PartitionCandidates.Performance.PPV.Value);
                                    break;
                                case ACCURACY:
                                    change = (PotentialCandidate.Performance.Accuracy.Value > PartitionCandidates.Performance.Accuracy.Value);
                                    break;
                                case BALANCEDACCURACY:
                                    change = (PotentialCandidate.Performance.BalancedAccuracy.Value > PartitionCandidates.Performance.BalancedAccuracy.Value);
                                    break;
                                case F1SCORE:
                                    change = (PotentialCandidate.Performance.F1score.Value > PartitionCandidates.Performance.F1score.Value);
                                    break;
                            }

                            if (change) {
                                PartitionCandidates = PotentialCandidate;

                                CPBest_global = Rule_i.CPBest;
                                CTBest_global = Rule_i.CTBest;
                            }
                            m2.unlock();
                        }

                        // TODO: check if inside or outside TestRule
                        if (IsPrintCutoffSets) { // Calculate performance of current rule in learn set
                            cout << "Candidate model: ";
                            Rule_i.PrintCutoffSet();
                        }
                        m1.lock();
                        CountCutoffSets++;
                        m1.unlock();
                        // if (IsUpdateRealtime) CalculateProgress();

#ifndef COMMANDVERSION
                        // BreatheCount++;// Increment breathe counter

                            if (BreatheCount>BREATHE_INTERVAL) {
                             if (PauseFunction()) {                                            // User paused the project
                                PrintSummary();
                                return false;
                              }
                              if (CancelFunction()) {                                           // User cancelled the project
                                PrintSummary();
                                CloseFunction();
                                return false;
                              }
                              BreatheCount = 0;
                            }
#endif
                    }
                }

//                std::stringstream sstr;
//                TermTupleTiming.Clear();
//                TermTupleTiming.AddTime(sstr.str(), StartTimeTermTuple, clock());
//                Rule_i.PrintCombination();
//                cout << TermTupleTiming.PrintTotal();
//                cout << "Candidates: " << Rule_i.GetCountCandidates() << endl << endl;
                m3.lock();
                CountCandidatesPartition += Rule_i.GetCountCandidates();
                m3.unlock();
            }
            // }
        });
    } else if (ParallelMethod == "Global double parallel") {

        vector<RULE> all_rules;
        while(Rule.NextCombinationGenerator()) {
            all_rules.push_back(this->Rule);
        }

        tbb::parallel_for(tbb::blocked_range<int>(0, Rule.GetCombinationsGenerated()), [&](tbb::blocked_range<int> r) {

            // for (int i = r.begin(); i < r.end(); ++i)
            int i = r.begin();
            {
                StartTimeTermTuple = clock();

                // NOTE: blocked_range uses open interval [start,end)
                tbb::parallel_for(tbb::blocked_range<int>(0, Rule.GetFeatureOperatorSize() + 1), [&](tbb::blocked_range<int> s) {

                    // for (int j = s.begin(); j < s.end(); j++)
                    {
                        int j = s.begin();

                        RULE Rule_ij = RULE(all_rules[i]); // CREATE DEEP COPY

                        float CandidatePerformance;
                        CANDIDATE PotentialCandidate;

                        m2.lock();

                        Rule_ij.CPBest = CPBest_global;
                        Rule_ij.CTBest = CTBest_global;

                        switch (MaximizeMeasure) {
                            case SENSITIVITY:
                                CandidatePerformance = PartitionCandidates.Performance.Sensitivity.Value;
                                break;
                            case SPECIFICITY:
                                CandidatePerformance = PartitionCandidates.Performance.Specificity.Value;
                                break;
                            case NPV:
                                CandidatePerformance = PartitionCandidates.Performance.NPV.Value;
                                break;
                            case PPV:
                                CandidatePerformance = PartitionCandidates.Performance.PPV.Value;
                                break;
                            case ACCURACY:
                                CandidatePerformance = PartitionCandidates.Performance.Accuracy.Value;
                                break;
                            case BALANCEDACCURACY:
                                CandidatePerformance = PartitionCandidates.Performance.BalancedAccuracy.Value;
                                break;
                            case F1SCORE:
                                CandidatePerformance = PartitionCandidates.Performance.F1score.Value;
                                break;
                        }
                        m2.unlock();

                        if (IsPrintCombinations) Rule_ij.PrintCombination();

                        // CANDIDATE PartitionCandidates_ij; // doesn't need to be concurrent vector
                        // printf("Combination %d and feature operators set %d and address %p and thread id %d \n", i, j, &Rule_ij, tbb::this_task_arena::current_thread_index());

                        while (Rule_ij.NextFeatureSetGenerator(j, j)) {
                            // TODO: create function that releases all print statements of one thread at once

                            if (IsPrintFeatureSets) Rule_ij.PrintFeatureSet_Thread();

//                            m0.lock();
//                            CountFeatureOperatorPairs++;
//                            m0.unlock();
                            // CalculateProgress();

                            while (Rule_ij.NextCutoffSetGenerator()) {

                                if (Rule_ij.TestRule(Initialised, Constraints,
                                                    CandidatePerformance, MaximizeMeasure, RestrictionSet,
                                                    RuleOutputMethod, IsPrintPerformance, IsPrintSets)) {

                                    PotentialCandidate = Rule_ij.SaveCandidate(MaximizeMeasure, RestrictionSet);

                                    m2.lock();

                                    bool change;
                                    switch (MaximizeMeasure) {
                                        case SENSITIVITY:
                                            change = (PotentialCandidate.Performance.Sensitivity.Value > PartitionCandidates.Performance.Sensitivity.Value);
                                            break;
                                        case SPECIFICITY:
                                            change = (PotentialCandidate.Performance.Specificity.Value > PartitionCandidates.Performance.Specificity.Value);
                                            break;
                                        case NPV:
                                            change = (PotentialCandidate.Performance.NPV.Value > PartitionCandidates.Performance.NPV.Value);
                                            break;
                                        case PPV:
                                            change = (PotentialCandidate.Performance.PPV.Value > PartitionCandidates.Performance.PPV.Value);
                                            break;
                                        case ACCURACY:
                                            change = (PotentialCandidate.Performance.Accuracy.Value > PartitionCandidates.Performance.Accuracy.Value);
                                            break;
                                        case BALANCEDACCURACY:
                                            change = (PotentialCandidate.Performance.BalancedAccuracy.Value > PartitionCandidates.Performance.BalancedAccuracy.Value);
                                            break;
                                        case F1SCORE:
                                            change = (PotentialCandidate.Performance.F1score.Value > PartitionCandidates.Performance.F1score.Value);
                                            break;
                                    }

                                    if (change) {
                                        PartitionCandidates = PotentialCandidate;

                                        CPBest_global = Rule_ij.CPBest;
                                        CTBest_global = Rule_ij.CTBest;
                                    }
                                    m2.unlock();
                                }

                                // TODO: check if inside or outside TestRule
                                if (IsPrintCutoffSets) { // Calculate performance of current rule in learn set
                                    cout << "Candidate model: ";
                                    Rule_ij.PrintCutoffSet();
                                }
//                                m1.lock();
//                                CountCutoffSets++;
//                                m1.unlock();
                                // if (IsUpdateRealtime) CalculateProgress();

#ifndef COMMANDVERSION
                                // BreatheCount++;// Increment breathe counter

                                    if (BreatheCount>BREATHE_INTERVAL) {
                                     if (PauseFunction()) {                                            // User paused the project
                                        PrintSummary();
                                        return false;
                                      }
                                      if (CancelFunction()) {                                           // User cancelled the project
                                        PrintSummary();
                                        CloseFunction();
                                        return false;
                                      }
                                      BreatheCount = 0;
                                    }
#endif
                            }
                        }

//                        std::stringstream sstr;
//                        TermTupleTiming.Clear();
//                        TermTupleTiming.AddTime(sstr.str(), StartTimeTermTuple, clock());
//                        Rule_ij.PrintCombination();
//                        cout << TermTupleTiming.PrintTotal();
//                        cout << "Candidates: " << Rule_ij.GetCountCandidates() << endl << endl;
//                        m3.lock();
//                        CountCandidatesPartition += Rule_ij.GetCountCandidates();
//                        m3.unlock();
                    }
                });
            }
        });


    }
}
// TODO: is "Rule" needed?

        BestLengthPartition = PartitionCandidates.Size(); // TODO: for multiple projectcandidates?

        cout << endl << endl;
        cout << "Best Length:" << BestLengthPartition << endl;
        cout << "====================================================" << endl;

        if (BestLengthPartition != 0) {
            // BestLengthPartition = Rule.FindBestLength(Initialised,PartitionCandidates, PartitionMethod, MaximizeMeasure);
            // BestLength[BestLengthPartition - 1] = BestLength[BestLengthPartition - 1] + 1; // Calculate performance of current rule in validation set
            BestLength.at(BestLengthPartition - 1) = BestLength.at(BestLengthPartition - 1) + 1;
        }
    } while (Partition());

	// Choose most frequently occurring Best Length
	auto MostFrequent = std::max_element(BestLength.begin(), BestLength.end());
    BestLengthFinal = std::distance(std::begin(BestLength), MostFrequent) + 1;

    cout << "Results EXPLORE with BestLength " << BestLengthFinal << " on full train set" << endl;
    if ((GetPartitionMethod())==CROSS_VALIDATION || (GetPartitionMethod())==HOLDOUT) {
        // Re-train model with full train set (learn + validate)
        Population.ResetTestPartitions(); // Sets all partitions to LEARN
        // PartitionCandidates.clear(); // Remove all the partition candidates used to find BestLength
        PartitionCandidates.Clear();

        SetRerun();

        Induce(BestLengthFinal, BestLengthFinal);

        Final = true;
        ValidateBestCandidate(); // Print results on full train set and save best rule

    } else {

#ifndef PARALLEL
        // Directly print results on full train set and save best rule
        if (PartitionCandidates.IsValid()) {
        // if (PartitionCandidates.size()>0) {
            CANDIDATE BestCandidate = PartitionCandidates;
            // CANDIDATE BestCandidate = Rule.ChooseBestCandidate(BestLengthFinal, Initialised, PartitionCandidates, MaximizeMeasure);
               // if (ChooseBestCandidate(BestLengthFinal)){
                    if (Rule.SetRule(BestCandidate)) {
                        cout << "Best candidate (overall): ";
                        Rule.PrintCutoffSet();
                        cout << endl;
                        cout << "Learn-set: ";
                        BestCandidate.Performance.Print();
                        cout << endl;

                        ProjectCandidates.push_back(BestCandidate);

                        cout << "Total Count Candidates:" << CountCandidatesPartition << endl;
                        cout << "Total Count Feature Operator Pairs:" << CountFeatureOperatorPairs << endl;
                        cout << "Total Count Cutoff Sets:" << CountCutoffSets << endl;

                    }
               // }
            }
# else
        Population.ResetTestPartitions(); // Sets all partitions to LEARN
        PartitionCandidates.clear(); // Remove all the partition candidates used to find BestLength

        SetRerun();

        Induce(BestLengthFinal, BestLengthFinal);

        Final = true;
        ValidateBestCandidate(); // Print results on full train set and save best rule

# endif

    }

  return true;
}

/**********************************************************************
Function: Induce()
Category: Modifiers
Scop


 e: public
In: Start rule length, End rule length
Out: bool, project finished or not
Description: Resumes an Explore project.
**********************************************************************/
void Explore::Induce(int nStart, int nEnd) {
     Rule.ResetComplexity();

  unsigned int BreatheCount = 0;

  SetMinRuleLength(nStart);
  SetMaxRuleLength(nEnd);
  while (Rule.NextCombinationGenerator()) {
	while (Rule.NextFeatureSetGenerator(0, Rule.GetFeatureOperatorSize())) {
	  while (Rule.NextCutoffSetGenerator()) {

          float CandidatePerformance;
          switch (MaximizeMeasure) {
              case SENSITIVITY:
                  CandidatePerformance = PartitionCandidates.Performance.Sensitivity.Value;
                  break;
              case SPECIFICITY:
                  CandidatePerformance = PartitionCandidates.Performance.Specificity.Value;
                  break;
              case NPV:
                  CandidatePerformance = PartitionCandidates.Performance.NPV.Value;
                  break;
              case PPV:
                  CandidatePerformance = PartitionCandidates.Performance.PPV.Value;
                  break;
              case ACCURACY:
                  CandidatePerformance = PartitionCandidates.Performance.Accuracy.Value;
                  break;
              case BALANCEDACCURACY:
                  CandidatePerformance = PartitionCandidates.Performance.BalancedAccuracy.Value;
                  break;
              case F1SCORE:
                  CandidatePerformance = PartitionCandidates.Performance.F1score.Value;
                  break;
          }

          if (Rule.TestRule(Initialised, Constraints,
                            CandidatePerformance, MaximizeMeasure, RestrictionSet,
                            RuleOutputMethod, IsPrintPerformance, IsPrintSets)) {

              PartitionCandidates = Rule.SaveCandidate(MaximizeMeasure, RestrictionSet);
          }


          if (IsPrintCutoffSetsBestLength) {
              cout << "Candidate model BestLength: ";
              Rule.PrintCutoffSet();
          }
          
		#ifndef COMMANDVERSION
		// BreatheCount++;                                                     	// Increment breathe counter
		if (BreatheCount>BREATHE_INTERVAL) {
		 if (PauseFunction()) {                                           		// User paused the project
			PrintSummary();
			//return false;
		  }
		  if (CancelFunction()) {                                           	// User cancelled the project
			PrintSummary();
			CloseFunction();
		   //return false;
		  }
		  BreatheCount = 0;
		}
		#endif
	  }
	}
  }

  cout << "Total Count Candidates:" << Rule.GetCountCandidates() << endl;
}



/**********************************************************************
Function: ResumeProject()
Category: Modifiers
Scope: public
In: -
Out: bool, project finished or not
Description: Resumes an Explore project.
**********************************************************************/
bool Explore::ResumeProject() {
  unsigned int BreatheCount = 0;

  if (RestrictionSet || Rule.IsMandatorySet()) {
    do {
      CalculateProgress();
      do {
       // TestRule();                                                             // Calculate performance of current rule

          float CandidatePerformance;
          switch (MaximizeMeasure) {
              case SENSITIVITY:
                  CandidatePerformance = PartitionCandidates.Performance.Sensitivity.Value;
                  break;
              case SPECIFICITY:
                  CandidatePerformance = PartitionCandidates.Performance.Specificity.Value;
                  break;
              case NPV:
                  CandidatePerformance = PartitionCandidates.Performance.NPV.Value;
                  break;
              case PPV:
                  CandidatePerformance = PartitionCandidates.Performance.PPV.Value;
                  break;
              case ACCURACY:
                  CandidatePerformance = PartitionCandidates.Performance.Accuracy.Value;
                  break;
              case BALANCEDACCURACY:
                  CandidatePerformance = PartitionCandidates.Performance.BalancedAccuracy.Value;
                  break;
              case F1SCORE:
                  CandidatePerformance = PartitionCandidates.Performance.F1score.Value;
                  break;
          }
          Rule.TestRule(Initialised, Constraints, CandidatePerformance, MaximizeMeasure, RestrictionSet, RuleOutputMethod, IsPrintPerformance, IsPrintSets);
          PartitionCandidates = Rule.SaveCandidate(MaximizeMeasure, RestrictionSet);

           if (IsPrintCutoffSets) {
          Rule.PrintCutoffSet();
        }
        if (IsUpdateRealtime) {
         CalculateProgress();
        }
#ifndef COMMANDVERSION
		// BreatheCount++;                                                         // Increment breathe counter
		if (BreatheCount>BREATHE_INTERVAL) {
		  if (PauseFunction()) {                                                // User paused the project
			PrintSummary();
			return false;
		  }
		  if (CancelFunction()) {                                               // User cancelled the project
			PrintSummary();
			CloseFunction();
			return false;
		  }
		  BreatheCount = 0;
		}
#endif
	  } while (Rule.NextCutoffSetGenerator()); // NextCutoffSetRestriction
    } while (Rule.NextFeatureSetGenerator(0, Rule.GetFeatureOperatorSize())); // NextFeatureSetRestriction
	return RunProject();
  } else {
    do {
	  CalculateProgress();
      do {
       // TestRule();                                                             // Calculate performance of current rule

          float CandidatePerformance;
          switch (MaximizeMeasure) {
              case SENSITIVITY:
                  CandidatePerformance = PartitionCandidates.Performance.Sensitivity.Value;
                  break;
              case SPECIFICITY:
                  CandidatePerformance = PartitionCandidates.Performance.Specificity.Value;
                  break;
              case NPV:
                  CandidatePerformance = PartitionCandidates.Performance.NPV.Value;
                  break;
              case PPV:
                  CandidatePerformance = PartitionCandidates.Performance.PPV.Value;
                  break;
              case ACCURACY:
                  CandidatePerformance = PartitionCandidates.Performance.Accuracy.Value;
                  break;
              case BALANCEDACCURACY:
                  CandidatePerformance = PartitionCandidates.Performance.BalancedAccuracy.Value;
                  break;
              case F1SCORE:
                  CandidatePerformance = PartitionCandidates.Performance.F1score.Value;
                  break;
          }

          Rule.TestRule(Initialised, Constraints, CandidatePerformance, MaximizeMeasure, RestrictionSet, RuleOutputMethod, IsPrintPerformance, IsPrintSets);
          PartitionCandidates = Rule.SaveCandidate(MaximizeMeasure, RestrictionSet);
          if (IsPrintCutoffSets) {
          Rule.PrintCutoffSet();
        }
        if (IsUpdateRealtime) {
		  CalculateProgress();
        }
 #ifndef COMMANDVERSION
		// BreatheCount++;                                                         // Increment breathe counter
		if (BreatheCount>BREATHE_INTERVAL) {
		  if (PauseFunction()) {                                                // User paused the project
			PrintSummary();
			return false;
		  }
		  if (CancelFunction()) {                                               // User cancelled the project
			PrintSummary();
			CloseFunction();
			return false;
		  }
		  BreatheCount = 0;
		}
#endif
      } while (Rule.NextCutoffSet()); // TODO: check if should be generator
    } while (Rule.NextFeatureSet(0, Rule.GetFeatureOperatorSize())); // TODO: check if should be generator
    return RunProject();
  }
}

/**********************************************************************
Function: ManualRunProject()
Category: Modifiers
Scope: public
In: string, start rule
Out: bool, project finished or not
Description: Manually Starts a project from a given starting point and
returns the bestrule and performance
**********************************************************************/
//bool Explore::ManualRunProject(string StartString, string StopString) {
//  ManualContinue = true;
//  ManualStop = false;
//
//  LastFeatureSet = "";
//  RulesProcessed = 0;
//  FeatureSetsProcessed = 0;
//
//  #if defined(EXPLORE_MPI_DEBUG)
//  cout << "--> Running project from (" << StartString << ") to (" << StopString << ")" << endl;
//  #endif
//
//  if (!StartRule.Interpret(StartString)) return false;                          // Interpret and set the start-rule
//  if (!StopRule.Interpret(StopString)) return false;                            // Interpret and set the stop-rule
//  if (!Rule.SetRule(StartRule)) return false;                                   // Initialise the rule with the start-rule
//
//  #if defined(EXPLORE_MPI_DEBUG)
//  cout << "--> Started: ";
//  Rule.PrintFeatureSet();
//  cout << "***************************************************************" << endl;
//  #endif
//
//  #if defined(BRANCH_AND_BOUND_DEBUG)
//  int PreviousCT = 0;
//  int PreviousCP = 0;
//  int PreviousFP = 0;
//  #endif
//
//  do {                                                                          // Run project
//    if (IsPrintCombinations) {
//      Rule.PrintCombination();
//    }
//
//    #if defined(EXPLORE_MPI_DEBUG)
//    cout << "--> New Combination: ";
//    Rule.PrintCombination();
//    #endif
//
//    do {
//
//      if (!Rule.IsFeatureSetGenerated()) {
//        Rule.NextFeatureSetGenerator(0, Rule.GetFeatureOperatorSize());
//      }
//
//      if (IsPrintFeatureSets) {
//        Rule.PrintFeatureSet();
//      }
//      do {
//
//        if (Rule.IsCutoffSetGenerated()) {
//
//          if (IsPrintCutoffSets) {
//            Rule.PrintCutoffSet();
//          }
//
//          // Test current rule (Branch-and-bound values could be updated)
//          // TestRule();
//
//            float CandidatePerformance;
//            if (PartitionCandidates.IsValid()) {
//                CandidatePerformance = PartitionCandidates.Performance.Accuracy.Value;
//            }
//
//            Rule.TestRule(Initialised, Constraints, CandidatePerformance, MaximizeMeasure, RestrictionSet, RuleOutputMethod, IsPrintPerformance, IsPrintSets);
//            PartitionCandidates = Rule.SaveCandidate(PartitionCandidates, MaximizeMeasure, RestrictionSet);
//        }
//      } while (Rule.NextCutoffSetGenerator());                                           // Create next cutoff-set
//
//      if (CheckStopFeature()) {
//        ManualStop = true;
//        #if defined(EXPONENTIAL_WORKUNITS)
//        RulesProcessed += (Rule.FeatureSetComplexityLimit() / pow(EXPONENTIAL_BASE,(GetNoConjunctions()-1)));
//        #else
//        RulesProcessed += Rule.FeatureSetComplexityLimit();
//        #endif
//      } else {
//        // Save last FeatureSet processed and (maximum) number of rules processed until now
//        LastFeatureSet = GetFeatureSet();
//        NewFeatureSet = true;
//        #if defined(EXPONENTIAL_WORKUNITS)
//        RulesProcessed += (Rule.FeatureSetComplexityLimit() / pow(EXPONENTIAL_BASE,(GetNoConjunctions()-1)));
//        #else
//        RulesProcessed += Rule.FeatureSetComplexityLimit();
//        #endif
//        FeatureSetsProcessed++;
//
//        #if defined(EXPLORE_MPI_DEBUG)
//        cout << "--> FeatureSet processed: " << LastFeatureSet << endl;
//        #endif
//
//        #if defined(BRANCH_AND_BOUND_DEBUG)
//        if ((PreviousCT != Rule.CTBest) || (PreviousCP != Rule.CPBest) || (PreviousFP != Rule.FPConstraint)) {
//          cout << "--> Bound changed: ";
//          cout << "CT (" << Rule.CTBest;
//          cout << ") CP (" << Rule.CPBest;
//          cout << ") FP (" << Rule.FPConstraint;
//          cout << ")" << endl;
//
//          PreviousCT = Rule.CTBest;
//          PreviousCP = Rule.CPBest;
//          PreviousFP = Rule.FPConstraint;
//        }
//        #endif
//
//        if (!Rule.NextFeatureSet(0, Rule.GetFeatureOperatorSize())) ManualContinue = false; // TODO: check if should be generator
//      }
//
//    } while (ManualContinue && !ManualStop);
//
//    if (ManualStop || CheckStopCombination()) {
//      ManualContinue = false;
//    } else {
//      if (Rule.NextCombinationGenerator()) ManualContinue = true;
//      else ManualContinue = false;
//    }
//
//  } while (ManualContinue);
//
//  Final = false; // TODO: AM check
//  ValidateBestCandidate();     // TODO: PR was testbestcandidate checken!!
//
//  #if defined(EXPLORE_MPI_DEBUG)
//  cout << "***************************************************************" << endl;
//  cout << "--> Stopped: ";
//  Rule.PrintFeatureSet();
//  #endif
//
//  return true;
//}

/**********************************************************************
Function: CalculateProgress()
Category: Modifiers
Scope: public
In: -
Out: -
Description: Calculates progress based on FeatureSets generated,
FeatureSets skipped and the complexity of the current project. Calls
ProgressFunction in case progress has increased > PROGRESS_INTERVAL.
**********************************************************************/
void Explore::CalculateProgress() {
#ifdef DEBUG_TIMING
  clock_t Start, End;
  Start = clock();
#endif

/*PR Old progress on FeauturesDone
  long FeatureSetsDone = GetFeatureSetsGenerated();

  if (FeatureSetsDone>0 && ExploreComplexity>0) {
	CurrentProgressCount =  (double)(((double)FeatureSetsDone / (double)ExploreComplexity)*100);
  }
*/

  CurrentProgressCount =  NoPartitionsDone*10;
  if ((CurrentProgressCount - PreviousProgressCount) >= PROGRESS_INTERVAL || IsUpdateRealtime) {
	#ifndef COMMANDVERSION
	ProgressFunction(CurrentProgressCount);
	#endif
	PreviousProgressCount = CurrentProgressCount;
	PrintSummary();
  }

  #ifdef DEBUG_TIMING
  End = clock();
  ExploreTiming.AddTime("EXPLORE::CalculateProgress", Start, End);
  #endif
}

/**********************************************************************
Function: PrintSummary()
Category: Selectors
Scope: public
In: -
Out: -
Description: Prints a summary of the current progress of Explore.
**********************************************************************/
void Explore::PrintSummary() {
#ifdef DEBUG_TIMING
  clock_t Start, End;
  Start = clock();
#endif

  string CurrentStringRule, CurrentStringPerformance, BestStringRule, BestStringPerformance;
  long CombinationsLongDone, FeatureSetsLongDone, CutoffSetsLongDone;
  long CombinationsLongTotal, FeatureSetsLongTotal;
  unsigned int PartitionsDone, PartitionsTotal;
  double RulesTotalLimit;


  if (ProjectCandidates.size() > 0) {
	vector<CANDIDATE>::iterator CurrentBestCandidate(ProjectCandidates.end());
    CurrentBestCandidate--;

    BestStringRule              = (*CurrentBestCandidate).PrintRule();
    BestStringPerformance       = (*CurrentBestCandidate).PrintPerformance(true);
  }

  CurrentStringRule             = Rule.PrintRuleString();
  CurrentStringPerformance      = CurrentPerformance.PrintString(true);

  CombinationsLongDone          = Rule.GetCombinationsGenerated();
  FeatureSetsLongDone           = Rule.GetFeatureSetsGenerated();
  CutoffSetsLongDone            = Rule.GetCutoffSetsGenerated();
  PartitionsDone                = NoPartitionsDone;

  CombinationsLongTotal         = Rule.GetCombinationsTotal();
  FeatureSetsLongTotal          = ExploreComplexity;
  RulesTotalLimit               = Rule.GetTotalComplexityLimit();

  #ifndef COMMANDVERSION
  SummaryFunction(CurrentStringRule,
				  CurrentStringPerformance,
				  BestStringRule,
				  BestStringPerformance,
				  CombinationsLongDone,
				  FeatureSetsLongDone,
				  CutoffSetsLongDone,
				  PartitionsDone,
				  CombinationsLongTotal,
				  FeatureSetsLongTotal,
				  RulesTotalLimit,
				  NoPartitions);
  #endif

  #ifdef COMMANDVERSION
	// printf("Partition %d of %d\n", PartitionsDone, NoPartitions);
	// printf("Best rule: %s \nPerformance: %s\n\n", BestStringRule.c_str(), BestStringPerformance.c_str());
  #endif

#ifdef DEBUG_TIMING
  End = clock();
  ExploreTiming.AddTime("EXPLORE::PrintSummary", Start, End);
#endif
}

/**********************************************************************
Function: PrintRuleString()
Category: Selectors
Scope: public
In: -
Out: -
Description: Print the current rule and returns this as a
string (instead of printing to cout).
**********************************************************************/
string Explore::PrintRuleString() {
  if (Initialised) {
    return Rule.PrintRuleString();
  }
  return "";
}

/**********************************************************************
Function: SavePartitionFile()
Category: Modifiers
Scope: public
In: -
Out: -
Description: Saves a PartitionFile as datafile-nr.learn or test
**********************************************************************/
void Explore::SavePartitionFile(string Output, PARTITION_TYPE Type, unsigned int PartNr) {

   ostringstream Prefix;

   // change file extension
   if (Type==LEARN) {
	  Prefix << "-" << PartNr << ".data";
   }
   else
	 if (Type==VALIDATION) {
	  Prefix << "-" << PartNr << ".test";
	 }
   char* dummy = (char*)ResultFile.c_str();
   string newfilename = str_parse(&dummy,".");
   newfilename += Prefix.str();

   //save to file
   if (PartitionFile.is_open()) {
	PartitionFile.close();
   }
     PartitionFile.open(newfilename.c_str(),ios::out);
	 PartitionFile << Output;
	 PartitionFile.flush();
	 PartitionFile.close();
}


/**********************************************************************
Function: SavePartitions()
Category: Selectors
Scope: public
In: -
Out: -
Description: Saves the partition associated with this
population to a file.
**********************************************************************/
void Explore::SavePartitions(unsigned int Number) {
  // Obtain the partition data of the learn and test set from population
  SavePartitionFile(Population.GetPartitionData(LEARN),LEARN,Number);
  SavePartitionFile(Population.GetPartitionData(VALIDATION),VALIDATION,Number);
}

/**********************************************************************
Function: NextCombination()
Category: Modifiers
Scope: public
In: -
Out: bool, was/wasn't able to create a new combination
Description: Generates the next combination of the rule.
**********************************************************************/
bool Explore::NextCombination() {
  if (Initialised) {
    if (RestrictionSet) {
      return Rule.NextCombinationGenerator(); // Rule.NextCombinationRestriction();
    } else {
      return Rule.NextCombination();
    }
  }
  return false;
}

/**********************************************************************
Function: NextFeatureSet()
Category: Modifiers
Scope: public
In: -
Out: bool, was/wasn't able to create a new featureset
Description: Generates the next featureset of the rule.
**********************************************************************/
bool Explore::NextFeatureSet(int FOperatorNr_start, unsigned int FOperatorNr_end) {
  if (Initialised) {
    if (RestrictionSet) {
      return Rule.NextFeatureSetGenerator(FOperatorNr_start, FOperatorNr_end); // Rule.NextFeatureSetRestriction();
    } else {
      return Rule.NextFeatureSet(FOperatorNr_start, FOperatorNr_end);
    }
  }
  return false;
}

/**********************************************************************
Function: GetCombination()
Category: Selectors
Scope: public
In: -
Out: string, the combination
Description: Returns the current combination of the rule as a string.
**********************************************************************/
string Explore::GetCombination() {
  return Rule.GetCombination();
}

/**********************************************************************
Function: GetFeatureSet()
Category: Selectors
Scope: public
In: -
Out: string, the featureset
Description: Returns the current featureset of the rule as a string.
**********************************************************************/
string Explore::GetFeatureSet() {
  return Rule.GetFeatureSet();
}

/**********************************************************************
Function: IsLastRuleLengthCombination()
Category: Selectors
Scope: public
In: -
Out: bool, it is/is not the last combination for this rulelength
Description: Returns true when the current combination is the last
for the current rulelength being processed.
**********************************************************************/
bool Explore::IsLastRuleLengthCombination() {
  return Rule.IsLastRuleLengthCombination();
}

bool Explore::SetRule(CANDIDATE NewRule) {
  return Rule.SetRule(NewRule);
}

unsigned long Explore::GetFeatureSetComplexityLimit() {
  return Rule.FeatureSetComplexityLimit();
}

bool Explore::CheckStopFeature() {
  return Rule.CheckStopFeature(StopRule);
}

bool Explore::CheckStopCombination() {
  return Rule.CheckStopCombination(StopRule);
}

bool Explore::SetStopRule(string NewStopRule) {
  CANDIDATE StopRuleDummy,FeatureSetDummy;

  StopRuleDummy.Interpret(NewStopRule);
  FeatureSetDummy.Interpret(LastFeatureSet);

  #if defined(EXPLORE_MPI_DEBUG)
  cout << "--> New StopRule " << NewStopRule << endl;
  #endif
  if (ManualContinue && !ManualStop) {
    if (FeatureSetDummy<StopRuleDummy) {
      if (StopRule.Interpret(NewStopRule)) {
        #if defined(EXPLORE_MPI_DEBUG)
        cout << "--> Rules processed: " << RulesProcessed << endl;
        #endif
        return true;
      } else {
        #if defined(EXPLORE_MPI_DEBUG)
        cout << "--> ERROR: Could not interpret StopRule! (StopRule: " << NewStopRule.c_str() << ")" << endl;
        #endif
        ManualStop = true;
      }
    } else {
      #if defined(EXPLORE_MPI_DEBUG)
      cout << "--> WARNING: Already passed StopRule! (StopRule: " << NewStopRule.c_str() << ") (FeatureSet: " << LastFeatureSet.c_str() << ")" << endl;
      #endif
      ManualStop = true;
    }
  } else {
    #if defined(EXPLORE_MPI_DEBUG)
    cout << "--> WARNING: Already stopped task (Explore not running)!" << endl;
    #endif
    ManualStop = true;
  }
  return false;
}

void Explore::SetBranchBoundValues(int CTBest, int CPBest, int FPConstraint) {
  Rule.CTBest = CTBest;
  Rule.CPBest = CPBest;
  Rule.FPConstraint = FPConstraint;
}

void Explore::ResetRule() {
  Rule.Reset();
}

double Explore::GetRulesComplexityLimit() {
  if (Initialised) {
    return Rule.GetTotalComplexityLimit();
  }
  return 0;
}

double Explore::GetFeatureSetComplexity() {
  if (Initialised) {
    return ExploreComplexity;
  }
  return 0;
}

double Explore::GetExponentialComplexityLimit() {
  if (Initialised) {
    return Rule.GetExponentialComplexityLimit();
  }
  return 0;
}

vector<CANDIDATE> *Explore::GetProjectCandidates() {
  if (Initialised) {
    return &ProjectCandidates;
  }
  return NULL;
}

CANDIDATE *Explore::GetPartitionCandidates() {
  if (Initialised) {
    return &PartitionCandidates;
  }
  return NULL;
}

int *Explore::GetCTBest() {
  return &Rule.CTBest;
}

int *Explore::GetCPBest() {
  return &Rule.CPBest;
}

int *Explore::GetFPConstraint() {
  return &Rule.FPConstraint;
}

string *Explore::GetLastFeatureSet() {
  return &LastFeatureSet;
}

double *Explore::GetRulesProcessed() {
  return &RulesProcessed;
}

double *Explore::GetFeatureSetsProcessed() {
  return &FeatureSetsProcessed;
}
