//----------------------------------------------------------------------------//
// $Header: //depot/Explore/Explore/explore.h#33 $
// $Author: Rijnbeek $
// $DateTime: 2008/09/09 14:16:02 $
// $Change: 87 $
//----------------------------------------------------------------------------//

#ifndef EXPLORE_H
#define EXPLORE_H

#include "initialise.h"
#include "population.h"
#include "rule.h"
//#include "../UIExplore/events.h"
// !!!
//#include "../../MPIExplore/Control.h"                                           // Needed for EXPONENTIAL_COMPLEXITY settings
#include <tbb/concurrent_vector.h>

#include "timing.h"


class Explore {
  private:
    // Properties
//    RULE Rule;                                                                  // Rule(s) of explore
    POPULATION Population;                                                      // Population of explore

    PARTITION_METHOD PartitionMethod;                                           // The partitioning method to be used
    CUTOFF_METHOD CutoffMethod;                                                 // The cutoff method to be used
    OPERATOR_METHOD OperatorMethod;                                             // The operator method to be used

    RULE_OUTPUT_METHOD RuleOutputMethod;                                        // Indicates which rules will be saved by Explore

    float LearnRatio;                                                           // Ratio of population to be used for learning
    unsigned int NoPartitions;                                                  // Number of partitions the population has to be split up in
    unsigned int PositiveClass;                                                 // Class for which a rule has to be created
    unsigned int NoPartitionsDone;
    unsigned int NoRuns;

    bool Initialised;                                                           // Is explore initialised? (dependant on above booleans)
    bool Randomize;                                                             // Use randomization when partitioning? -> default = true
    bool Final;

    PERFORMANCE CurrentPerformance;                                             // Performance of last rule generated

    PERFORMANCE_MEASURE MaximizeMeasure;                                        // Measure which has to be maximized
    vector<CONSTRAINT> Constraints;                                             // Measures to constraint performances

    CANDIDATE PartitionCandidates;                                      // Best rules with corresponding performances for the current partition
    vector<CANDIDATE> ProjectCandidates;                                        // Best rules with corresponding performances for the complete project

    CANDIDATE BestCandidate;                                                    // The best rule and performance
    bool NewCandidate;
    bool NewFeatureSet;

    // ManualRunProject
    CANDIDATE StartRule;
    CANDIDATE StopRule;

    bool ManualContinue;
    bool ManualStop;

    string Name;                                                                // Project name
    string Description;                                                         // Project description
    string DataFileName;                                                        // DataFile used
    string OutputFileName;                                                      // OutputFile
    bool IncrementalOutputFile;
    
    string LastFeatureSet;                                                      // Last FeatureSet processed by Explore
    double RulesProcessed;                                                      // Number of rules processed
    double FeatureSetsProcessed;                                                // Number of FeatureSets processed

    // Internal Settings
    bool NoClassesSet;                                                          // Is the number of classes set?
    bool PartitionMethodSet;                                                    // Is partition method set?
    bool NoPartitionsSet;                                                       // Has number of partitions been set?
    bool LearnTestRatioSet;                                                     // Has learn/test ratio been set?
    bool CutoffMethodSet;                                                       // Is feature cutoff method set?
    bool OperatorMethodSet;                                                     // Is feature operator method set?
    bool PositiveClassSet;                                                      // For which class do we need to find a rule?
    bool RuleOutputMethodSet;                                                   // Which way do good rules have to be saved?
    bool MaximizeMeasureSet;                                                    // Have any constraints been to set for which performance has to be maximised?
    bool RestrictionSet;                                                        // Has a restriction been set?
    bool MandatorySet;                                                          // Has a mandatory feature been set?

	bool BranchBound;                                                           // Optimize rule generation process?
	bool SubSumption;                                                           // Use SubSumption Pruning to remove cutoffs
    bool Parallel;                                                              // Use parallelization
    PARALLEL_METHOD ParallelMethod;                                             // Method of parallelization
    bool IsUpdateRealtime;                                                      // Print summary information in real-time
    bool BinaryReduction;

	TIMING RuleLengthTiming;
	clock_t StartRuleLength;
	string RestrictionString;

    // Internal complexity/progress settings
    double PreviousProgressCount;                                               // Previous number of featuresets generated
    double CurrentProgressCount;                                                // Current number of featuresets generated

