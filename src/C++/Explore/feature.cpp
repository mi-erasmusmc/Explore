//----------------------------------------------------------------------------//
// $Header: //depot/Explore/Explore/feature.cpp#19 $
// $Author: Rijnbeek $
// $DateTime: 2008/09/09 14:16:02 $
// $Change: 87 $
//----------------------------------------------------------------------------//

#include "feature.h"

/**********************************************************************
Function: FEATURE()
Category: Constructors and Destructors
Scope: public
In: -
Out: -
Description: Default constructor for a Feature.
**********************************************************************/
FEATURE::FEATURE() {
}

/**********************************************************************
Function: FEATURE()
Category: Constructors and Destructors
Scope: public
In: -
Out: -
Description: Default constructor for a Feature.
**********************************************************************/
FEATURE::FEATURE(string FeatureName, unsigned int FeatureNumber, CUTOFF_METHOD *FCutoffMethod,
                 OPERATOR_METHOD *FOperatorMethod, unsigned int *PClass, bool FContinuous) {
    Name = FeatureName;
    Number = FeatureNumber;

    CutoffMethod = FCutoffMethod;
    OperatorMethod = FOperatorMethod;
    PositiveClass = PClass;
    Continuous = FContinuous;
    CutoffRangeSet = false;
    Mandatory = false;
}

/**********************************************************************
Function: ~FEATURE()
Category: Constructors and Destructors
Scope: public
In: -
Out: -
Description: Destructor for a Feature.
**********************************************************************/
FEATURE::~FEATURE() {
}

/**********************************************************************
Function: PrintObservations()
Category: Selectors
Scope: public
In: -
Out: -
Description: Prints information of all observations currently associated
with this feature.
**********************************************************************/
void FEATURE::PrintObservations() {
    unsigned int LastObservation = Observations.size();
    for (unsigned int i = 0; i != LastObservation; i++) {
        i++;
        cout << "Observation: " << i;
        cout << "\tValue: " << Observations[i].GetValue();
        cout << "\tClass: " << Observations[i].GetClass();
        cout << "\tOrder: " << Observations[i].GetOrder();
    }
}

/**********************************************************************
Function: GetNoObservations()
Category: Selectors
Scope: public
In: -
Out: unsigned int, the number of cutoffs.
Description: Returns the number of cutoffs associated with this feature.
**********************************************************************/
unsigned int FEATURE::GetNoObservations() {
    return Observations.size();
}

/**********************************************************************
Function: GetObservationsPerClass()
Category: Selectors
Scope: public
In: -
Out: -
Description: Prints the number of observations for each class indicated
by the observations of this feature.
**********************************************************************/
vector<unsigned int> FEATURE::GetObservationsPerClass(unsigned int NoClasses) {
    vector<unsigned int> ObservationsPerClass;

    for (unsigned int i = 0; i < NoClasses; i++) {
        ObservationsPerClass.push_back(0);
    }

    vector<OBSERVATION>::iterator CurrentObservation(Observations.begin());
    vector<OBSERVATION>::iterator LastObservation(Observations.end());
    unsigned int CurrentClass;

    for (; CurrentObservation != LastObservation; CurrentObservation++) {
        CurrentClass = (*CurrentObservation).GetClass();
        ObservationsPerClass[CurrentClass]++;

    }
    return ObservationsPerClass;
}

/**********************************************************************
Function: GetNoCutoffs()
Category: Selectors
Scope: public
In: -
Out: unsigned int, the number of cutoffs.
Description: Returns the number of cutoffs associated with this feature.
**********************************************************************/
unsigned int FEATURE::GetNoCutoffs() {
    return Cutoffs.size();
}


/**********************************************************************
Function: GetNoOperators()
Category: Selectors
Scope: public
In: -
Out: unsigned int, the number of operators.
Description: Returns the number of operators associated with this feature.
**********************************************************************/
unsigned int FEATURE::GetNoOperators() {
    switch (Operator) {
        case EQUAL:
            return 1;
        case GREATER:
            return 1;
        case LESS:
            return 1;
        case GREATERLESS:
            return 2;
        default:
            return 0;
    }
}

/**********************************************************************
Function: GetOrder()
Category: Selectors
Scope: public
In: -
Out: unsigned int, order of the feature
Description: Returns the order of a specific feature in the population.
**********************************************************************/
unsigned int FEATURE::GetOrder() {                                              // Return the order of a feature
    return Number;
}

/**********************************************************************
Function: IsContinuous()
Category: Modifiers
Scope: private
In:
Out: bool, feature is continuous or not
Description: Indicates whether feature is continuous.
**********************************************************************/
bool FEATURE::IsContinuous() {
    return Continuous;
}


