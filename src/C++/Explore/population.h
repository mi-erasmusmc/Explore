//----------------------------------------------------------------------------//
// $Header: //depot/Explore/Explore/population.h#18 $
// $Author: Rijnbeek $
// $DateTime: 2008/09/09 14:16:02 $
// $Change: 87 $
//----------------------------------------------------------------------------//

#ifndef POPULATION_H
#define POPULATION_H

#include "initialise.h"
#include "feature.h"
#include "partition.h"

class POPULATION {
  private:
    unsigned int NoObservations;                                                // Number of observations
    float* LearnRatio;                                                          // Percentage of observations used to learn (in case of holdout method)

    time_t Seed;                                                                // Needed for random number generator
    bool SeedIsSet;

    unsigned int* NoPartitions;                                                 // Number of partitions (in case of cross-validation)
	unsigned int* PositiveClass;                                                // Class for which a classifier should be constructed
	unsigned int* ClassFeature;													// Feature that is used as Class
    unsigned int NoClasses;                                                     // Number of classes in the population
    unsigned int PartitionNumber;                                               // Current partition being used to test a rule (in case of more than one testing partition)
    bool* Randomize;                                                            // Randomize the partitions?
    bool IsPartitioned;                                                         // Has the population been partitioned before?

    unsigned int CurrentPartition;                                              // Current partition being tested

    vector<FEATURE> Features;                                                   // Features of this population
    vector<PARTITION> Partitions;                                               // Actual partitions

    PARTITION_METHOD* PartitionMethod;                                          // Partitioning method
    CUTOFF_METHOD* CutoffMethod;                                                // Method used to find cutoffs
    OPERATOR_METHOD* OperatorMethod;                                            // Method used to find operators

    vector<ClassRatio> ClassRatios;                                             // Vector of classes (size,class,ratio)
    list<string> ClassNames;                                                    // List of class-names

    // Output
    bool IsPrintOperatorMethod;
    bool IsPrintCutoffMethod;

    // Modifiers

	void InitialiseRandomNumber();                                              // Initialise random number generator
    unsigned int GetRandomNumber(unsigned int UpperBound);                      // Get a number between 0 and UpperBound
    void Stratify();                                                            // Calculate stratification info for stratifying classes over partitions

    bool Resubstitute();                                                        // Partition using resubstitution method
    bool Holdout();                                                             // Partition using holdout method
    bool LeaveOneOut();                                                         // Partition using leave-one-out method
	bool CrossValidate();                                                       // Partition using cross-validation method
	void ResetPartitions();
	// Partition modifiers
	void AddPartition(PARTITION_TYPE PType);                                    // Add a partition for this population
	void RemovePartition(unsigned int PNumber);                                 // Remove a partition based on PNumber

	string StratifyOutput;

  public:
	// Constructors and destructors
	POPULATION();
	~POPULATION();

	// Selectors
	unsigned int GetNoFeatures();                                               // Return number of features in this population
	unsigned int GetNoObservations();                                           // Return the number of observations in this population
	unsigned int GetNoCutoffs(unsigned int FeatureNumber);                      // Return the number of cutoffs for a feature in this population
	unsigned int GetNoOperators(unsigned int FeatureNumber);
	unsigned int GetNoPartitions();
	bool IsContinuous(unsigned int FeatureNumber);                              // Return if a feature is continuous
	PARTITION_TYPE GetPartitionType(unsigned int PartitionNumber);
	vector<Range>* GetCutoffRanges(unsigned int FeatureNumber);
	OPERATOR GetOperator(unsigned int FeatureNumber);
	string GetPartitionData(PARTITION_TYPE Type);

	bool OperatorSet(unsigned int FeatureNumber);                               // Returns true if an operator has been set for a specific feature
	vector<FEATURE>* GetFeatures();                                             // Return pointer to vector of features