    long ExploreComplexity;                                                     // Total number of featuresets

    // Output settings
    bool IsPrintSettings;                                                       // Print settings to output
    bool IsPrintPartitions;                                                     // Print population to output
    bool IsSavePartitions;                                                      // Print population to output
    bool IsPrintCombinations;                                                   // Print combinations to output
    bool IsPrintFeatureSets;                                                    // Print featuresets to output
    bool IsPrintCutoffSets;                                                     // Print conditionsets to output
    bool IsPrintPerformance;                                                    // Print performance to output
    bool IsPrintSets;                                                           // Print sets to output
    bool IsPrintOperatorMethod;                                                 // Print operator-method information to output
    bool IsPrintCutoffMethod;                                                   // Print cutoff-method information to output
    bool IsPrintOperatorValues;                                                 // Print operator values to output
    bool IsPrintCutoffValues;                                                   // Print cutoff values to output
    bool IsPrintFeatureOperators;                                               // Should FeatureOperators be printed at the start of rule generation?

    // Private modifiers
    //  bool ChooseBestCandidate(unsigned int RuleLength);

    // Output functions
    void PrintSettings();                                                       // Print settings (debug settings)
    void PrintRule();                                                           // Print the current rule (debug rule)
    string PrintRuleString();
    void PrintBestCandidate();                                                  // Prints the best rule with its performance (the best candidate)
    void PrintCutoffs();                                                        // Print cutoffs for all features (debug cutoffs)
    void PrintOperators();                                                      // Print operators for all features (debug operators)
    void PrintPartitions();                                                     // Print partition information (debug population)
	void SavePartitions(unsigned int Number);                                   // Print partition information (debug population)
	void SavePartitionFile(string Output, PARTITION_TYPE Type, unsigned int PartNr); //Save partitions to file
	void PrintPerformance();                                                    // Print performance of current rule (debug performance)
	void PrintSets();                                                           // Print individual set observations (debug performance)
	void PrintConstraints();                                                    // Print the maximizing feature and all constraints
    void PrintFeatureOperators();                                               // Prints all FeatureOperators of the rule

    void SetRerun();                                                            // Set project for rerun of project or partition
	bool RunProject();                                                          // Run a new project (start rule-generation)
	// int  Stop();                                                                // Stop criterium for internal testing
	// int  FindBestLength();                                                          // Return the best length on internal testing
	void Induce(int nStart, int nEnd);											// Induce rule on learn parition form start tot end
    bool ResumeProject();

	void CalculateProgress();                                                   // Calculates progress of current project
	void PrintSummary();                                                        // Prints summary information to summaryfunction

    #ifdef DEBUG_EXPLORE
    TIMING ExploreTiming;
    #endif

  public:

    RULE Rule;                                                                  // Rule(s) of explore

    void PrintCombination();                                                    // Print all combinations of the rule
    void PrintFeatureSet();                                                     // Print all featuresets of the rule
    void PrintCutoffSet();                                                      // Print all conditionsets of the rule
  
    // Constructors and destructors
    Explore();                                                                  // Constructor
    ~Explore();                                                                 // Destructor

    // Main functions
    bool CheckSettings();                                                       // Check explore settings
    bool Initialise();                                                          // Initialise Explore
    bool LimitedInitialise();                                                   // Initialise Explore without rebuilding vector of FeatureOperatorSets    
    void InitialiseCPFP();                                                      // Initialise CPFP Method
    void Uninitialise();                                                        // Uninitialise Explore

    double RuleComplexity();                                                    // Return complexity for the entire rule
    double CombinationComplexity();                                             // Return complexity for the current rulelength
    double FeatureSetComplexityLimit();                                         // Return upper-limit complexity for the current featureset    

    // Selectors
    unsigned int GetCurrentRuleLength();                                        // Get the current rulelength
    unsigned int GetMaxRuleLength();                                            // Get maximum rulelength
    unsigned int GetMinRuleLength();                                            // Get minimum rulelength

    unsigned int GetNoConjunctions();                                           // Get the current number of subsets in the rule