/**********************************************************************
Function: IsMandatory()
Category: Modifiers
Scope: private
In:
Out: bool, feature is mandatory or not
Description: Indicates whether feature is mandatory.
**********************************************************************/
bool FEATURE::IsMandatory() {
    return Mandatory;
}

/**********************************************************************
Function: IsClassFeature()
Category: Modifiers
Scope: private
In:
Out: bool, feature is determing the class or not
Description: Indicates whether feature is the classfeature.
**********************************************************************/
bool FEATURE::IsClassFeature() {
    return ClassFeature;
}

/**********************************************************************
Function: GetOperator()
Category: Selectors
Scope: public
In: -
Out: enum OPERATOR
Description: Returns the operator(s) associated with this feature.
**********************************************************************/
OPERATOR FEATURE::GetOperator() {
//  if (Operator==GREATERLESS) return GREATER;
//  else return Operator;
    return Operator;
}

/**********************************************************************
Function: GetCutoffs()
Category: Selectors
Scope: public
In: -
Out: vector<CUTOFF>, list of Cutoff(s).
Description: Returns a reference to the list of Cutoff(s) for this feature.
**********************************************************************/
vector<CUTOFF> FEATURE::GetCutoffs() {
    return Cutoffs;
}

/**********************************************************************
Function: GetCutoffs()
Category: Selectors
Scope: public
In: -
Out: vector<CUTOFF>*, pointer to the list of Cutoff(s).
Description: Returns a reference to the list of Cutoff(s) for this feature.
**********************************************************************/
list<string> FEATURE::GetCutoffValues() {
    list<string> Result;

    vector<CUTOFF>::iterator CurrentCutoff(Cutoffs.begin());
    vector<CUTOFF>::iterator LastCutoff(Cutoffs.end());

    for (; CurrentCutoff != LastCutoff; CurrentCutoff++) {
        Result.push_back((*CurrentCutoff).GetValue());
    }
    return Result;
}

/**********************************************************************
Function: PrintCutoffs()
Category: Selectors
Scope: public
In: -
Out: -
Description: Prints the current Cutoff(s) for this feature.
**********************************************************************/
void FEATURE::PrintCutoffs() {
    vector<CUTOFF>::iterator CurrentCutoff(Cutoffs.begin());
    vector<CUTOFF>::iterator LastCutoff(Cutoffs.end());

    for (; CurrentCutoff != LastCutoff; CurrentCutoff++) {
        cout << (*CurrentCutoff).GetValue() << " ";
    }
    cout << endl;
}

/**********************************************************************
Function: PrintOperators()
Category: Selectors
Scope: public
In: -
Out: -
Description: Prints the current Operator(s) for this feature.
**********************************************************************/
void FEATURE::PrintOperators() {
    switch (Operator) {
        case LESS:
            cout << "<=";
            break;
        case GREATER:
            cout << ">";
            break;
        case GREATERLESS:
            cout << "<=>";
            break;
        case EQUAL:
            cout << "=";
            break;
        case NONE:
            cout << "None";
            break;
    }
    cout << endl;
}

/**********************************************************************
Function: GetClass()
Category: Selectors
Scope: private
In: enum PARTITION_TYPE, type of partition.
	unsigned int PClass, class of the Class.
Out: CLASS*, pointer to the Class.
Description: Returns a reference to a particular Class that exists for this
feature.
**********************************************************************/
CLASSES *FEATURE::GetClass(PARTITION_TYPE PType, unsigned int PClass) {
    vector<CLASSES>::iterator CurrentClass;
    vector<CLASSES>::iterator LastClass;
    bool Found = false;

    switch (PType) {
        case LEARN:
            CurrentClass = LearnClasses.begin();
            LastClass = LearnClasses.end();
            break;
        case VALIDATION:
            CurrentClass = ValidationClasses.begin();
            LastClass = ValidationClasses.end();
            break;
        case TRAIN:
            CurrentClass = TrainClasses.begin();
            LastClass = TrainClasses.end();
            break;
    }

    for (; CurrentClass != LastClass && !Found; CurrentClass++) {
        if ((*CurrentClass).GetNumber() == PClass) {
            Found = true;
        }
    }
    CurrentClass--;

    return &(*CurrentClass);
}

