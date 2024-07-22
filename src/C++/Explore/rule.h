//----------------------------------------------------------------------------//
// $Header$
// $Author$
// $DateTime$
// $Change$
//----------------------------------------------------------------------------//

#ifndef RULE_H
#define RULE_H

#include "initialise.h"
#include "candidate.h"
#include "conjunction.h"
#include "roc.h"
#include "mandatory.h"
#include <tbb/concurrent_vector.h>

class RULE {
  private:
    // Private variables
    unsigned int MaxLength;                                                     // Maximum length of the rule
    unsigned int MinLength;
    unsigned int CurrentLength;                                                 // Current length of the rule
    bool MandatorySet;
    bool RestrictionSet;
    
    PARTITION_TYPE TestMode;
    
    unsigned int NoFeatures{};                                                    // Number of features

    vector<CONJUNCTION> Conjunctions;                                           // Conjunctions which make up a rule
    vector<FEATURE>* Features{};                                                  // Pointer to a list of features

    vector<CONDITION> FeatureOperators;                                         // List of FeatureOperators per Feature

    bool CombinationGenerated;
    bool FeatureSetGenerated;
    bool CutoffSetGenerated;

    bool LastCombination;                                                       // Is this the last partition?
    bool LastFeatureSet{};                                                        // Is this the last featureset?
    bool LastConditionSet{};                                                      // Is this the last instance?

    bool BranchBound;                                                           // Should optimization be used when generating rules
    bool BinaryReduction;

    unsigned int CombinationsGenerated;                                         // Number of combinations generated until now
    long FeatureSetsGenerated;                                                  // Number of featuresets generated until now
    long CutoffSetsGenerated;                                                   // Number of conditionsets generated until now

    long CountCandidates;                                                   // Number of candidate rules found until now

    unsigned int CombinationsTotal{};
    double TotalComplexityLimit{};                                                // Total number of rules without branch and bound for this partition
    double TotalExponentialLimit{};

    // Private selectors
    long double Faculty(long double a);                                // Calculates the faculty
    double Binomial(unsigned int a, unsigned int b);                            // Calculates the binomial
    double ConjunctionComplexity(unsigned int PartitionSize,                    // Calculates the complexity of a conjunction
                                 unsigned int NoFeatures,
                                 unsigned int NoPartitions);
                                 
    PERFORMANCE GetPerformance();                                               // Basically retrieves all relevant performance variables from a ruleset and puts them in ruleperformance (compatibility with candidates)

    // Private modifiers
    void AddConjunction(unsigned int CSize);                                    // Add conjunction of size RLength to end of rule (push_back)
    void RemoveConjunction();                                                   // Remove last conjunction of rule (pop back)

    void ClearConjunctions();                                                   // Remove all conditions associated with every conjunction

    void SetCurrentRuleLength(unsigned int RRuleLength);                        // Change the current rulelength

    bool InterpretRule(string StringRule, CANDIDATE& Dummy);                    // Interpret rule into dummy

    void CreateFeatureOperators();                                              // Create all possible conditions for this rule

    void InitialiseRanges();                                                    // Implement ranges for each feature
    void InitialiseMandatory();                                                 // See whether a feature is mandatory
    void InitialiseROC();                                                       // Initialise ROC curves for each rulelength

    // Needed for restrictions
    bool MatchConjunctionFeatureSet(unsigned int RestrictionIndex, unsigned int ConjunctionNumber);
    bool MatchConjunctionCutoffSet(unsigned int RestrictionIndex, unsigned int ConjunctionNumber);

    bool IsPrintCombinations{};
    bool IsPrintFeatureSets{};
    bool IsPrintCutoffSets{};
    bool IsPrintCutoffSetsBestLength{};

    vector<ROC> ROCCurves;

//------------------------------------------------------------------------------

unsigned int NoFeatureOperators{};                                            // Easy access to total (real total - 1)!!!

//------------------------------------------------------------------------------

    CANDIDATE Restriction;                                                      // Restriction for the rule

    PERFORMANCE_MEASURE* MaximizeMeasure{};

    // Mathematical functions from 'Numerical Recipes'
    float gammln(float xx);
    float factrl(int n);
    float factln(int n);
    float bico(int n, int k);

  public:

    SET RuleSet;                                                                // A SET for calculating rule-performance
    