    unsigned int GetNoFeatures();                                               // Get the number of features in the population
    unsigned int GetNoObservations();                                           // Get the number of observations in the population
	int GetClassValueByName(string ClassValue);                        // returns the index of a class value, -1 is error
	int GetFeatureNumber(string FeatureName);                          // returns the index of a feature by name, -1 is error
	unsigned int GetNoClasses();                                                // Get the number of classes in the population
	list<string>* GetClassNames();                                      // Get names of the classes in population
	vector<Range>* GetCutoffRanges(unsigned int FeatureNumber);                 // Get a cutoff range for a feature
    OPERATOR GetOperator(unsigned int FeatureNumber);                           // Get the operator(s) associated with a particular feature
    bool IsContinuous(unsigned int FeatureNumber);
    string GetName();                                                           // Returns the name of the project
    string GetDescription();                                                    // Returns the description of the project
    PARTITION_METHOD GetPartitionMethod();                                      // Returns the partition-method of the project
	bool GetRandomize();                                                        // Returns whether the partition-observations have been randomized
    unsigned int GetNoPartitions();                                             // Returns the number of partitions in the population
    float GetLearnRatio();                                                      // Returns the learn ratio of the population
    OPERATOR_METHOD GetOperatorMethod();                                        // Returns the operator method used
    CUTOFF_METHOD GetCutoffMethod();                                            // Returns the cutoff method used
    unsigned int GetPositiveClass();                                            // Returns the positive class of the population
    string GetRestriction();                                                    // Returns the restriction set
    PERFORMANCE_MEASURE GetMaximizeMeasure();                                   // Returns the measure that is/will be maximized
    vector<CONSTRAINT> GetConstraints();                                        // Returns any constraints set
    RULE_OUTPUT_METHOD GetRuleOutputMethod();                                   // Returns the output method of rules for this project
    bool GetBranchBound();                                                      // Returns whether Explore will optimize rules when it has the chance
    bool GetSubSumption();                                                      // Returns whether Explore will use subsumption
    bool GetParallel();                                                      // Returns whether Explore will use parallelization
    PARALLEL_METHOD GetParallelMethod();                                     // Returns method of parallelization
    bool GetBinaryReduction();

    bool GetPrintSettings();                                                    // Should settings be printed to output
    bool GetPrintPartitions();                                                  // Should population be printed to output
    bool GetPrintCombinations();                                                // Should combinations be printed to output
    bool GetPrintFeatureSets();                                                 // Should featuresets be printed to output
    bool GetPrintCutoffSets();                                                  // Should conditionsets be printed to output
    bool GetPrintPerformance();                                                 // Should performance be printed to output
	bool GetPrintSets();                                                        // Should sets be printed to output
	bool GetPrintOperatorMethod();                                              // Should operator-method information be printed to output
    bool GetPrintCutoffMethod();                                                // Should cutoff-method information be printed to output
    bool GetPrintOperatorValues();                                              // Should operator values be printed to output
    bool GetPrintCutoffValues();                                                // Should cutoff values be printed to output
    bool GetPrintFeatureOperators();                                            // Should FeatureOperators with their corresponding cutoff values be printed to output
    bool GetSavePartitions();                                                   // Should partitions be saved to separate files after partitioning the population?

    bool IsCombinationGenerated();
    bool IsFeatureSetGenerated();
    bool IsCutoffSetGenerated();


    CANDIDATE GetProjectCandidate(unsigned int COrder);                         // Get a candidate from the project list
    // CANDIDATE GetPartitionCandidate(unsigned int COrder);                       // Get a candidate from the partition list
    unsigned int GetNoProjectCandidates();                                      // Get the number of current best-performing rules of the project list
    unsigned int GetNoPartitionCandidates();                                    // Get the number of current best-performing rules of the partition list

	void ValidateBestCandidate();                                               // Calculate performance best candidate on VALIDATION partition
	CANDIDATE GetBestCandidate();                                               // Returns the best rule with its performance (the best candidate)
    void SummarisePerformance();                                                // Calculate minimum, maximum and average values for MAXIMIZE_MEASURE based on candidates

    unsigned int GetCombinationsGenerated();                                    // Return the number of combinations generated until now
    long GetFeatureSetsGenerated();                                             // Return the number of featuresets generated until now
    long GetCutoffSetsGenerated();                                              // Return the number of conditionsets generated until now