/**********************************************************************
Function: ClassExists()
Category: Selectors
Scope: private
In: enum PARTITION_TYPE, type of partition.
	unsigned int PClass, class of the Class
Out: bool, found or not.
Description: Returns true if a particular Class exists for this feature.
**********************************************************************/
bool FEATURE::ClassExists(PARTITION_TYPE PType, unsigned int PClass) {
    vector<CLASSES>::iterator CurrentClass;
    vector<CLASSES>::iterator LastClass;

    switch (PType) {
        case LEARN:
            CurrentClass = LearnClasses.begin();
            LastClass = LearnClasses.end();
            break;
        case VALIDATION:
            CurrentClass = ValidationClasses.begin();
            LastClass = ValidationClasses.end();
            break;

        case TRAIN:
            CurrentClass = TrainClasses.begin();
            LastClass = TrainClasses.end();
            break;

    }

    for (; CurrentClass != LastClass; CurrentClass++) {
        if ((*CurrentClass).GetNumber() == PClass) {
            return true;
        }
    }
    return false;
}

/**********************************************************************
Function: GetName()
Category: Selectors
Scope: public
In: -
Out: char*, name
Description: Returns the name of feature
**********************************************************************/
string FEATURE::GetName() {
    return Name;
}

/**********************************************************************
Function: GetLearnClasses()
Category: Selectors
Scope: public
In: -
Out: vector<CLASSES>*, pointer to a Class
Description: Returns a reference to the Classes currently used
for learning.
**********************************************************************/
vector<CLASSES> *FEATURE::GetLearnClasses() {
    return &LearnClasses;
}

/**********************************************************************
Function: GetValidationClasses()
Category: Selectors
Scope: public
In: -
Out: vector<CLASSES>*, pointer to a Class
Description: Returns a reference to the Classes currently used
for validation.
**********************************************************************/
vector<CLASSES> *FEATURE::GetValidationClasses() {
    return &ValidationClasses;
}

/**********************************************************************
Function: GetTrainClasses()
Category: Selectors
Scope: public
In: -
Out: vector<CLASSES>*, pointer to a Class
Description: Returns a reference to the Classes currently used
for testing.
**********************************************************************/
vector<CLASSES> *FEATURE::GetTrainClasses() {
    return &TrainClasses;
}


/**********************************************************************
Function: GeCutofftRange()
Category: Selectors
Scope: public
In: -
Out: Range, the range for this feature
Description: Returns the range of this feature.
**********************************************************************/
vector<Range> *FEATURE::GetCutoffRanges() {
    return &CutoffRange;
}

/**********************************************************************
Function: AddObservation()
Category: Modifiers
Scope: public
In: float, value of the observation.
	unsigned int, class of the observation.
	unsigned int, order of the observation
Out: -
Description: Adds an observation to a feature.
**********************************************************************/
void FEATURE::AddObservation(string FValue, unsigned int FClass) {
    Observations.push_back(OBSERVATION(FValue, FClass, Observations.size()));
}

/**********************************************************************
Function: SortObservations()
Category: Modifiers
Scope: private
In: enum FEATURE_SORT, structure defining the different ways in which
the observations can be sorted.
Out: -
Description: Sorts all observations associated with a feature.
**********************************************************************/
void FEATURE::SortObservations(FEATURE_SORT FSort) {
    switch (FSort) {
        case ORDER:
            sort(Observations.begin(), Observations.end(), ByValue);
            break;
        case CLASS:
            sort(Observations.begin(), Observations.end(), ByClass);
            break;
        case VALUE:
            sort(Observations.begin(), Observations.end(), ByValue);
            break;
    }
}

/**********************************************************************
Function: GetObservation()
Category: Modifiers
Scope: public
In: unsigned int, order of the observation to return.
Out: OBSERVATION*, pointer to the original observation.
Description: Returns a reference to an observation.
**********************************************************************/
OBSERVATION *FEATURE::GetObservation(unsigned int ONumber) {
    OBSERVATION *CurrentObservation = &Observations.at(ONumber);
    return CurrentObservation;
}

/**********************************************************************
Function: AddClassObservation()
Category: Modifiers
Scope: public
In: OBSERVATION*, pointer to an observation.
	enum PARTITION_TYPE, type of partition.
Out: -
Description: Adds an observation to a Class.
**********************************************************************/
void FEATURE::AddClassObservation(OBSERVATION *PObservation, PARTITION_TYPE PType) {
    unsigned int CurrentClass = (*PObservation).GetClass();
    CLASSES *TempClass;


    if (CurrentClass != *PositiveClass) {
        CurrentClass = 0;
    } else {
        CurrentClass = 1;
    }

    TempClass = GetClass(PType, CurrentClass);
    (*TempClass).AddObservation(PObservation);

    switch (PType) {
        case LEARN:
            LearnObservations.push_back(PObservation);
            break;
        case VALIDATION:
            ValidationObservations.push_back(PObservation);
            break;
        case TRAIN:
            TrainObservations.push_back(PObservation);
            break;

    }
}