    int CPBest;                                                                 // Best CorrectPositive until now (Sensitivity optimisation)
    int CTBest;                                                                 // Best CorrectPositive + CorrectNegative until now (Accuracy optimisation)

    int FPConstraint;                                                           // Specificity optimisation
    bool FPCPOptimization{};

    //--------------------------------------------------------------------------

    // Constructors and destructors
    RULE();                                                                     // Default constructor
    ~RULE();                                                                    // Default destructor

    // Selectors
    unsigned int Size();                                                        // Get size of rule (number of subsets)
    double CombinationComplexity();                                             // Calculate the number of featuresets that can be generated in this combination (without optimization)
    double RuleComplexity(unsigned int NoPartitions);                           // Calculate the total number of featuresets that can be generated (without optimization)
    int CalculateCombinationsTotal();
    double TimeComplexity(unsigned int NoPartitions);
    double FeatureSetComplexityLimit();                                          // Calculate the total number of rule in a featureset
    unsigned int GetCurrentRuleLength();                                        // Get the current rule length
    unsigned int GetMaxRuleLength();                                            // Get the maximum rule length
    unsigned int GetMinRuleLength();
    bool IsCombinationGenerated();                                              // Get CombinationGenerated
    bool IsFeatureSetGenerated();                                               // Get FeatureSetGenerated
    bool IsCutoffSetGenerated();                                                // Get CutoffSetGenerated

    vector<double>* GetROC(unsigned int RRuleLength);                           // Get an ROC Curve for a specific rulelength

    string GetCombination();                                                    // Returns current combination as string
    string GetFeatureSet();                                                     // Returns current featureset as string
    bool IsLastRuleLengthCombination();                                         // Returns true if the current combination is the last for the rulelength currently being processed

    vector<string> GetFeatureNames();                                           // Get a list of featurenames
    vector<unsigned int> GetConjunctions();                                     // Get a list of conjunctions
    vector<unsigned int> GetFeatures();                                         // Get a list of features
    vector<OPERATOR> GetOperators();                                            // Get a list of operators
    vector<string> GetCutoffs();                                                 // Get a list of cutoffs

    unsigned int GetMinCutoff(unsigned int FOperator, int ConjunctionNr);
    bool CutoffsAtMax(int ConjunctionNr, int ConditionNr);
    unsigned int GetMaxCutoff(unsigned int FOperator);

    unsigned int GetCombinationsGenerated();                                    // Get the number of combinations generated
    long GetFeatureSetsGenerated();                                             // Get the number of featuresets generated
    long GetCutoffSetsGenerated();                                              // Get the number of conditionsets generated

    long GetCountCandidates();                                                  // Get the number of candidate rules found until now

    unsigned int GetCombinationsTotal();                                        // Get total amount of combinations
    double GetTotalComplexityLimit();                                           // Get limit of number of rules
    double GetExponentialComplexityLimit();                                     // Returns a weighted limit of rules

    PARTITION_TYPE GetTestMode();
    // Restriction and mandatory checking
    bool IsMandatorySet();                                                      // Check whether a mandatory feature is set
    bool CheckFeatureSetMandatory();                                            // Check whether rule contains all mandatory features
    bool CheckCombinationRestriction();                                         // Check whether the current combination fits the restriction
    bool CheckStopFeature(CANDIDATE StopRule);                                  // Check whether the stop featureoperator set has been reached
    bool CheckStopCombination(CANDIDATE StopRule);                              // Check whether the stop combination has been reached
    bool CheckFeatureSetRestriction();                                          // Check whether the current featureset fits the restriction
    bool CheckCutoffSetRestriction();                                           // Check whether the current conditionset fits the restriction

    // Output
    void SetPrintCombinations(bool Setting);
    void SetPrintFeatureSets(bool Setting);
    void SetPrintCutoffSets(bool Setting);
    void SetPrintCutoffSetsBestLength(bool Setting);

    void PrintSettings();
    void PrintCombination();                                                    // Print partition information of the rule
    void PrintFeatureSet();                                                     // Print featureset information of the rule
    void PrintFeatureSet_Thread();
    void PrintCutoffSet();                                                      // Print rule structure
    void PrintPerformance();                                                    // Print the performance
    void PrintRestriction();                                                     // Print restrictions
    void PrintCumulativeSets();                                                 // Print cumulative conjunction sets

    string PrintRuleString();                                                   // Prints current rule and returns as string

    void PrintSets();
    void PrintFeatureOperators();