    bool SetRestriction(string StringRule);                                     // Set the rule from a String representing a rule
    void RemoveRestriction();                                                   // Remove any restriction on the rule

    void ClearCandidates();                                                     // Clear all candidates
    void ClearConstraints();                                                    // Clear all constraints
    void ClearPopulation();                                                     // Clear all features/classnames/ratio's etc of the population

    list<string> GetCutoffValues(unsigned int FeatureNumber);                    // Get all cutoffs for a specific feature as a list of floats (quicker)

    vector<double>* GetROC(unsigned int RuleLength);                            // Get an ROC Curve for a given rulelength

    // Output
    void SetUpdateRealtime(bool Setting);                                       // Print summary/progress in realtime
    void SetPrintSettings(bool Setting);                                        // Print settings to output
    void SetPrintPartitions(bool Setting);                                      // Print population to output
    void SetSavePartitions(bool Setting);                                       // Print combinations to output
    void SetPrintCombinations(bool Setting);                                    // Print combinations to output
    void SetPrintFeatureSets(bool Setting);                                     // Print featuresets to output
    void SetPrintCutoffSets(bool Setting);                                      // Print conditionsets to output
    void SetPrintPerformance(bool Setting);                                     // Print performance to output
    void SetPrintSets(bool Setting);                                            // Print sets to output
    void SetPrintOperatorMethod(bool Setting);                                  // Print operator-method information to output
    void SetPrintCutoffMethod(bool Setting);                                    // Print cutoff-method information to output
    void SetPrintOperatorValues(bool Setting);                                  // Print operator values to output
    void SetPrintCutoffValues(bool Setting);                                    // Print cutoff values to output
    void SetPrintFeatureOperators(bool Setting);                                // Print FeatureOperators with their corresponding cutoff values to output

    // Modifiers
    void SetName(string ProjectName);                                           // Set Explore project name
    void SetDescription(string ProjectDescription);                             // Set Explore project description

    void InitialiseOutput();                                                    // Initialise explore (read population, partition, etc)
    bool Partition();                                                           // Partition the population
    void ClearPartitions();                                                     // Remove all partitions without deleting any observations

    
    bool SetRuleOutputMethod(RULE_OUTPUT_METHOD ROutputMethod);                 // Which ways do good performing rules have to be saved?
    bool SetRandomize(bool Random);                                             // Randomize the partitions of the population or not?
    long double GetSeed();                                                      // Get the seed used to randomize the population
    bool SetSeed(long double Seed);                                             // Set the seed manually for randomizing the population
	void ResetSeed();															// Resets the SeedIsSet boolean in population

    void SetMandatoryFeature(unsigned int FeatureNumber, bool IsMandatory);     // Make a feature mandatory/or not.
    bool GetMandatoryFeature(unsigned int FeatureNumber);                       // Get a mandatory feature.

    bool SetConstraint(PERFORMANCE_MEASURE PMeasure, float PValue);             // Set a constraint-rule (this rule must always be part of each rule generated by Explore
    bool SetMaximizeMeasure(PERFORMANCE_MEASURE PMeasure);                      // Set the measure that has to be maximized by explore
    void SetCPBest(float Sensitivity);                                          // Set CPBest
    void SetCTBest(float Accuracy);                                             // Set CTBest
	bool AddFeature(string FeatureName, unsigned int Nom);                      // Add a feature to the population
	void AddObservation(list<string> Values, unsigned int Class);               // Add an observation to the population

	bool SetPartitionMethod(const PARTITION_METHOD PMethod);                    // Partitioning method to be used
    bool SetCutoffMethod(const CUTOFF_METHOD CMethod);                          // Cutoff method to be used
    bool SetOperatorMethod(const OPERATOR_METHOD OMethod);                      // Operator method to be used

    bool SetLearnRatio(const float PLearnRatio);                                // In case of holdout method, percentage of population used for learning
    bool SetNoPartitions(const unsigned int PNoPartitions);                     // In case of random subsampling or cross-validation, number of partitions to cut up population in
	bool SetPositiveClass(unsigned int Feature, unsigned int Class);            // Set the class a classifier has to be created for by index
    void SetClassNames(list<string> NClassNames);                               // Set the class-names as a list of strings
	void SetClassNamesByFName(string ClassFName);                               // Set the classnames of the population