/**********************************************************************
Function: ClearClassObservations()
Category: Modifiers
Scope: private
In: -
Out: -
Description: Removes all references in LearnObservations and
TestObservations.
**********************************************************************/
void FEATURE::ClearClassObservations() {
    LearnObservations.clear();
    ValidationObservations.clear();
    TrainObservations.clear();
}

/**********************************************************************
Function: AddClass()
Category: Modifiers
Scope: private
In: unsigned int, classnumber
    enum PARTITION_TYPE, type of partition.
Out: -
Description: Adds a Class to the LearnClasses or TestClasses
list.
**********************************************************************/
void FEATURE::AddClass(unsigned int ClassNumber, PARTITION_TYPE PType) {
    switch (PType) {
        case LEARN:
            LearnClasses.push_back(CLASSES(ClassNumber));
            break;
        case VALIDATION:
            ValidationClasses.push_back(CLASSES(ClassNumber));
            break;
        case TRAIN:
            LearnClasses.push_back(CLASSES(ClassNumber));
            ValidationClasses.push_back(CLASSES(ClassNumber));
            break;
    }
}

/**********************************************************************
Function: AddClass()
Category: Modifiers
Scope: private
In: Class, the Class.
	enum PARTITION_TYPE, type of partition.
Out: -
Description: Adds a Class to the LearnClsses or TestClasses
list.
**********************************************************************/
void FEATURE::AddClass(CLASSES FClass, PARTITION_TYPE PType) {
    switch (PType) {
        case LEARN:
            LearnClasses.push_back(FClass);
            break;
        case VALIDATION:
            ValidationClasses.push_back(FClass);
            break;
        case TRAIN:
            LearnClasses.push_back(FClass);
            ValidationClasses.push_back(FClass);
            break;
    }
}

/**********************************************************************
Function: InitialiseClasses()
Category: Modifiers
Scope: public
In: -
Out: -
Description: Clears all classes of observations.
**********************************************************************/
void FEATURE::InitialiseClasses() {
    LearnClasses.clear();
    ValidationClasses.clear();
    TrainClasses.clear();

    LearnObservations.clear();
    ValidationObservations.clear();
    TrainObservations.clear();

//  CLASSES Negative(0);
//  CLASSES Positive(1);

//  AddClass(Negative,LEARN);
//  AddClass(Negative,TEST);
//  AddClass(Positive,LEARN);
//  AddClass(Positive,TEST);

    LearnClasses.push_back(0);
    LearnClasses.push_back(1);

    ValidationClasses.push_back(0);
    ValidationClasses.push_back(1);

    TrainClasses.push_back(0);
    TrainClasses.push_back(1);

}

/**********************************************************************
Function: SortPartitions()
Category: Modifiers
Scope: public
In: -
Out: -
Description: Sorts LearnObservations and TestObservations based on
their value (through pointers).
**********************************************************************/
void FEATURE::SortPartitions() {
    sort(LearnObservations.begin(), LearnObservations.end(), ByReferentValue);
    sort(ValidationObservations.begin(), ValidationObservations.end(), ByReferentValue);
    sort(TrainObservations.begin(), TrainObservations.end(), ByReferentValue);
}

/**********************************************************************
Function: FindOperators()
Category: Modifiers
Scope: public
In: unsigned int*, pointer to a class.
Out: -
Description: Decides which function has to be called for finding
operators for a particular feature.
**********************************************************************/
void FEATURE::FindOperators() {
    if (Continuous) {
        switch (*OperatorMethod) {
            case MANUAL:
                break;
            case EXHAUSTIVE:
                ExhaustiveOperator();
                break;
            case MEDIAN:
                MedianOperator();
                break;
            case ROCAREA:
                ROCAreaOperator();
                break;
            case ROCDIAGONAL:
                ROCDiagonalOperator();
                break;
        }
    } else {
        Operator = EQUAL;
    }

}