    // Modifiers
    void Initialise(vector<FEATURE>* RFeatures);                                // Initialise the rule (to start generating rules)
    void LimitedInitialise();                                                   // Initialise the rule (without rebuilding a new vector of FeatureOperatorSets)
    void SetRerun(vector<FEATURE>* RFeatures);                                  // Prepare for a new run of project or partition
	void Reset();                                                               // Remove all conjunctions and create a single conjunction of minimum rulelength
	void SetReset(vector<FEATURE>* RFeatures);

    void SetMaxLength(unsigned int RMaxLength);                                 // Set the maximum length of the rule
    void SetMinLength(unsigned int RMinLength);
    void SetNoFeatures(unsigned int RNoFeatures);                               // Set the maximum number of features of the population
    void SetCurrentLength(unsigned int NRuleLength);                            // Set the length of the rule manually
    void SetBranchBound(bool Optimize);                                         // Should we use optimization for generating rules
    void SetBinaryReduction(bool BinaryReduction);

	void GetMaxLength(unsigned int RMaxLength);                                 // Set the maximum length of the rule
	void GetMinLength(unsigned int RMinLength);


    bool NextCombination();                                                     // Generate next combination
    bool NextFeatureSet(int FOperatorNr_start, int FOperatorNr_end);                                                      // Generate next feature set
	bool NextCutoffSet();                                                       // Generate next cutoff set
	bool StartRuleLength(int i);												// Start Rule generation at rulelength i

    bool NextCandidateCutoffSet_Rule(PERFORMANCE_MEASURE Optimize);             // Generate next cutoff set (FPCP optimisation)
    bool NextCandidateCutoffSet_Conjunction(int ConjunctionNo, PERFORMANCE_MEASURE Optimize,
                                            bool Reset);                        // Generate next cutoff set for a conjunction (FPCP optimisation)

    bool NextCombinationGenerator();
    bool NextFeatureSetGenerator(int FOperatorNr_start, int FOperatorNr_end);
    bool NextCutoffSetGenerator();

    PERFORMANCE CalculatePerformance();                                         // Calculate the performance of this rule on the current Test or Learn partition

    bool SetRule(CANDIDATE Candidate);                                          // Set the rule with a candidate
    bool SetRestriction(string StringRule);                                     // Interpret and set the rule from a given string representing a rule
    void RemoveRestriction();                                                   // Remove any restriction

    void SetTestMode(PARTITION_TYPE PType);                                     // Sets partition type that rules should be tested on
    void Prune();
    
    void SortCutoffsByCP();                                                     // Sort cutoffs based on CorrectPositive (Sensitivity)
    void SortCutoffsByCT();                                                     // Sort cutoffs based on CorrectPositive + CorrectNegative (Accuracy)

    void SetFPConstraint(float Specificity);
    void SetCPBest(float Sensitivity);
    void SetCTBest(float Accuracy);
    void SetMaximizeMeasure(PERFORMANCE_MEASURE* Measure);

    void ResetComplexity();
    void ResetCountCandidates();

    bool TestRule(bool Initialised, vector<CONSTRAINT> Constraints,
                  float CandidatePerformance, PERFORMANCE_MEASURE MaximizeMeasure, bool RestrictionSet,
                               RULE_OUTPUT_METHOD RuleOutputMethod, bool IsPrintPerformance, bool IsPrintSets);                                                            // Test current rule
   // TODO: below 3 could be private
    bool CompareConstraints(PERFORMANCE CurrentPerformance, bool Initialised, vector<CONSTRAINT> Constraints);          // Compare current performance with constraints
    bool CompareBestCandidate(PERFORMANCE CurrentPerformance, bool Initialised, float CandidatePerformance, PERFORMANCE_MEASURE MaximizeMeasure);                                                // Compare current performance with best previous performance
    CANDIDATE SaveCandidate(PERFORMANCE_MEASURE MaximizeMeasure, bool RestrictionSet);                                                       // Save the current candidate (current rule + performance)

    int FindBestLength(bool Initialised, CANDIDATE PartitionCandidates, PARTITION_METHOD PartitionMethod,PERFORMANCE_MEASURE MaximizeMeasure);
    CANDIDATE ChooseBestCandidate(unsigned int RuleLength, bool Initialised, CANDIDATE PartitionCandidates, PERFORMANCE_MEASURE MaximizeMeasure);

    unsigned int GetFeatureOperatorSize();
};

#endif