	unsigned int GetNoClasses();                                                // return number of classes
	int GetClassValueByName(string ClassValue);                        // returns the index of a class value, -1 is error
	int GetFeatureNumber(string FeatureName);                          // returns the index of a feature by name, -1 is error
	list<string>* GetClassNames();                                              // return the class names
	list<string> GetCutoffValues(unsigned int FeatureNumber);

  	void GenerateTestPartition();												// Divide the learning partition in a learn and test (1/3)
	void ResetTestPartitions();                                                  // Set all test partitions back to learn to induce best rule on whole learn set

	// Output
	void PrintSettings();                                                       // Print summary info
	void PrintCutoffs();                                                        // Print cutoffs for all features
	void PrintOperators();                                                      // Print operators for all features
	void PrintPartitions();                                                     // Print partition observations and class distribution
	void PrintMandatoryFeatures();                                              // Print all mandatory features
	void PrintCutoffRanges();                                                   // Print all mandatory features
	void PrintCutoffMethod();
	void PrintOperatorMethod();
	void PrintObservationsPerClass();

	string PrintStratify();                                                     // Print population stratification information

	// Modifiers
	void ResetSeed();															// Resets SeedIsSet boolan
	void SetPositiveClass(unsigned int* PFeature, unsigned int* PClass);        // Set the class a classifier will be found for
	void SetNoClasses(unsigned int PNoClasses);                                 // Set the number of classes in the population
    void SetClassNames(list<string> NClassNames);                               // Set the classnames of the population
	void SetClassNamesByFName(string ClassFName);                                     // Set the classnames of the population
	void InitialiseSets(PARTITION_TYPE PType);                                  // Create sets
    void InitialisePartitions();                                                // Initialise observations of test and learn partitions

	void AddFeature(string FeatureName, CUTOFF_METHOD* CutoffMethod,            // Add feature to the population
					OPERATOR_METHOD* OperatorMethod, bool Continuous);

    void AddObservation(unsigned int FeatureNumber, string Value, unsigned int Class);
                                                                                // Add an observation to a feature
    void SetCutoffMethod(CUTOFF_METHOD* PCutoffMethod);                         // Set cutoff method
    void SetOperatorMethod(OPERATOR_METHOD* POperatorMethod);                   // Set operator method

    void SetPartitionMethod(PARTITION_METHOD* PMethod);                         // Needed for resubstitution or leave-one-out
    void SetLearnRatio(float* PLearnRatio);                                     // Needed for holdout method
    void SetNoPartitions(unsigned int* PNoPartitions);                          // Needed for random subsampling or cross-validation
    void SetRandomize(bool* Random);                                            // Set randomization for the population
    void SetMandatoryFeature(unsigned int FeatureNumber, bool IsMandatory);     // Make a feature mandatory or not.
    bool GetMandatoryFeature(unsigned int FeatureNumber);

    void AddCutoffRange(unsigned int FeatureNumber, Range NewRange);            // Set a cutoff-range for a specific feature

    long double GetSeed();                                                      // Get the seed used to randomize the population
    void SetSeed(long double NewSeed);                                          // Set the seed manually to randomize the population   

    bool Partition();                                                           // Partition observations, or switch to next partition
    void ClearPartitions();                                                     // Clear all partitions of the population (without removing actual features or their observations)

    void SetOperator(unsigned int FeatureNumber, OPERATOR FeatureOperator);     // Manually set an operator for a feature

    void AddCutoff(unsigned int FeatureNumber, string CutoffValue);              // Manually add a cutoff to a feature
    void RemoveCutoff(unsigned int FeatureNumber, unsigned int CutoffNumber);   // Manually remove a cutoff to a feature
    void RemoveCutoff(unsigned int FeatureNumber, string CutoffValue);
    void RemoveCutoffRange(unsigned int FeatureNumber);
    void RemoveFeatureCutoffs(unsigned int FeatureNumber);    

	void FindCutoffs();                                                         // Find cutoffs for each feature
    void FindOperators();                                                       // Find operators for each feature

    void Clear();                                                               // Clear the population

    void SetPrintOperatorMethod(bool Setting);
    void SetPrintCutoffMethod(bool Setting);


};

#endif