/**********************************************************************
Function: MedianOperator()
Category: Modifiers
Scope: private
In: -
Out: -
Description: Finds operators by finding median observations in
positive and negative class and comparing these.
**********************************************************************/
void FEATURE::MedianOperator() {
    unsigned int PositiveIndex, NegativeIndex;
    OBSERVATION *PositiveObservation;
    OBSERVATION *NegativeObservation;
    ostringstream Result;

    // Sort classes by order
    LearnClasses[0].SortOrder();
    LearnClasses[1].SortOrder();

    // Calculate index of negative and positive classes of learn partition
    NegativeIndex = (unsigned int) (LearnClasses[0].GetSize() / 2);
    PositiveIndex = (unsigned int) (LearnClasses[1].GetSize() / 2);

    // Retrieve observations associated with indexes
    NegativeObservation = LearnClasses[0].GetObservation(NegativeIndex);
    PositiveObservation = LearnClasses[1].GetObservation(PositiveIndex);

    // Compare values of median observations in both classes and set operator
    if ((*PositiveObservation).GetValue() > (*NegativeObservation).GetValue()) {
        Operator = GREATER;                                                         // Set cutoff operator to GREATER
    } else {
        Operator = LESS;                                                            // Set cutoff operator to LESS
    }

    // Print debug information
    Result << "MEDIAN OPERATOR" << endl;
    Result << "Feature " << Number << endl;
    Result << "Positive\tNegative\tOperator" << endl;
    Result << (*PositiveObservation).GetValue() << "\t";
    Result << (*NegativeObservation).GetValue() << "\t";
    Result << endl;

    OperatorOutput = Result.str();
}

/**********************************************************************
Function: ROCAreaOperator()
Category: Modifiers
Scope: private
In: -
Out: -
Description: Finds operators by calculating the area under 'Receiver
Operating Characteristics' method.
**********************************************************************/
void FEATURE::ROCAreaOperator() {
    vector<ROCPair> ROC;
    float FPF, Sensitivity, Value, Class;
    ostringstream Result;

    Result.precision(4);
    Result.width(4);

    // Sort observations of learning partition by value
    sort(LearnObservations.begin(), LearnObservations.end(), ByReferentValue);

    // Debug information
    Result << endl << "ROC AREA (Feature " << (Number + 1) << ")" << endl << endl;
    Result << "Observation\tValue\tClass\tFPF\t\tSensitivity" << endl;

    // Traverse observations in learning partition
    for (unsigned int i = 0; i < LearnObservations.size(); i++) {
        Value = atof(LearnObservations[i]->GetValue().c_str());
        Class = LearnObservations[i]->GetClass();
        FPF = (float) 1 - (float) LearnClasses[0].GetSpecificity(Value);
        Sensitivity = LearnClasses[1].GetSensitivity(Value);

        // Debug information for calculating FPF and sensitivity
        Result << i << "\t\t" << Value << "\t" << Class << "\t" << FPF << "\t\t" << Sensitivity << endl;

        // Add calculated FPF and sensitivity plus cutoff value to ROC
        ROC.push_back(ROCPair(FPF, Sensitivity, Value));
    }

    // Sort ROC observations, comparison function is defined in ROCPair
    sort(ROC.begin(), ROC.end());

    // Initialise variables for calculating area under ROC
    float X1 = 0, X2 = 0, RYMax = 0;
    float TriangleHeight = 0, TriangularArea = 0, RectangularArea = 0;
    float ROCArea = 0;

    // Debug information for calculating area under ROC
    Result << endl << "AREA" << endl << endl;
    Result << "Observation\tTotal Area" << endl;

    // Traverse ROC and calculate area under it
    for (unsigned int i = 0; i < ROC.size() - 1; i++) {
        TriangularArea = RectangularArea = 0;

        X1 = ROC[i].GetFPF();
        RYMax = ROC[i].GetSensitivity();

        X2 = ROC[i + 1].GetFPF();

        if (ROC[i + 1].GetSensitivity() < RYMax) {
            TriangleHeight = RYMax - ROC[i + 1].GetSensitivity();
            RYMax = ROC[i + 1].GetSensitivity();
        } else {
            TriangleHeight = ROC[i + 1].GetSensitivity() - RYMax;
        }

        RectangularArea = (X2 - X1) * RYMax;
        TriangularArea = (float) ((X2 - X1) * TriangleHeight) / (float) 2;

        // Add current area to total area
        ROCArea += (RectangularArea + TriangularArea);

        // Print debug information
        Result << i << "\t\t" << ROCArea << endl;
    }

    // Save debug information
    OperatorOutput = Result.str();

    // Decide on operator based on area under ROC
    if (ROCArea >= 0.5) {
        Operator = LESS;
    } else {
        Operator = GREATER;
    }
}

