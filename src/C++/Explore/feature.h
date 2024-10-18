//----------------------------------------------------------------------------//
// $Header: //depot/Explore/Explore/feature.h#15 $
// $Author: Rijnbeek $
// $DateTime: 2008/09/09 14:16:02 $
// $Change: 87 $
//----------------------------------------------------------------------------//

#ifndef FEATURE_H
#define FEATURE_H

#include "initialise.h"
#include "observation.h"
#include "cutoff.h"
#include "classdef.h"
#include "bin.h"
#include "rocpair.h"

class FEATURE {
private:
    vector<OBSERVATION> Observations;                                           // Vector of observations enabling direct access
    // Vector of cutoffs enabling direct access

    vector<CLASSES> LearnClasses;                                               // List of pointers to class objects of observations ordered on value for learning
    vector<CLASSES> ValidationClasses;                                          // List of pointers to class objects of observations ordered on value for validation
    vector<CLASSES> TrainClasses;                                                // List of pointers to class objects of observations ordered on value for training

    vector<OBSERVATION*> LearnObservations;                                     // A vector of observations used for learning ordered by value
    vector<OBSERVATION*> ValidationObservations;                                // A vector of observations used for validation ordered by value
    vector<OBSERVATION*> TrainObservations;                                      // A vector of observations used for training ordered by value

    OPERATOR Operator;                                                          // Operator associated with this feature

    CUTOFF_METHOD* CutoffMethod;                                                // Pointer to cutoff method to be used
    OPERATOR_METHOD* OperatorMethod;                                            // Pointer to operator method to be used

    vector<Range> CutoffRange;                                                  // Vector of ranges

    unsigned int* PositiveClass;                                                // Class for which a rule has to be found

    string Name;                                                                // Name of the feature
    unsigned int Number;                                                        // Order of the feature

    bool Continuous;															// Does this feature contain continuously valued observations?
    bool ClassFeature;															// Is this the feature that is used as ClassFeature?
    bool Mandatory;                                                             // Is this a mandatory feature?
    bool CutoffRangeSet;                                                        // Has a cutoff range been set?

    // Private structures for sorting observations

    // Structure to sort observations based on their class
    struct SortByClass {
        bool operator()(const OBSERVATION& a, const OBSERVATION& b) {
            if (a.GetClass() == b.GetClass()) return a.GetValue() < b.GetValue();
            else return a.GetClass() < b.GetClass();
        }
    } ByClass;

    // Structure to sort observations based on their order
    struct SortByOrder {
        bool operator()(const OBSERVATION& a, const OBSERVATION& b) {
            return a.GetOrder() < b.GetOrder();
        }
    } ByOrder;

    // Structure to sort observations based on their value
    struct SortByValue {
        bool operator()(const OBSERVATION& a, const OBSERVATION& b) {
            return a.GetValue() < b.GetValue();
        }
    } ByValue;

    // Structure to sort observations based on their value (pointers)
    struct SortPByValue {
        bool operator()(const OBSERVATION* a, const OBSERVATION* b) {
            return a->GetValue() < b->GetValue();
        }
    } ByReferentValue;

    // Private modifiers
    void RVACutoff();                                                           // Find cutoffs based on referent-value-analysis
    void SimpleCutoff();                                                        // Find cutoffs based on unique values

    void MedianOperator();                                                      // Find operators based on Median method for specified class
    void ROCAreaOperator();                                                     // Find operators based on calculating area under ROC for specified class
    void ROCDiagonalOperator();                                                 // Find operators based on calculating diagonal of ROC for specified class
    void ExhaustiveOperator();                                                  // Assign both operators to each feature

    void SortObservations(FEATURE_SORT FSort);                                  // Sort observations based on value,order,class etc

    bool ClassExists(PARTITION_TYPE PType, unsigned int PClass);                // See if a particular class of observations exists
    CLASSES* GetClass(PARTITION_TYPE PType, unsigned int PClass);               // Get a pointer to a specific class
    void AddClass(CLASSES FClass, PARTITION_TYPE PType);                        // Add a Class
    void AddClass(unsigned int ClassNumber, PARTITION_TYPE PType);              // Add a Class