    bool SetMaxRuleLength(const unsigned int ERuleLength);                      // Maximum rule length to be generated
    bool SetMinRuleLength(const unsigned int ERuleLength);                      // Maximum rule length to be generated
    
	bool AddCutoff(unsigned int FeatureNumber, string CutoffValue);              // Manually add a cutoff to a feature
	bool RemoveCutoff(unsigned int FeatureNumber, unsigned int CutoffNumber);   // Manually remove a cutoff by number
    bool RemoveCutoff(unsigned int FeatureNumber, string CutoffValue);           // Manually remove a cutoff by value
    bool RemoveCutoffRange(unsigned int FeatureNumber);
    bool RemoveFeatureCutoffs(unsigned int FeatureNumber);
    void RemoveAllCutoffs();        
    bool SetOperator(unsigned int FeatureNumber, OPERATOR FeatureOperator);     // Manually set the operator for a feature

    void AddCutoffRange(unsigned int FeatureNumber, Range NewRange);            // Set a cutoff-range for a specific feature

    void FindCutoffs();                                                         // Find cutoffs for all features within the population
    void FindOperators();                                                       // Find operators for all features within the population

	void SetBranchBound(bool Optimize);                                         // Optimize on accuracy/sensitivity or specificity depending on constraints and maximize measure
	void SetSubSumption(bool Value);                                            // Prune Cutoffs by SubSumption
    void SetParallel(bool Value);                                               // Use parallelization
    void SetParallelMethod(PARALLEL_METHOD Value);                              // Set method for parallelization
    void SetBinaryReduction(bool Value);

    // Interfacing
    void   SetOutput(streambuf *OutputBuffer);                                  // Redirect explore output
    void   Start();                                                             // Start explore rule-generation
    void   Resume();                                                            // Resume explore rule-generation
    void   Reset();                                                             // Reset explore rule-generation
    
	// Function Handlers
	#ifndef COMMANDVERSION
    std::function<void(double Progress)> ProgressFunction;

    std::function<void(string CurrentRule,  string CurrentPerformance,  // Print summary information
                       string BestRule,
                       string BestPerformance,
                       long CombinationsDone,
                       long FeatureSetsDone,
                       long ConditionSetsDone,
                       unsigned int PartitionsDone,
                       long CombinationsTotal,
                       long FeatureSetsTotal,
                       double RulesTotalLimit,
                       unsigned int PartitionsTotal)> SummaryFunction;

    std::function<bool()> CancelFunction;                                         // Cancel function-handler
    std::function<bool()> PauseFunction;                                          // Pause function-handler
    std::function<void()> CloseFunction;                                          // Close function-handler
    std::function<void(string Output, PARTITION_TYPE type, unsigned int PartNr)> SavePartitionFileFunction;
    #endif

	// Selectors
	string GetCombination();                                                    // Returns the current Combination as a string
	string GetFeatureSet();                                                     // Returns the current FeatureSet as a string
	bool IsLastRuleLengthCombination();                                         // Is this the fore-last combination for this rulelength?

	// Control
	bool NextCombination();                                                     // Creates the next Combination
	bool NextFeatureSet(int FOperatorNr_start, unsigned int FOperatorNr_end);                                                      // Creates the next FeatureSet

    bool SetRule(CANDIDATE NewRule);
    void SetBranchBoundValues(int CTBest, int CPBest, int FPConstraint);
	void ResetRule();

    // bool ManualRunProject(string StartString, string StopString);               // Run a project from a given start to a given end
    bool CheckStopFeature();
    bool CheckStopCombination();
    bool SetStopRule(string NewStopRule);

    // Complexity/Rule-space
    double GetRulesComplexityLimit();
    double GetFeatureSetComplexity();
    double GetExponentialComplexityLimit();
    unsigned long GetFeatureSetComplexityLimit();

    // Exchanging information
    vector<CANDIDATE> *GetProjectCandidates();
    CANDIDATE *GetPartitionCandidates();

    int *GetCTBest();
    int *GetCPBest();
    int *GetFPConstraint();

    string *GetLastFeatureSet();
    double *GetRulesProcessed();
    double *GetFeatureSetsProcessed();
	fstream PartitionFile;
 	string ResultFile;

};
#endif