/**********************************************************************
Function: ROCDiagonalOperator()
Category: Modifiers
Scope: private
In: -
Out: -
Description: Finds operators by calculating if 'Receiver Operating
Characteristics' method is below or above the diagonal.
**********************************************************************/
void FEATURE::ROCDiagonalOperator() {
    float ROCDiagonal, Sensitivity, FPF;
    bool Below, Above;
    ostringstream Result;

    Result.precision(4);
    Result.width(4);

    Result << "ROC DIAGONAL OPERATOR METHOD" << endl;
    Result << "Feature " << Number << endl;
    Result << "Value\tSensitivity\tFPF\tAbove/Below Diagonal" << endl;

    for (unsigned int i = 0; i < LearnObservations.size(); i++) {
        // Calculate specificity and FPF
        Sensitivity = LearnClasses[1].GetSensitivity(atof(LearnObservations[i]->GetValue().c_str()));
        FPF = (float) 1 - (float) LearnClasses[0].GetSpecificity(atof(LearnObservations[i]->GetValue().c_str()));

        // Debug information
        Result << endl << LearnObservations[i]->GetValue() << "\t" << Sensitivity << "\t" << FPF << "\t";

        // Compare
        if (Sensitivity < FPF) {
            Below = true;
            Result << "BELOW DIAGONAL";
        } else if (Sensitivity > FPF) {
            Above = true;
            Result << "ABOVE DIAGONAL";
        }
    }
    Result << endl;

    // Save debug information
    OperatorOutput = Result.str();

    // Calculate operator
    if (Below && Above) {
        Operator = GREATERLESS;
    } else if (Above) {
        Operator = GREATER;
    } else if (Below) {
        Operator = LESS;
    }
}

/**********************************************************************
Function: ExhaustiveOperator()
Category: Modifiers
Scope: private
In: -
Out: -
Description: Assigns both operators to the feature.
**********************************************************************/
void FEATURE::ExhaustiveOperator() {
    Operator = GREATERLESS;
}

/**********************************************************************
Function: FindCutoffs()
Category: Modifiers
Scope: public
In:  -
Out: -
Description: Decides which function to call for finding cutoff values.
**********************************************************************/
void FEATURE::FindCutoffs() {
    switch (*CutoffMethod) {
        case USER:
            break;
        case ALL:
            Cutoffs.clear();
            SimpleCutoff();
            break;
        case RVAC:
            Cutoffs.clear();
            RVACutoff();
            break;
    }
}

/**********************************************************************
Function: SimpleCutoff()
Category: Modifiers
Scope: private
In: -
Out: -
Description: Finds interesting cutoff values where each unique
observation value is a cutoff value.
**********************************************************************/
void FEATURE::SimpleCutoff() {
    string LastValue = "";
    ostringstream Result;

    sort(LearnObservations.begin(), LearnObservations.end(), ByReferentValue);

    // Traverse observations in learning partition
    for (unsigned int i = 0; i < LearnObservations.size(); i++) {
        // Compare sequential values and store each unique value
        if (LastValue != LearnObservations[i]->GetValue()) {
            LastValue = LearnObservations[i]->GetValue();
            Cutoffs.push_back(CUTOFF(LearnObservations[i]->GetValue()));
        }
    }

    // Debug information
    Result << "SIMPLE CUTOFF FEATURE " << Number << endl;
    Result << "Number\tValue" << endl;

    for (unsigned int i = 0; i < Cutoffs.size(); i++) {
        Result << i + 1 << "\t" << Cutoffs[i].GetValue() << endl;
    }

    Result << endl;

    // Save debug information
    CutoffOutput = Result.str();
}