    string CutoffOutput;
    string OperatorOutput;

public:
    // Constructors and destructors
    FEATURE();
    FEATURE(string FeatureName, unsigned int FeatureNumber, CUTOFF_METHOD* FCutoffMethod, OPERATOR_METHOD* FOperatorMethod, unsigned int* PClass, bool Continuous);
    ~FEATURE();

    // Selectors
    vector<unsigned int> GetObservationsPerClass(unsigned int NoClasses);       // Return the number of observations per class for this features
    unsigned int GetNoObservations();                                           // Return the number of observations for this feature
    unsigned int GetNoCutoffs();                                                // Return the number of cutoffs for this feature
    unsigned int GetNoOperators();                                              // Return the number of operators for this feature
    unsigned int GetOrder();                                                    // Return the order of a feature
    string GetName();                                                           // Return the name of a feature

    bool IsContinuous();                                                        // Is this feature continuous?
    bool IsMandatory();                                                         // Is this feature mandatory?
    bool IsClassFeature();                                                      // Is this feature determining the class?

    OBSERVATION* GetObservation(unsigned int ONumber);                          // Get a pointer to an observation
    vector<CUTOFF> GetCutoffs();                                                // Return a pointer to a vector of cutoff values for this feature
    list<string> GetCutoffValues();                                              // Return a list of cutoffs
    OPERATOR GetOperator();                                                     // Return the operator associated with this feature

    vector<CLASSES>* GetLearnClasses();                                         // Return a pointer to the learn Class
    vector<CLASSES>* GetValidationClasses();                                    // Return a pointer to the validation Class
    vector<CLASSES>* GetTrainClasses();                                          // Return a pointer to the test Class

    // Modifiers
    void AddObservation(string FValue, unsigned int FClass);                    // Add an observation to this feature

    void AddClassObservation(OBSERVATION* PObservation, PARTITION_TYPE PType);  // Add an observation to LearnObservations or TestObservations or ValidationObservations and Class
    void ClearClassObservations();                                              // Remove all references in LearnObservations or TestObservations
    void InitialiseClasses();                                                   // Clear all Class and Learn/TestObservation structures
    void SortPartitions();                                                      // Sort Learn + TestObservations by value (through pointers)

    void FindCutoffs();                                                         // Calculate interesting cutoffs
    void FindOperators();                                                       // Calculate operators for each cutoff value

    void AddCutoff(string CutoffValue);                                          // Manually add a cutoff to this feature
    void RemoveCutoff(unsigned int CutoffNumber);                               // Manually remove a cutoff from this feature (by number)
    void RemoveCutoff(string CutoffValue);                                       // Manually remove a cutoff from this feature (by value)
    void RemoveAllCutoffs();                                                    // Manually remove all cutoffs from this feature
    void SetOperator(OPERATOR FeatureOperator);                                 // Manually set the operator for this feature

    void SetContinuous(bool IsContinuous);
    void SetClassFeature(bool IsClassFeature);
    void SetMandatory(bool IsMandatory);
    void SetPositiveClass(unsigned int* FeatureNumber);

    void AddCutoffRange(Range NewRange);                                        // Set a cutoff-range for this feature
    vector<Range>* GetCutoffRanges();                                           // Get the cutoff-range for this feature
    void RemoveCutoffRanges();                                                  // Remove all cutoff-ranges for this feature
    void RemoveCutoffRange(unsigned int RangeNumber);                           // Remove a cutoff-range for this feature
    void InitialiseRanges();                                                    // Implement the ranges for this feature
    bool IsCutoffRangeSet();                                                    // Has a cutoff-range been set for this feature

    void Reset();                                                               // Reset this feature.

    // Output
    void PrintCutoffs();                                                        // Print cutoff values
    void PrintOperators();                                                      // Print operator(s)
    void PrintObservations();                                                   // Print population observations

    string PrintCutoffMethod();                                                 // Print cutoff method information
    string PrintOperatorMethod();                                               // Print operator method information
    vector<CUTOFF> Cutoffs;
};

#endif