/**********************************************************************
Function: RVACutoff()
Category: Modifiers
Scope: private
In: -
Out: -
Description: Finds interesting cutoff values by using referent-value
analysis.
**********************************************************************/
void
FEATURE::RVACutoff() {                                                     // Referent value analysis, group observations with same value, then compare these groups according to number present in each class
    string LastValue, CurrentValue;
    ostringstream Result;

    // Sort observations in learn partition
    sort(LearnObservations.begin(), LearnObservations.end(), ByReferentValue);

    vector<BIN> Bins;
    LastValue = "";
    bool FirstObservation = true;

    for (unsigned int i = 0; i < LearnObservations.size(); i++) {
        // Save every unique observation value in a bin
        CurrentValue = LearnObservations[i]->GetValue();
        if (LastValue != CurrentValue || FirstObservation) {
            LastValue = CurrentValue;
            Bins.push_back(BIN(CurrentValue));
        }

        // Bin already exists, increase positive or negative count for later use
        if (LearnObservations[i]->GetClass() == (*PositiveClass)) {
            Bins[Bins.size() - 1].IncreasePositiveCount();
        } else {
            Bins[Bins.size() - 1].IncreaseNegativeCount();
        }

        FirstObservation = false;
    }

    unsigned int CurrentBin = 0;
    unsigned int NextBin = 1;
    unsigned int LastBin = Bins.size();

    unsigned int CurrentNegCount = 0;
    unsigned int CurrentPosCount = 0;
    unsigned int NextNegCount = 0;
    unsigned int NextPosCount = 0;
    char dum[10];

    float BinCurrentValue, BinNextValue, BinCutoffValue;

    // Debug information
    Result << "RVA FEATURE " << (Number + 1) << endl;
    Result << "Bin\tValue\tNegativeCount\tPositiveCount" << endl;
    for (unsigned int i = 0; i < LastBin; i++) {
        Result << i << "\t" << Bins[i].GetValue() << "\t" << Bins[i].GetNegativeCount() << "\t\t"
               << Bins[i].GetPositiveCount() << endl;
    }
    Result << endl;

    // Save debug information
    CutoffOutput = Result.str();

    // Compare each Bin sequentially
    if (Continuous) {
        while (NextBin != LastBin || LastBin == 1) {
            CurrentNegCount = Bins[CurrentBin].GetNegativeCount();
            CurrentPosCount = Bins[CurrentBin].GetPositiveCount();

            NextNegCount = Bins[NextBin].GetNegativeCount();
            NextPosCount = Bins[NextBin].GetPositiveCount();

            // If following condition is met, then add this value as a Cutoff
            if ((CurrentNegCount != 0 && CurrentPosCount != 0) ||
                ((CurrentNegCount * NextPosCount) != (NextNegCount * CurrentPosCount))) {

                BinCurrentValue = atof(Bins[CurrentBin].GetValue().c_str());
                BinNextValue = atof(Bins[NextBin].GetValue().c_str());
                BinCutoffValue = (BinCurrentValue + BinNextValue) / 2;
                sprintf(dum, "%.2f", BinCutoffValue);                                    //convert float to string
                Cutoffs.push_back(CUTOFF(dum));
            }

            if (LastBin==1) {
                break;
            }
            CurrentBin++;
            NextBin++;
        }
    } else {
        while (CurrentBin != LastBin) {                                            //Nominal feature so add all bins
            Cutoffs.push_back(CUTOFF(Bins[CurrentBin].GetValue()));
            CurrentBin++;
        }
    }
    Bins.clear();
}

/**********************************************************************
Function: AddCutoff()
Category: Modifiers
Scope: private
In: float, cutoff value
Out: -
Description: Manually add a cutoff value to this feature.
**********************************************************************/
void FEATURE::AddCutoff(string CutoffValue) {
    Cutoffs.push_back(CUTOFF(CutoffValue));
}

/**********************************************************************
Function: RemoveCutoff()
Category: Modifiers
Scope: private
In: unsigned int, cutoff number
Out: -
Description: Manually remove a cutoff value from this feature (by number).
**********************************************************************/
void FEATURE::RemoveCutoff(unsigned int CutoffNumber) {
    if (CutoffNumber < Cutoffs.size()) {
        vector<CUTOFF>::iterator CurrentCutoff(Cutoffs.begin() + CutoffNumber);
        Cutoffs.erase(CurrentCutoff);
    }
}

/**********************************************************************
Function: RemoveCutoff()
Category: Modifiers
Scope: private
In: float, cutoff value
Out: -
Description: Manually remove a cutoff value from this feature (by value).
**********************************************************************/
void FEATURE::RemoveCutoff(string CutoffValue) {
    vector<CUTOFF>::iterator CurrentCutoff(Cutoffs.begin());
    vector<CUTOFF>::iterator LastCutoff(Cutoffs.end());

    for (; CurrentCutoff != LastCutoff; CurrentCutoff++) {
        if (CurrentCutoff->GetValue() == CutoffValue) {
            Cutoffs.erase(CurrentCutoff);
        }
    }
}

/**********************************************************************
Function: RemoveAllCutoff()
Category: Modifiers
Scope: private
In: -
Out: -
Description: Remove all cutoffs of this feature.
**********************************************************************/
void FEATURE::RemoveAllCutoffs() {
    Cutoffs.clear();
}

/**********************************************************************
Function: SetOperator()
Category: Modifiers
Scope: private
In: OPERATOR, feature operator
Out: -
Description: Manually set the operator for this feature.
**********************************************************************/
void FEATURE::SetOperator(OPERATOR FeatureOperator) {
    Operator = FeatureOperator;
}

/**********************************************************************
Function: SetPositiveClass()
Category: Modifiers
Scope: private
In: unsigned int*, positive class
Out: -
Description: Set the positive class for this feature.
**********************************************************************/
void FEATURE::SetPositiveClass(unsigned int *PClass) {
    PositiveClass = PClass;
}

/**********************************************************************
Function: SetContinuous()
Category: Modifiers
Scope: private
In: bool, feature is continuous or not
Out: -
Description: Manually indicate whether feature is continous.
**********************************************************************/
void FEATURE::SetContinuous(bool IsContinuous) {
    Continuous = IsContinuous;
}

/**********************************************************************
Function: SetClassFeature()
Category: Modifiers
Scope: private
In: bool, feature is determining the class
Out: -
Description: Manually indicate whether feature is the class feature
**********************************************************************/
void FEATURE::SetClassFeature(bool IsClassFeature) {
    ClassFeature = IsClassFeature;
}

/**********************************************************************
Function: SetMandatory()
Category: Modifiers
Scope: private
In: bool, feature is mandatory or not
Out: -
Description: Manually indicate whether feature is mandatory.
**********************************************************************/
void FEATURE::SetMandatory(bool IsMandatory) {
    Mandatory = IsMandatory;
}

/**********************************************************************
Function: SetCutoffRange()
Category: Modifiers
Scope: private
In: Range, the cutoff range
Out: -
Description: Manually set a cutoff-range for this feature.
**********************************************************************/
void FEATURE::AddCutoffRange(Range NewRange) {
    if (NewRange.Min < NewRange.Max) {
        CutoffRange.push_back(NewRange);
        CutoffRangeSet = true;
    }
}

/**********************************************************************
Function: RemoveCutoffRange()
Category: Modifiers
Scope: private
In: -
Out: -
Description: Remove the cutoff range.
**********************************************************************/
void FEATURE::RemoveCutoffRanges() {
    if (CutoffRangeSet) {
        CutoffRange.clear();
        CutoffRangeSet = false;
    }
}

/**********************************************************************
Function: RemoveCutoffRange()
Category: Modifiers
Scope: private
In: -
Out: -
Description: Remove the cutoff range.
**********************************************************************/
void FEATURE::RemoveCutoffRange(unsigned int RangeNumber) {
    vector<Range>::iterator CurrentRange(CutoffRange.begin());
    CurrentRange += (RangeNumber - 1);
    CutoffRange.erase(CurrentRange);
    if (CutoffRange.size() == 0) {
        CutoffRangeSet = false;
    }
}

/**********************************************************************
Function: IsCutoffRangeSet()
Category: Modifiers
Scope: private
In: -
Out: bool, cutoff-range has/has not been set
Description: Returns whether the cutoff-range has been set for this
feature.
**********************************************************************/
bool FEATURE::IsCutoffRangeSet() {
    return CutoffRangeSet;
}

/**********************************************************************
Function: InitialiseRanges()
Category: Modifiers
Scope: private
In: -
Out: -
Description: Implement the cutoff range. Delete any cutoffs which
aren't part of the range given.
**********************************************************************/
void FEATURE::InitialiseRanges() {
    vector<CUTOFF>::iterator CurrentCutoff(Cutoffs.begin());
    vector<CUTOFF>::iterator LastCutoff(Cutoffs.end());
    bool InRange;

    if (CutoffRangeSet && Continuous) {
        while (CurrentCutoff != LastCutoff) {
            vector<Range>::iterator CurrentRange(CutoffRange.begin());
            vector<Range>::iterator LastRange(CutoffRange.end());
            for (; CurrentRange != LastRange; CurrentRange++) {
                InRange = false;
                if ((*CurrentRange).InRange(atof(CurrentCutoff->GetValue().c_str()))) {
                    InRange = true;
                }
            }
            if (!InRange) {
                Cutoffs.erase(CurrentCutoff);
                LastCutoff = Cutoffs.end();
            } else {
                CurrentCutoff++;
            }
        }
    }
}

/**********************************************************************
Function: Reset()
Category: Modifiers
Scope: private
In: -
Out: -
Description: Reset this feature (clears all modifiable data).
**********************************************************************/
void FEATURE::Reset() {
}

/**********************************************************************
Function: PrintOperatorMethod()
Category: Selectors
Scope: private
In: -
Out: string, debug output
Description: Prints operator-method debug information. Stored in
string (private member).
**********************************************************************/
string FEATURE::PrintOperatorMethod() {
    return OperatorOutput;
}

/**********************************************************************
Function: PrintOperatorMethod()
Category: Selectors
Scope: private
In: -
Out: string, debug output
Description: Prints cutoff-method debug information. Stored in
string (private member).
**********************************************************************/
string FEATURE::PrintCutoffMethod() {
    return CutoffOutput;
}
