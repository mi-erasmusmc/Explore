//----------------------------------------------------------------------------//
// $Header: //depot/Explore/Explore/population.cpp#22 $
// $Author: Rijnbeek $
// $DateTime: 2008/09/09 14:16:02 $
// $Change: 87 $
//----------------------------------------------------------------------------//

#include "population.h"
#include "explore.h"

#ifdef DEBUG_TIMING
extern TIMING ExploreTiming;
#endif

/**********************************************************************
Function: POPULATION()
Category: Constructors and Destructors
Scope: public
In: -
Out: -
Description: Default constructor for a Population.
**********************************************************************/
POPULATION::POPULATION() {
  NoObservations = 0;
  IsPartitioned = false;
  SeedIsSet = false;
}                                  

/**********************************************************************
Function: ~POPULATION()
Category: Constructors and Destructors
Scope: public
In: -
Out: -
Description: Destructor for a Population.
**********************************************************************/
POPULATION::~POPULATION() {
}

/**********************************************************************
Function: Clear()
Category: Modifiers
Scope: public
In: -
Out: -
Description: Clear this population.
**********************************************************************/
void POPULATION::Clear() {
#ifdef DEBUG_TIMING
  clock_t Start, End;
  Start = clock();
#endif

  ClassRatios.clear();
  ClassNames.clear();

  Features.clear();
  Partitions.clear();

#ifdef DEBUG_TIMING
  End = clock();
  ExploreTiming.AddTime("POPULATION::Reset", Start, End);
#endif
}

/**********************************************************************
Function: GetNoFeatures()
Category: Selectors
Scope: public
In: -
Out: unsigned int, number of features
Description: Returns the number of features associated with this population.
**********************************************************************/
unsigned int POPULATION::GetNoFeatures() {                                      // Get the number of features in this population
  return Features.size();
}

/**********************************************************************
Function: GetNoObservations()
Category: Selectors
Scope: public
In: -
Out: unsigned int, number of observations
Description: Returns the number of observations in this population.
**********************************************************************/
unsigned int POPULATION::GetNoObservations() {
  return NoObservations;
}

/**********************************************************************
Function: GetNoCutoffs()
Category: Selectors
Scope: public
In: -
Out: unsigned int, number of cutoffs
Description: Returns the number of observations in this population.
**********************************************************************/
unsigned int POPULATION::GetNoCutoffs(unsigned int FeatureNumber) {
#ifdef DEBUG_TIMING
  clock_t Start, End;
  Start = clock();
#endif

  vector<FEATURE>::iterator CurrentFeature(Features.begin());
  CurrentFeature += (FeatureNumber);

#ifdef DEBUG_TIMING
  End = clock();
  ExploreTiming.AddTime("POPULATION::GetNoCutoffs", Start, End);
#endif
  
  return (*CurrentFeature).GetNoCutoffs();
}

/**********************************************************************
Function: GetNoOperators()
Category: Selectors
Scope: public
In: -
Out: unsigned int, number of cutoffs
Description: Returns the number of operators for a specific features.
**********************************************************************/
unsigned int POPULATION::GetNoOperators(unsigned int FeatureNumber) {
#ifdef DEBUG_TIMING
  clock_t Start, End;
  Start = clock();
#endif

  vector<FEATURE>::iterator CurrentFeature(Features.begin());
  CurrentFeature += (FeatureNumber);

#ifdef DEBUG_TIMING
  End = clock();
  ExploreTiming.AddTime("POPULATION::GetNoOperators", Start, End);
#endif

  return (*CurrentFeature).GetNoCutoffs();
}

/**********************************************************************
Function: GetNoPartitions()
Category: Selectors
Scope: public
In: -
Out: unsigned int, number of partitions
Description: Returns the number of partitions for the experiment.
**********************************************************************/
unsigned int POPULATION::GetNoPartitions() {
  return *NoPartitions;
}

/**********************************************************************
Function: IsContinuous()
Category: Selectors
Scope: public
In: unsighed int, featurenumber
Out: bool, continuous or not
Description: Returns if a specific feature is continuous.
**********************************************************************/
bool POPULATION::IsContinuous(unsigned int FeatureNumber) {
#ifdef DEBUG_TIMING
  clock_t Start, End;
  Start = clock();
#endif

  vector<FEATURE>::iterator CurrentFeature(Features.begin());
  CurrentFeature += (FeatureNumber);

#ifdef DEBUG_TIMING
  End = clock();
  ExploreTiming.AddTime("POPULATION::IsContinuous", Start, End);
#endif

  return (*CurrentFeature).IsContinuous();
}

/**********************************************************************
Function: GetNoOperators()
Category: Selectors
Scope: public
In: -
Out: unsigned int, number of cutoffs
Description: Returns the type of a partition.
**********************************************************************/
PARTITION_TYPE POPULATION::GetPartitionType(unsigned int PartitionNumber) {
#ifdef DEBUG_TIMING
  clock_t Start, End;
  Start = clock();
#endif

  PARTITION_TYPE Type;
  vector<PARTITION>::iterator CurrentPartition(Partitions.begin());
  CurrentPartition += (PartitionNumber);
  Type = (*CurrentPartition).GetType();
  return Type;

#ifdef DEBUG_TIMING
  End = clock();
  ExploreTiming.AddTime("POPULATION::GetPartitionType", Start, End);
#endif
}

/**********************************************************************
Function: OperatorSet()
Category: Selectors
Scope: public
In: -
Out: bool, operators have/have not been set
Description: Checks whether operators have been set for a specific feature.
**********************************************************************/
bool POPULATION::OperatorSet(unsigned int FeatureNumber) {
#ifdef DEBUG_TIMING
  clock_t Start, End;
  Start = clock();
#endif

  vector<FEATURE>::iterator CurrentFeature(Features.begin());
  CurrentFeature += (FeatureNumber);
  if ((*CurrentFeature).GetNoOperators()>0) {

    #ifdef DEBUG_TIMING
      End = clock();
      ExploreTiming.AddTime("POPULATION::OperatorSet", Start, End);
    #endif

    return true;
  }

#ifdef DEBUG_TIMING
  End = clock();
  ExploreTiming.AddTime("POPULATION::OperatorSet", Start, End);
#endif
  
  return false;
}

/**********************************************************************
Function: GetFeatures()
Category: Selectors
Scope: public
In: -
Out: vector<FEATURES>&, address of the vector of features.
Description: Returns a reference to the vector of features associated
with this population.
**********************************************************************/
vector<FEATURE>* POPULATION::GetFeatures() {                                    // Return vector of feature-pointers
  return &Features;
}

/**********************************************************************
Function: GetNoClasses()
Category: Constructors and Destructors
Scope: public
In: -
Out: unsigned int, number of classes
Description: Returns the number of classes in the population.
**********************************************************************/
unsigned int POPULATION::GetNoClasses() {
  return ClassRatios.size();
}

/**********************************************************************
Function: GetClassValueByName()
Category: Constructors and Destructors
Scope: public
In: string, ClassValue
Out: unsigned int, index
Description: Returns the index of a class value in classnames
**********************************************************************/
int POPULATION::GetClassValueByName(string ClassValue){
  list<string>::iterator CurrentClassName(ClassNames.begin());
  list<string>::iterator LastClassName(ClassNames.end());
  for (unsigned int i=0; CurrentClassName != LastClassName; CurrentClassName++) {
	if ((*CurrentClassName)== ClassValue) {
	  return i;
	}
	i++;
  }
  return -1;
}

/**********************************************************************
Function: GetFeatureNumber()
Category: Constructors and Destructors
Scope: public
In: string, FeatureName
Out: unsigned int, index
Description: Returns the index of a feature by name
**********************************************************************/
int POPULATION::GetFeatureNumber(string FeatureName){
  vector<FEATURE>::iterator CurrentFeature(Features.begin());
  vector<FEATURE>::iterator LastFeature(Features.end());

  for (unsigned int i=0; CurrentFeature != LastFeature; CurrentFeature++) {
	if ((*CurrentFeature).GetName()== FeatureName.c_str()) {
	  return i;
	}
	i++;
  }
  return -1;
}


/**********************************************************************
Function: GetClassNames()
Category: Selectors
Scope: public
In: -
Out: -
Description: Returns a pointer to a <list> of classnames.
**********************************************************************/
list<string>* POPULATION::GetClassNames() {
  return &ClassNames;
}

/**********************************************************************
Function: PrintSettings()
Category: Selectors
Scope: public
In: -
Out: -
Description: Prints all settings related to population.
**********************************************************************/
void POPULATION::PrintSettings() {
  cout << endl << "POPULATION SETTINGS" << endl << endl;
  switch (*PartitionMethod) {
    case RESUBSTITUTION:
      cout << "Partition method: Resubstitution" << endl;
      break;
    case HOLDOUT:
      cout << "Partition method: Holdout" << endl;
      cout << "Learn-ratio: " << ((*LearnRatio)*100) << " %" << endl;
      cout << "Randomize: ";
      if (*Randomize) {
		cout << "Yes" << " (seed:" << Seed << ")";
      } else {
        cout << "No";
      }
      cout << endl;
      break;
    case CROSS_VALIDATION:
      cout << "Partition method: Cross-validation" << endl;
      cout << "Number of partitions: " << Partitions.size() << endl;
      cout << "Randomize: ";
      if (*Randomize) {
        cout << "Yes" << " (seed:" << Seed << ")";;
      } else {
        cout << "No";
      }
      cout << endl;
      break;
  }
  cout << "Number of classes: " << NoClasses << endl;
  cout << "Number of observations: " << NoObservations << endl;

  list<string>::iterator CurrentClassName(ClassNames.begin());
  list<string>::iterator LastClassName(ClassNames.end());
  for (unsigned int i=0; CurrentClassName != LastClassName; CurrentClassName++) {
	if (i==(*PositiveClass)) {
	  cout << "Positive class: " << (*CurrentClassName) << endl;
	}
	i++;
  }

  PrintStratify();

  PrintMandatoryFeatures();
  PrintCutoffRanges();


}

/**********************************************************************
Function: PrintCutoffs()
Category: Selectors
Scope: public
In: -
Out: -
Description: Prints the cutoffs of each individual feature
associated with this population.
**********************************************************************/
void POPULATION::PrintCutoffs() {
#ifdef DEBUG_TIMING
  clock_t Start, End;
  Start = clock();
#endif

  vector<FEATURE>::iterator CurrentFeature(Features.begin());
  vector<FEATURE>::iterator LastFeature(Features.end());
  int tot=0;

  cout << endl << "CUTOFFS" << endl << endl;
  for (unsigned int i=1; CurrentFeature != LastFeature; CurrentFeature++) {
	cout << "Feature " << i << " (" << (*CurrentFeature).GetNoCutoffs() << "): ";
	(*CurrentFeature).PrintCutoffs();
	i++;
	tot += (*CurrentFeature).GetNoCutoffs();
  }
  cout << "\n";
  cout << "Total: " << tot << "\n" << "\n";
#ifdef DEBUG_TIMING
  End = clock();
  ExploreTiming.AddTime("POPULATION::PrintCutoffs", Start, End);
#endif

}

/**********************************************************************
Function: PrintOperators()
Category: Selectors
Scope: public
In: -
Out: -
Description: Prints the operator(s) of each individual feature
associated with this population.
**********************************************************************/
void POPULATION::PrintOperators() {
#ifdef DEBUG_TIMING
  clock_t Start, End;
  Start = clock();
#endif

  vector<FEATURE>::iterator CurrentFeature(Features.begin());
  vector<FEATURE>::iterator LastFeature(Features.end());

  cout << endl << "OPERATORS" << endl << endl;
  for (unsigned int i=1; CurrentFeature != LastFeature; CurrentFeature++) {
    cout << "Feature " << i << ": ";
    (*CurrentFeature).PrintOperators();
    i++;
  }

#ifdef DEBUG_TIMING
  End = clock();
  ExploreTiming.AddTime("POPULATION::PrintOperators", Start, End);
#endif

}

/**********************************************************************
Function: PrintPartitions()
Category: Selectors
Scope: public
In: -
Out: -
Description: Prints a summary of each partition associated with this
population.
**********************************************************************/
void POPULATION::PrintPartitions() {
#ifdef DEBUG_TIMING
  clock_t Start, End;
  Start = clock();
#endif

  vector<PARTITION>::iterator CurrentPartition(Partitions.begin());
  vector<PARTITION>::iterator LastPartition(Partitions.end());
  char* Type;

  cout << endl << "PARTITIONS (" << Partitions.size() << ")" << endl << endl;
  for (unsigned int i=1; CurrentPartition != LastPartition; CurrentPartition++) {
    unsigned int PartitionSize = (*CurrentPartition).GetSize();
    unsigned int ObservationOrder;
    OBSERVATION* CurrentObservation;

	vector<unsigned int> Classes;
	vector<unsigned int>::iterator CurrentClass;
	vector<unsigned int>::iterator LastClass;

	cout << "OBSERVATIONS PARTITION " << i << " (Type: ";
    if ((*CurrentPartition).GetType()==LEARN) {
      cout << "LEARN";
	} else if ((*CurrentPartition).GetType()==VALIDATION) {
	  cout << "VALIDATION";
	} else if ((*CurrentPartition).GetType()==TRAIN) {
        cout << "TRAIN";
    }
    cout << ")" << endl << endl;

    cout << "Index\tOrder\t";
    for (unsigned int i=0; i < Features.size(); i++) {
      cout << "Feat. " << i+1 << "\t";
    }
    cout << "Class" << endl;

	vector<FEATURE>::iterator CurrentFeature(Features.begin());
	vector<FEATURE>::iterator LastFeature(Features.end());

    for (unsigned int i=0; i < PartitionSize; i++) {
      ObservationOrder = (*CurrentPartition).GetObservation(i);

      CurrentFeature = Features.begin();
      LastFeature = Features.end();

      CurrentObservation = (*CurrentFeature).GetObservation(ObservationOrder);
      cout << (i+1) << "\t" << (*CurrentObservation).GetOrder() << "\t";
      for (; CurrentFeature != LastFeature; CurrentFeature++) {
        CurrentObservation = (*CurrentFeature).GetObservation(ObservationOrder);
        cout << (*CurrentObservation).GetValue() << "\t";
      }
      cout << (*CurrentObservation).GetClass() << endl;                  

      while (Classes.size() < ((*CurrentObservation).GetClass()+1)) {
        Classes.push_back(0);
      }

      CurrentClass = Classes.begin();
      CurrentClass += (*CurrentObservation).GetClass();
      (*CurrentClass)++;
    }

    LastClass = Classes.end();
    CurrentClass=Classes.begin();
    list<string>::iterator CurrentClassName(ClassNames.begin());
    cout << endl << "CLASS DISTRIBUTION" << endl << endl;
    cout << "No. Observations\tClass" << endl;
    for (unsigned int i=0; CurrentClass != LastClass; i++) {
      cout << (*CurrentClass) << "\t\t" << (*CurrentClassName) << endl;
      CurrentClass++;
      CurrentClassName++;
    }
    cout << "\t\t--" << endl;
    cout << "\t\t" << PartitionSize << " (" << (100 * (float)PartitionSize/(float)NoObservations) << "%)" << endl << endl;
    i++;
  }
  cout << endl;

#ifdef DEBUG_TIMING
  End = clock();
  ExploreTiming.AddTime("POPULATION::PrintPartitions", Start, End);
#endif
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
string POPULATION::GetPartitionData(PARTITION_TYPE Type) {

  ostringstream Output;

  //write featurenames to file
  vector<FEATURE>::iterator CurrentFeature(Features.begin());
  vector<FEATURE>::iterator LastFeature(Features.end());

 /* PR: removed because of data format used in other algorithms
	for (unsigned int i=0; CurrentFeature != LastFeature; CurrentFeature++) {
	 if (i>0) Output << ",";
		 Output << (*CurrentFeature).GetName();
	 i++;
	}
	Output << endl; */

  //write observations to str
  vector<PARTITION>::iterator CurrentPartition(Partitions.begin());
  vector<PARTITION>::iterator LastPartition(Partitions.end());
  list<string>::iterator CurrentClassName(ClassNames.begin());
  for (;CurrentPartition != LastPartition; CurrentPartition++) {

    unsigned int PartitionSize = (*CurrentPartition).GetSize();
    unsigned int ObservationOrder;
    OBSERVATION* CurrentObservation;

    if ((*CurrentPartition).GetType()==Type){
      for (unsigned int i=0; i < PartitionSize; i++) {
        ObservationOrder = (*CurrentPartition).GetObservation(i);

        CurrentFeature = Features.begin();
        LastFeature = Features.end();

        for (unsigned int i=0; CurrentFeature != LastFeature; CurrentFeature++) {
          if (i>0) Output << ",";
          CurrentObservation = (*CurrentFeature).GetObservation(ObservationOrder);
          Output << (*CurrentObservation).GetValue();
          i++;
		}
	/* PR: Remove because of data format used in other algorithms
		   CurrentClassName=ClassNames.begin();
		   for (unsigned int i=0; i<(*CurrentObservation).GetClass();i++)
			CurrentClassName++;
			Output << "," << (*CurrentClassName) << endl; */

   	     Output << endl; 

	  }
    }
  }

  return Output.str();
}



/**********************************************************************
Function: ResetSeed()
Category: Selectors
Scope: private
In: -
Out: -
Description: Resets the SeedIsSet boolean neccesary for multiple runs
**********************************************************************/
void POPULATION::ResetSeed() {
  SeedIsSet = false;
}


/**********************************************************************
Function: InitialiseRandomNumber()
Category: Selectors
Scope: private
In: -
Out: -
Description: Initialise the random number generator.
**********************************************************************/
void POPULATION::InitialiseRandomNumber() {
#ifdef DEBUG_TIMING
  clock_t Start, End;
  Start = clock();
#endif

 if (!SeedIsSet) {    //(PR for multiple runs necessary !! problem with project file)
	time(&Seed);
	SeedIsSet = true;
 }

  if ((*Randomize)) {
    srand(Seed);                                                                // Initialise random number generator
  } else {
    srand(10000);                                                               // Fixed
  }

#ifdef DEBUG_TIMING
  End = clock();
  ExploreTiming.AddTime("POPULATION::InitialiseRandomNumber", Start, End);
#endif
}

/**********************************************************************
Function: GetRandomNumber()
Category: Selectors
Scope: private
In: unsigned int, maximum number.
Out: unsigned int, a random number.
Description: Returns a random number between 1 and UpperBound.
**********************************************************************/
unsigned int POPULATION::GetRandomNumber(unsigned int UpperBound) {
#ifdef DEBUG_TIMING
  clock_t Start, End;
  Start = clock();
#endif

  double FloatingPoint;                                                         // Needed to generator a random number
  unsigned int RandomNumber;                                                    // Where the random number is stored

  //FloatingPoint = ((double)rand() / (double)(RAND_MAX+1));                      // Add random observations to learn partition
  //RandomNumber = (unsigned int) ceil(FloatingPoint * UpperBound);               // Generate random number based by multiplying the FloatingPoint with the UpperBound

  if (UpperBound>0) {
   RandomNumber = rand() % UpperBound +1;
  } else {
   RandomNumber = 0;
  }

#ifdef DEBUG_TIMING
  End = clock();
  ExploreTiming.AddTime("POPULATION::GetRandomNumber", Start, End);
#endif
  
  return RandomNumber;
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
void POPULATION::AddCutoffRange(unsigned int FeatureNumber, Range NewRange) {
#ifdef DEBUG_TIMING
  clock_t Start, End;
  Start = clock();
#endif

  vector<FEATURE>::iterator CurrentFeature(Features.begin());
  CurrentFeature += FeatureNumber;
  (*CurrentFeature).AddCutoffRange(NewRange);

#ifdef DEBUG_TIMING
  End = clock();
  ExploreTiming.AddTime("POPULATION::SetCutoffRange", Start, End);
#endif

}

/**********************************************************************
Function: GetCutoffRanges()
Category: Modifiers
Scope: public
In: unsigned int, feature number
Out: vector<Range>*, pointer to vector of ranges
Description: Get a cutoff-range for a specific feature.
**********************************************************************/
vector<Range>* POPULATION::GetCutoffRanges(unsigned int FeatureNumber) {
  vector<FEATURE>::iterator CurrentFeature(Features.begin());
  CurrentFeature += FeatureNumber;
  return (*CurrentFeature).GetCutoffRanges();
}

/**********************************************************************
Function: GetOperator()
Category: Modifiers
Scope: public
In: unsigned int, feature number
Out: OPERATOR, operator associated with feature
Description: Get a cutoff-range for a specific feature.
**********************************************************************/
OPERATOR POPULATION::GetOperator(unsigned int FeatureNumber) {
  vector<FEATURE>::iterator CurrentFeature(Features.begin());
  CurrentFeature += FeatureNumber;
  return (*CurrentFeature).GetOperator();
}

/**********************************************************************
Function: SetPositiveClass()
Category: Modifiers
Scope: public
In: unsigned int*, class.
Out: -
Description: Assigns a class, for which a rule has to be generated, to the
population.
**********************************************************************/
void POPULATION::SetPositiveClass(unsigned int* PFeature, unsigned int* PClass) {
#ifdef DEBUG_TIMING
  clock_t Start, End;                                                 
  Start = clock();
#endif

  PositiveClass = PClass;
  vector<FEATURE>::iterator CurrentFeature(Features.begin());
  vector<FEATURE>::iterator ClassFeature(Features.begin());
  ClassFeature += *PFeature;                                                    //set ClassFeature
  OBSERVATION* TempObservation;
  OBSERVATION* ClassFeatureObservation;
  for (unsigned int i=0;i<GetNoFeatures();i++){
	(*CurrentFeature).SetPositiveClass(PClass);
	(*CurrentFeature).SetClassFeature(false);
	//set class correctly for the observations
	for (unsigned int k=0; k <(*CurrentFeature).GetNoObservations();k++){
	 TempObservation = (*CurrentFeature).GetObservation(k);
	 ClassFeatureObservation = (*ClassFeature).GetObservation(k);

	 list<string>::iterator CurrentClass(ClassNames.begin());
	 list<string>::iterator LastClass(ClassNames.end());
	 int c=0;
	 string temp;
	 string temp2;
	 while (((*CurrentClass).c_str()) != ((*ClassFeatureObservation).GetValue())){
	  temp =(*CurrentClass).c_str();
	  temp2 =(*ClassFeatureObservation).GetValue();
	  c++;
	  CurrentClass++;
	 }
	 TempObservation->SetClass(c);
	}
	CurrentFeature += 1;
  }
  (*ClassFeature).SetClassFeature(true);

#ifdef DEBUG_TIMING
  End = clock();
  ExploreTiming.AddTime("POPULATION::SetPositiveClass", Start, End);
#endif
}

/**********************************************************************
Function: SetNoClasses()
Category: Modifiers
Scope: public
In: unsigned int, number of classes.
Out: -
Description: Sets the number of classes found in the population, used
for partitioning with stratification.
**********************************************************************/
void POPULATION::SetNoClasses(unsigned int PNoClasses) {
  NoClasses = PNoClasses;
}

/**********************************************************************
Function: SetClassNames()
Category: Modifiers
Scope: public
In: list<string>, classnames
Out: -
Description: Sets the classnames for a specific population.
**********************************************************************/
void POPULATION::SetClassNames(list<string> NClassNames) {
  ClassNames = NClassNames;
}

/**********************************************************************
Function: SetFClassNames()
Category: Modifiers
Scope: public
In: list<string>, classnames
Out: -
Description: Sets the classnames for a specific population based on a feature.
**********************************************************************/
void POPULATION::SetClassNamesByFName(string ClassFName) {
  string dum;
  vector<FEATURE>::iterator CurrentFeature(Features.begin());
  while ((*CurrentFeature).GetName() != ClassFName){
   dum = (*CurrentFeature).GetName();
   CurrentFeature++;
  }

  ClassNames.clear();
  for (unsigned int i=1; i<(*CurrentFeature).GetNoObservations(); i++){

   list<string>::iterator CurrentClass(ClassNames.begin());
   list<string>::iterator LastClass(ClassNames.end());
   bool Found = false;
   for (unsigned int k=0; CurrentClass != LastClass && !Found; k++) {
	if ((*CurrentClass).c_str()==(*CurrentFeature).GetObservation(i)->GetValue()){
	  Found = true;
	}
	CurrentClass++;
   }
   if (!Found) {
	ClassNames.push_back((*CurrentFeature).GetObservation(i)->GetValue());      // Add Classname to list if needed
   }
  }
  SetNoClasses(ClassNames.size());
}

/**********************************************************************
Function: SetPartitionMethod()
Category: Modifiers
Scope: public
In: enum PARTITION_METHOD*, partitioning method.
Out: -
Description: Assign a partitioning method to the population.
**********************************************************************/
void POPULATION::SetPartitionMethod(PARTITION_METHOD* PMethod) {                // In case of resubstitution or leave-one-out
  PartitionMethod = PMethod;
}

/**********************************************************************
Function: SetCutoffMethod()
Category: Modifiers
Scope: public
In: enum CUTOFF_METHOD*, cutoff method.
Out: -
Description: Assign a cutoff method to the population.
**********************************************************************/
void POPULATION::SetCutoffMethod(CUTOFF_METHOD* PCutoffMethod) {                // Set cutoff method
  CutoffMethod = PCutoffMethod;
}

/**********************************************************************
Function: SetOperatorMethod()
Category: Modifiers
Scope: public
In: enum OPERATOR_METHOD*, operator method.
Out: -
Description: Assign an operator method to the population.
**********************************************************************/
void POPULATION::SetOperatorMethod(OPERATOR_METHOD* POperatorMethod) {          // Set operator method
  OperatorMethod = POperatorMethod;
}

/**********************************************************************
Function: SetLearnRatio()
Category: Modifiers
Scope: public
In: float*, ratio.
Out: -
Description: Assign a ratio used for dividing the population into a test
and learn partition.
**********************************************************************/
void POPULATION::SetLearnRatio(float* PLearnRatio) {                            // In case of holdout method
  LearnRatio = PLearnRatio;
}

/**********************************************************************
Function: SetNoPartitions()
Category: Modifiers
Scope: public
In: unsigned int*, number of partitions.
Out: -
Description: Assign the number of partitions to the population.
**********************************************************************/
void POPULATION::SetNoPartitions(unsigned int* PNoPartitions) {                 // In case of random subsampling or cross-validation
  NoPartitions = PNoPartitions;
  PartitionNumber = 1;
}

/**********************************************************************
Function: SetRandomize()
Category: Modifiers
Scope: public
In: bool*, randomize yes/no
Out: -
Description: Set the option of randomizing the partitions or not.
**********************************************************************/
void POPULATION::SetRandomize(bool* Random) {
  Randomize = Random;
}

/**********************************************************************
Function: SetMandatoryFeature()
Category: Modifiers
Scope: public
In: unsigned int, feature number
    bool, mandatory or not
Out: -
Description: Make a feature mandatory or not.
**********************************************************************/
void POPULATION::SetMandatoryFeature(unsigned int FeatureNumber, bool IsMandatory) {
#ifdef DEBUG_TIMING
  clock_t Start, End;
  Start = clock();
#endif

  vector<FEATURE>::iterator CurrentFeature(Features.begin());
  CurrentFeature += FeatureNumber;
  (*CurrentFeature).SetMandatory(IsMandatory);

#ifdef DEBUG_TIMING
  End = clock();
  ExploreTiming.AddTime("POPULATION::SetMandatoryFeature", Start, End);
#endif
}

/**********************************************************************
Function: GetMandatoryFeature()
Category: Modifiers
Scope: public
In: unsigned int, feature number
    bool, mandatory or not
Out: -
Description: Make a feature mandatory or not.
**********************************************************************/
bool POPULATION::GetMandatoryFeature(unsigned int FeatureNumber) {
  vector<FEATURE>::iterator CurrentFeature(Features.begin());
  CurrentFeature += FeatureNumber;
  return (*CurrentFeature).IsMandatory();
}

/**********************************************************************
Function: PrintMandatoryFeatures()
Category: Modifiers
Scope: public
In: -
Out: -
Description: Print all mandatory features.
**********************************************************************/
void POPULATION::PrintMandatoryFeatures() {
#ifdef DEBUG_TIMING
  clock_t Start, End;
  Start = clock();
#endif

  vector<FEATURE>::iterator CurrentFeature(Features.begin());
  vector<FEATURE>::iterator LastFeature(Features.end());
  bool Found = false;
  
  cout << endl << "MANDATORY FEATURES" << endl << endl;
  for (; CurrentFeature != LastFeature; CurrentFeature++) {
    if ((*CurrentFeature).IsMandatory()) {
      Found = true;
      cout << "Feature " << (*CurrentFeature).GetOrder() + 1 << " (" << (*CurrentFeature).GetName() << ")" << endl;
    }
  }

  if (!Found) {
    cout << "None." << endl;
  }

#ifdef DEBUG_TIMING
  End = clock();
  ExploreTiming.AddTime("POPULATION::PrintMandatoryFeatures", Start, End);
#endif
}

/**********************************************************************
Function: PrintCutoffRanges()
Category: Modifiers
Scope: public
In: -
Out: -
Description: Print all ranges set for any features.
**********************************************************************/
void POPULATION::PrintCutoffRanges() {
#ifdef DEBUG_TIMING
  clock_t Start, End;
  Start = clock();
#endif

  vector<FEATURE>::iterator CurrentFeature(Features.begin());
  vector<FEATURE>::iterator LastFeature(Features.end());
  vector<Range>* Dummy;
  bool Found = false;

  cout << endl << "CUTOFF RANGE" << endl << endl;
  for (; CurrentFeature != LastFeature; CurrentFeature++) {
    if ((*CurrentFeature).IsCutoffRangeSet()) {
      Dummy = (*CurrentFeature).GetCutoffRanges();
      vector<Range>::iterator CurrentRange((*Dummy).begin());
      vector<Range>::iterator LastRange((*Dummy).end());
      for (; CurrentRange != LastRange; CurrentRange++) {
        cout << "Feature " << (*CurrentFeature).GetOrder() + 1 << " (" << (*CurrentFeature).GetName() << "): Min: " << (*CurrentRange).Min << " Max: " << (*CurrentRange).Max << endl;
        Found = true;
      }
    }
  }

  if (!Found) {
    cout << "None." << endl;
  }

#ifdef DEBUG_TIMING
  End = clock();
  ExploreTiming.AddTime("POPULATION::PrintCutoffRanges", Start, End);
#endif
}

/**********************************************************************
Function: Partition()
Category: Modifiers
Scope: public
In: -
Out: bool, population could/could not be partitioned
Description: Decide with function to call for partitioning the population in learn
			 and validation set.
**********************************************************************/
bool POPULATION::Partition() {                                                  // Partition a population according to method PartitionMethod
#ifdef DEBUG_TIMING
  clock_t Start, End;
  Start = clock();
#endif

  bool Result;

  switch (*PartitionMethod) {
    case RESUBSTITUTION:
	  Result = Resubstitute();                                                  // Create one partition with all observations in it, set type to LEARNTEST
      break;
    case HOLDOUT:
      Stratify();
      Result = Holdout();
      break;
    case LEAVE_ONE_OUT:
      Result = LeaveOneOut();                                                   // Create learn partition based on N - 1 observations, decrease partition by one each time, test on leftover
      break;
    case CROSS_VALIDATION:
      Stratify();
	  Result = CrossValidate();                                                 // Create k partitions, use each partition sequentially to learn, other partitions to test
	  break;
  }

  InitialisePartitions();                                                       // Set observations for learning/testing and add any Classes

  if (Result) {

    IsPartitioned = true;

#ifdef DEBUG_TIMING
  End = clock();
  ExploreTiming.AddTime("POPULATION::Partition", Start, End);
#endif

    return true;
  }

#ifdef DEBUG_TIMING
  End = clock();
  ExploreTiming.AddTime("POPULATION::Partition", Start, End);
#endif

  return false;
}

/**********************************************************************
Function: ClearPartitions()
Category: Modifiers
Scope: public
In: -
Out: -
Description: Delete all partitions of the population without deleting
actual features or their observations.
**********************************************************************/
void POPULATION::ClearPartitions() {
  Partitions.clear();
  ClassRatios.clear();

  IsPartitioned = false;

  vector<FEATURE>::iterator CurrentFeature(Features.begin());
  vector<FEATURE>::iterator LastFeature(Features.end());

  for (; CurrentFeature != LastFeature; CurrentFeature++) {
    (*CurrentFeature).ClearClassObservations();
  }
  
}

/**********************************************************************
Function: InitialisePartitions()
Category: Modifiers
Scope: public
In: -
Out: -
Description: Propagate the partitions to each feature associated with
this population.
**********************************************************************/
void POPULATION::InitialisePartitions() {
#ifdef DEBUG_TIMING
  clock_t Start, End;
  Start = clock();
#endif

  unsigned int CurrentClass, PartitionSize, Order;

  vector<PARTITION>::iterator CurrentPartition(Partitions.begin());
  vector<PARTITION>::iterator LastPartition(Partitions.end());

  vector<FEATURE>::iterator CurrentFeature(Features.begin());
  vector<FEATURE>::iterator LastFeature(Features.end());

  CLASSES* TempClass;

  OBSERVATION* CurrentObservation;
  PARTITION_TYPE PartitionType;

  for (CurrentFeature = Features.begin(); CurrentFeature != LastFeature; CurrentFeature++) {
    (*CurrentFeature).InitialiseClasses();                                      // Sort feature classes and partitioned observations of each feature
  }

  for (; CurrentPartition != LastPartition; CurrentPartition++) {               // Iterate through partitions
    PartitionSize = (*CurrentPartition).GetSize();                              // Get current partition size
    PartitionType = (*CurrentPartition).GetType();                              // Get current partition type (learn/test/learntest)

    for (unsigned int i=0; i < PartitionSize; i++) {                            // Interate through observations of each partition
      Order = (*CurrentPartition).GetObservation(i);                            // Get the next observation number of the current partition

      for (CurrentFeature = Features.begin(); CurrentFeature != LastFeature; CurrentFeature++) {  // Iterate through features
        CurrentObservation = (*CurrentFeature).GetObservation(Order);                             // Get the actual observation of this feature
        (*CurrentFeature).AddClassObservation(CurrentObservation, PartitionType);                 // Initialise this partition observation for each feature
      }
    }
  }

  // Important for correct cutoff heuristic results!
  for (CurrentFeature = Features.begin(); CurrentFeature != LastFeature; CurrentFeature++) {
    (*CurrentFeature).SortPartitions();                                         // Sort LearnObservations and TestObservations
  }

#ifdef DEBUG_TIMING
  End = clock();
  ExploreTiming.AddTime("POPULATION::InitialisePartitions", Start, End);
#endif
}

/**********************************************************************
Function: Stratify()
Category: Modifiers
Scope: private
In: -
Out: -
Description: Make a vector of StratifyClasses that compare the
number of observations per class with the total number of observations.
**********************************************************************/
void POPULATION::Stratify() {
#ifdef DEBUG_TIMING
  clock_t Start, End;
  Start = clock();
#endif

  vector<FEATURE>::iterator RefFeature(Features.begin());                       // Reference feature for retrieving classes
  OBSERVATION* CurrentObservation;                                              // Pointer to the current observation to be partitioned
  ClassRatio* CurrentClassRatio;
  unsigned int Class,Order,no;
  bool Found=false;
  ostringstream Result;

  StratifyOutput.clear();

  for (unsigned int i=0; i < NoObservations; i++) {
    CurrentObservation = (*RefFeature).GetObservation(i);
    Class = (*CurrentObservation).GetClass();

    vector<ClassRatio>::iterator CurrentClass(ClassRatios.begin());
    vector<ClassRatio>::iterator LastClass(ClassRatios.end());

    Found = false;
    for (; CurrentClass != LastClass; CurrentClass++) {
      if ((*CurrentClass).Class==Class) {
        Found = true;
      }
    }
    if (!Found) {
      ClassRatio Dummy;
      Dummy.Class = Class;
      ClassRatios.push_back(Dummy);
    }
  }

  ClassRatios.clear();
  for (unsigned int i=0; i<NoClasses; i++) {
    ClassRatio Dummy;
    Dummy.Class = i;
    ClassRatios.push_back(Dummy);
  }

  for (unsigned int i=0; i < NoObservations; i++) {                            // Fill the vector with all observations
    CurrentObservation = (*RefFeature).GetObservation(i);                       // Get the actual observation being referenced
    Class = (*CurrentObservation).GetClass();
    Order = (*CurrentObservation).GetOrder();
    CurrentClassRatio = &ClassRatios.at(Class);
    (*CurrentClassRatio).Observations.push_back(Order);
  }

  Result << endl << "CLASSES" << endl << endl;
  Result << "Class\t\tSize\tRatio" << endl;

  vector<ClassRatio>::iterator CurrentClass(ClassRatios.begin());
  list<string>::iterator CurrentClassName(ClassNames.begin());
  for (unsigned int i=0; i < NoClasses; i++) {
    (*CurrentClass).Ratio = (float)((float)(*CurrentClass).Observations.size() / (float)NoObservations);
     Result << (*CurrentClassName) << "\t\t" << (*CurrentClass).Observations.size() << "\t" << (*CurrentClass).Ratio << endl;
     CurrentClass++;
     CurrentClassName++;
  }

  Result << endl;

  StratifyOutput = Result.str();

#ifdef DEBUG_TIMING
  End = clock();
  ExploreTiming.AddTime("POPULATION::Stratify", Start, End);
#endif
}

/**********************************************************************
Function: GetSeed()
Category: Selectors
Scope: public
In: -
Out: long double, the seed which is a cast from time_t
Description: Get the seed used to randomize the population.
**********************************************************************/
long double POPULATION::GetSeed() {
  return Seed;
}

/**********************************************************************
Function: SetSeed()
Category: Modifiers
Scope: public
In: long double, the seed which is a cast from time_t
Out: bool, setting the seed succeeded/did not succeed
Description: Set the seed used to randomize the population.
**********************************************************************/
void POPULATION::SetSeed(long double NewSeed) {
  Seed = NewSeed;
  SeedIsSet = true;
}   

/**********************************************************************
Function: Resubstitute()
Category: Modifiers
Scope: private
In: -
Out: -
Description: Partition the population according to the 'resubstitution'
method.
**********************************************************************/
bool POPULATION::Resubstitute() {                                               // One partition for both learning and testing
#ifdef DEBUG_TIMING
  clock_t Start, End;
  Start = clock();
#endif

  if (IsPartitioned) {
    return false;
  } else {
    Partitions.clear();                                                         // Clear any partitions
	AddPartition(LEARN);  //TRAIN                                                  // Add a single learn and test partition
	vector<PARTITION>::iterator CurrentPartition(Partitions.begin());

    for (unsigned int i=0; i < NoObservations; i++) {                           // Add each observation to the partition
      (*CurrentPartition).AddObservation(i);
    }
    return true;
  }

#ifdef DEBUG_TIMING
  End = clock();
  ExploreTiming.AddTime("POPULATION::Resubstitute", Start, End);
#endif
}

/**********************************************************************
Function: Holdout()
Category: Modifiers
Scope: private
In: -
Out: -
Description: Partition the population according to the 'holdout' method.
**********************************************************************/
bool POPULATION::Holdout() {                                                    // Partition in learn and test using LearnRatio
#ifdef DEBUG_TIMING
  clock_t Start, End;
  Start = clock();
#endif
  if (IsPartitioned) {                                                          // Always repartition if Randomize is set
    if (*Randomize){
     IsPartitioned = false;
     Holdout();
    }
    return false;
  } else {
    unsigned int RandomNumber, UpperBound, NoClassObs, PartitionSize;

    Partitions.clear();                                                         // Clear any existing partitions

    AddPartition(LEARN);
	AddPartition(VALIDATION);

    InitialiseRandomNumber();

	vector<PARTITION>::iterator CurrentPartition(Partitions.begin());           // Iterator for partitions
    vector<PARTITION>::iterator LastPartition(Partitions.end());                // Last partition

    vector<unsigned int>::iterator CurrentObservation;
    vector<unsigned int>::iterator LastObservation;

    PartitionSize = (unsigned int)((*LearnRatio) * (float)NoObservations);      // How big does each partition have to be?

    for (; CurrentPartition != LastPartition; CurrentPartition++) {             // Iterate through partitions
      vector<ClassRatio>::iterator CurrentClass(ClassRatios.begin());           // Class iterator
      vector<ClassRatio>::iterator LastClass(ClassRatios.end());                // Last class
      if (CurrentPartition == Partitions.begin()) {                             // Is this the learning partition?
        while (CurrentClass != LastClass) {                                     // Iterate through classes
          NoClassObs = (unsigned int)((*CurrentClass).Ratio * (float)PartitionSize);
                                                                                // Calculate number of observations have to be included into this partition for this class
          for (unsigned int i=0; i < NoClassObs; i++) {                         // Iterate until we have the number of observations needed for this class
            RandomNumber = GetRandomNumber((*CurrentClass).Observations.size()-1);
                                                                                // Choose a random number in the range of the number of observations in the class
            CurrentObservation = (*CurrentClass).Observations.begin();
            CurrentObservation += (RandomNumber);
            (*CurrentPartition).AddObservation(*CurrentObservation);            // Add the observation to this partition
            (*CurrentClass).Observations.erase(CurrentObservation);
          }
          CurrentClass++;
        }
      } else {
        while (CurrentClass != LastClass) {                                     // Iterate through classes
          CurrentObservation = (*CurrentClass).Observations.begin();
          LastObservation = (*CurrentClass).Observations.end();
          while (CurrentObservation != LastObservation) {
            (*CurrentPartition).AddObservation(*CurrentObservation);
            CurrentObservation++;
          }
          CurrentClass++;
        }
      }
    }
     IsPartitioned=true;

     return true;

  }
#ifdef DEBUG_TIMING
  End = clock();
  ExploreTiming.AddTime("POPULATION::Holdout", Start, End);
#endif
}

/**********************************************************************
Function: LeaveOneOut()
Category: Modifiers
Scope: private
In: -
Out: -
Description: Partition the population according to the 'leave-one-out'
method.
**********************************************************************/
bool POPULATION::LeaveOneOut() {                                                // Create learning partition based on N - 1 observations, iterate N times, decrease partition by one each time
#ifdef DEBUG_TIMING
  clock_t Start, End;
  Start = clock();
#endif

  if (IsPartitioned) {
    return false;
  } else {
    vector<PARTITION>::iterator CurrentPartition;                               // Set partition iterator to the first partition
    Partitions.clear();                                                         // Clear any existing partitions

    AddPartition(LEARN);                                                        // Create a learning partition
    CurrentPartition = Partitions.begin();
    for (unsigned int i=0; i<NoObservations; i++) {                             // Add all observations except last one
      (*CurrentPartition).AddObservation(i);
    }

    AddPartition(VALIDATION);                                                         // Create testing partition
    CurrentPartition = Partitions.end();                                        // Set partition iterator to the next partition
    (*CurrentPartition).AddObservation(NoObservations);                         // Add last observation
    return true;
  }

#ifdef DEBUG_TIMING
  End = clock();
  ExploreTiming.AddTime("POPULATION::LeaveOneOut", Start, End);
#endif
}

/**********************************************************************
Function: CrossValidate()
Category: Modifiers
Scope: private
In: -
Out: -
Description: Partition the population according to the 'cross-validation'
method.
**********************************************************************/
bool POPULATION::CrossValidate() {                                              // Create k partitions, use each partition sequentially to create classifier, use other partitions to test
#ifdef DEBUG_TIMING
  clock_t Start, End;
  Start = clock();
#endif

  if (IsPartitioned) {
    vector<PARTITION>::iterator CurrentPartition(Partitions.begin());
    vector<PARTITION>::iterator LastPartition(Partitions.end());

	for (; CurrentPartition != LastPartition; CurrentPartition++) {
	  if ((*CurrentPartition).GetType()==VALIDATION) {
		(*CurrentPartition).SetType(LEARN);
		CurrentPartition++;
		if (CurrentPartition != LastPartition) {
		  (*CurrentPartition).SetType(VALIDATION);
		} else {
		  if (*Randomize) {
			IsPartitioned = false;                                              // Randomize partitions again for next run
			CrossValidate();                                                    // Call CrossValidate again to make first sets
		  }
		  else ResetPartitions();                                               // Cross-validation: reset partitions without re-partitioning for other runs
		  return false;
		}
	  }
	}
	return true;
  } else {
	unsigned int UpperBound, NoClassObs;
	float PartitionSize;
	int RandomNumber;
	bool TestDone = false;

	Partitions.clear();                                                         // Clear any existing partitions

	AddPartition(VALIDATION);

	InitialiseRandomNumber();

	vector<PARTITION>::iterator CurrentPartition(Partitions.begin());           // Iterator for partitions

	vector<unsigned int>::iterator CurrentObservation;
    vector<unsigned int>::iterator LastObservation;

    PartitionSize = ((float)NoObservations/(float)(*NoPartitions)); // How big does each partition have to be?

    for (; Partitions.size() != (*NoPartitions);) {                             // Iterate through partitions
      vector<ClassRatio>::iterator CurrentClass(ClassRatios.begin());           // Class iterator
	  vector<ClassRatio>::iterator LastClass(ClassRatios.end());                // Last class
	  if (!TestDone) {
		  TestDone = true;
          while (CurrentClass != LastClass) {                                   // Iterate through classes
            NoClassObs = (unsigned int)((*CurrentClass).Ratio * PartitionSize);// Calculate number of observations have to be included into this partition for this class
            for (unsigned int i=0; i < NoClassObs; i++) {                       // Iterate until we have the number of observations needed for this class
              RandomNumber = GetRandomNumber((*CurrentClass).Observations.size()-1);// Choose a random number in the range of the number of observations in the class
              CurrentObservation = (*CurrentClass).Observations.begin();
              CurrentObservation += (RandomNumber);
              (*CurrentPartition).AddObservation(*CurrentObservation);          // Add the observation to this partition
              (*CurrentClass).Observations.erase(CurrentObservation);
            }
            CurrentClass++;
          }
	  } else {
		  AddPartition(LEARN);
		  CurrentPartition = Partitions.end();
		  CurrentPartition--;
		  while (CurrentClass != LastClass) {                                   // Iterate through classes
			NoClassObs = (unsigned int)((*CurrentClass).Ratio * PartitionSize);// Calculate number of observations have to be included into this partition for this class
			for (unsigned int i=0; i < NoClassObs; i++) {                       // Iterate until we have the number of observations needed for this class
			  RandomNumber = GetRandomNumber((*CurrentClass).Observations.size()-1);// Choose a random number in the range of the number of observations in the class
			  CurrentObservation = (*CurrentClass).Observations.begin();
			  CurrentObservation += (RandomNumber);
			  (*CurrentPartition).AddObservation(*CurrentObservation);          // Add the observation to this partition
			  (*CurrentClass).Observations.erase(CurrentObservation);
			}
			CurrentClass++;
		  }
	  }
	}

    // Add rest of observations random to partitions with max 1 per partition
    vector<ClassRatio>::iterator CurrentClass(ClassRatios.begin());             // Class iterator
    vector<ClassRatio>::iterator LastClass(ClassRatios.end());                  // Last class
    list <int> Partitionlist;
	for (unsigned int i=0;i< (*NoPartitions);i++)
      Partitionlist.push_back(i);

    while (CurrentClass != LastClass) {
      Partitionlist.clear();
	  for (unsigned int i=0;i< (*NoPartitions);i++)                                      // Fill left partition list
        Partitionlist.push_back(i);

	  CurrentObservation = (*CurrentClass).Observations.begin();
	  LastObservation = (*CurrentClass).Observations.end();
	  while (CurrentObservation != LastObservation) {
	   RandomNumber = GetRandomNumber(Partitionlist.size()-1);                  // Choose a random number in the range of the number of left partitions to fill
       list<int>::iterator CurrentLeftPartition(Partitionlist.begin());         // Iterator for left partitions
       vector<PARTITION>::iterator CurrentPartition(Partitions.begin());        // Iterator for partitions
       for (int j=0; j<RandomNumber; j++)
         CurrentLeftPartition++;                                                // Select to random selected left partition
       for (int j=0; j<(*CurrentLeftPartition); j++)
         CurrentPartition++;                                                    // Actually select that partition to add the observartion

       (*CurrentPartition).AddObservation(*CurrentObservation);                 // Add the observation to this partition
       (*CurrentClass).Observations.erase(CurrentObservation);
       CurrentObservation = (*CurrentClass).Observations.begin();
	   LastObservation = (*CurrentClass).Observations.end();
       Partitionlist.erase(CurrentLeftPartition);                               // Remove this partition form the left partitions
      }
      CurrentClass++;
    }
    IsPartitioned = true;

    #ifdef DEBUG_TIMING
      End = clock();
      ExploreTiming.AddTime("POPULATION::CrossValidate", Start, End);
    #endif

    return true;
  }
}


/**********************************************************************
Function: GenerateTestPartition()
Category: Modifiers
Scope: private
In: -
Out: -
Description: Partition the learn partition in 2/3 learn and 1/3 test
			 only works for now in a cross-validation experiment
**********************************************************************/
void POPULATION::GenerateTestPartition() {
#ifdef DEBUG_TIMING
  clock_t Start, End;
  Start = clock();
#endif
  int t1,t2,t3,i;
  if (IsPartitioned) {
	vector<PARTITION>::iterator CurrentPartition(Partitions.begin());
	vector<PARTITION>::iterator LastPartition(Partitions.end());

	//generate 3 unique random numbers to select 3 partitions as test set
	t1=0;
	while (t1==0) t1=GetRandomNumber(9);
	t2=t1;
	t3=t1;
	while (t2==t1 || t2==0) t2=GetRandomNumber(9);
	while (t3==t1 || t3==t2 || t3==0) t3=GetRandomNumber(9);
    i=0;
	for (; CurrentPartition != LastPartition; CurrentPartition++) {
	  if ((*CurrentPartition).GetType()==LEARN) {
		i++;
		if ((i==t1) || (i==t2) || (i==t3)) {
//		if ((i==t1)) {
		 (*CurrentPartition).SetType(VALIDATION);
		}
	  }
	}

  }
}


void POPULATION::ResetPartitions() {
  vector<PARTITION>::iterator CurrentPartition(Partitions.begin());
  vector<PARTITION>::iterator LastPartition(Partitions.end());

  (*CurrentPartition).SetType(VALIDATION);
  for (CurrentPartition++; CurrentPartition != LastPartition; CurrentPartition++) {
	(*CurrentPartition).SetType(LEARN);
  }
}

void POPULATION::ResetTestPartitions() {
  vector<PARTITION>::iterator CurrentPartition(Partitions.begin());
  vector<PARTITION>::iterator LastPartition(Partitions.end());

   for (CurrentPartition; CurrentPartition != LastPartition; CurrentPartition++) {
	 if ((*CurrentPartition).GetType()==VALIDATION)
		(*CurrentPartition).SetType(LEARN);
   }
}

/**********************************************************************
Function: AddPartition()
Category: Modifiers
Scope: private
In: const PARTITION_TYPE, type of partition.
Out: -
Description: Add a partition to the population.
**********************************************************************/
void POPULATION::AddPartition(const PARTITION_TYPE PType) {
#ifdef DEBUG_TIMING
  clock_t Start, End;
  Start = clock();
#endif

  Partitions.push_back(PARTITION(PType));                                       // Add a new partition with type PType to list of partitions

#ifdef DEBUG_TIMING
  End = clock();
  ExploreTiming.AddTime("POPULATION::AddPartition", Start, End);
#endif
}

/**********************************************************************
Function: RemovePartition()
Category: Modifiers
Scope: public
In: unsigned int, partition number.
Out: -
Description: Remove a partition from the population.
**********************************************************************/
void POPULATION::RemovePartition(unsigned int PNumber) {
#ifdef DEBUG_TIMING
  clock_t Start, End;
  Start = clock();
#endif

  if (PNumber<Partitions.size()) {
    vector<PARTITION>::iterator CurrentPartition(Partitions.begin()+PNumber);   // Iterator needed to select a partition
    Partitions.erase(CurrentPartition);                                         // Remove the partition
  }

#ifdef DEBUG_TIMING
  End = clock();
  ExploreTiming.AddTime("POPULATION::RemovePartition", Start, End);
#endif
}

/**********************************************************************
Function: AddFeature()
Category: Modifiers
Scope: private
In: CUTOFF_METHOD*, cutoff method.
    OPERATOR_METHOD*, operator method.
Out: -
Description: Add a feature to the population.
**********************************************************************/
void POPULATION::AddFeature(string FeatureName, CUTOFF_METHOD* CutoffMethod, OPERATOR_METHOD* OperatorMethod, bool Continuous) {
#ifdef DEBUG_TIMING
  clock_t Start, End;
  Start = clock();
#endif

  Features.push_back(FEATURE(FeatureName,
                             Features.size(),
                             CutoffMethod,
                             OperatorMethod,
							 PositiveClass,
							 Continuous));                                   // Add to the end of any existing features

#ifdef DEBUG_TIMING
  End = clock();
  ExploreTiming.AddTime("POPULATION::AddFeature", Start, End);
#endif
}

/**********************************************************************
Function: AddObservation()
Category: Modifiers
Scope: private
In: unsigned int, feature number
    unsigned int, order of the observation
    unsigned int, class of the observation
	string, value of the observation
Out: -
Description: Add an obervation to the population.
**********************************************************************/
void POPULATION::AddObservation(unsigned int FeatureNumber, string Value, unsigned int Class) {
#ifdef DEBUG_TIMING
  clock_t Start, End;
  Start = clock();
#endif

  vector<FEATURE>::iterator CurrentFeature(Features.begin());
  vector<FEATURE>::iterator LastFeature(Features.end());

  CurrentFeature += FeatureNumber;
  (*CurrentFeature).AddObservation(Value,Class);

  unsigned int MaxObservations=0;
  for (CurrentFeature = Features.begin(); CurrentFeature != LastFeature; CurrentFeature++) {
	if ((*CurrentFeature).GetNoObservations()>MaxObservations) {
      MaxObservations = (*CurrentFeature).GetNoObservations();
    }
  }
  
  NoObservations = MaxObservations;

#ifdef DEBUG_TIMING
  End = clock();
  ExploreTiming.AddTime("POPULATION::AddObservation", Start, End);
#endif
}

/**********************************************************************
Function: FindCutoffs()
Category: Modifiers
Scope: public
In: -
Out: -
Description: Find cutoffs for each feature associated with this population.
**********************************************************************/
void POPULATION::FindCutoffs() {                                                // Find cutoffs for each feature in population
#ifdef DEBUG_TIMING
  clock_t Start, End;
  Start = clock();
#endif

  vector<FEATURE>::iterator CurrentFeature(Features.begin());
  vector<FEATURE>::iterator LastFeature(Features.end());

  for (; CurrentFeature != LastFeature; CurrentFeature++) {
    (*CurrentFeature).FindCutoffs();                                            // Find actual cutoffs for this feature
  }

#ifdef DEBUG_TIMING
  End = clock();
  ExploreTiming.AddTime("POPULATION::FindCutoffs", Start, End);
#endif
}

/**********************************************************************
Function: FindOperators()
Category: Modifiers
Scope: public
In: -
Out: -
Description: Find operators for each feature associated with this population.
**********************************************************************/
void POPULATION::FindOperators() {                                              // Find cutoffs for each observation in learning partition
#ifdef DEBUG_TIMING
  clock_t Start, End;
  Start = clock();
#endif

  vector<FEATURE>::iterator CurrentFeature(Features.begin());
  vector<FEATURE>::iterator LastFeature(Features.end());

  for (; CurrentFeature != LastFeature; CurrentFeature++) {
    (*CurrentFeature).FindOperators();                                          // Find actual cutoffs for this feature with specified class
  }

#ifdef DEBUG_TIMING
  End = clock();
  ExploreTiming.AddTime("POPULATION::FindOperators", Start, End);
#endif
}

/**********************************************************************
Function: AddCutoff()
Category: Modifiers
Scope: public
In: unsigned int, feature number
    float, cutoff value
Out: -
Description: Manually add a cutoff to a feature.
**********************************************************************/
void POPULATION::AddCutoff(unsigned int FeatureNumber, string CutoffValue) {
#ifdef DEBUG_TIMING
  clock_t Start, End;
  Start = clock();
#endif

  if (FeatureNumber<Features.size()) {
    vector<FEATURE>::iterator CurrentFeature(Features.begin()+FeatureNumber);
    (*CurrentFeature).AddCutoff(CutoffValue);
  }
  
#ifdef DEBUG_TIMING
  End = clock();
  ExploreTiming.AddTime("POPULATION::AddCutoff", Start, End);
#endif
}

/**********************************************************************
Function: RemoveCutoff()
Category: Modifiers
Scope: public
In: unsigned int, feature number
    unsigned int, cutoff number
Out: -
Description: Manually remove a cutoff from a feature by number.
**********************************************************************/
void POPULATION::RemoveCutoff(unsigned int FeatureNumber, unsigned int CutoffNumber) {
#ifdef DEBUG_TIMING
  clock_t Start, End;
  Start = clock();
#endif

  if (FeatureNumber<Features.size()) {
    vector<FEATURE>::iterator CurrentFeature(Features.begin()+FeatureNumber);
    (*CurrentFeature).RemoveCutoff(CutoffNumber);
  }

#ifdef DEBUG_TIMING
  End = clock();
  ExploreTiming.AddTime("POPULATION::RemoveCutoff", Start, End);
#endif
}

/**********************************************************************
Function: RemoveCutoff()
Category: Modifiers
Scope: public
In: unsigned int, feature number
    float, cutoff value
Out: -
Description: Manually remove a cutoff from a feature by value.
**********************************************************************/
void POPULATION::RemoveCutoff(unsigned int FeatureNumber, string CutoffValue) {
#ifdef DEBUG_TIMING
  clock_t Start, End;
  Start = clock();
#endif

  if (FeatureNumber<Features.size()) {
    vector<FEATURE>::iterator CurrentFeature(Features.begin()+FeatureNumber);
    (*CurrentFeature).RemoveCutoff(CutoffValue);
  }

#ifdef DEBUG_TIMING
  End = clock();
  ExploreTiming.AddTime("POPULATION::RemoveCutoff", Start, End);
#endif
}

/**********************************************************************
Function: RemoveFeatureCutoffs()
Category: Modifiers
Scope: public
In: unsigned int, feature number
Out: -
Description: Remove all cutoffs from a specific feature.
**********************************************************************/
void POPULATION::RemoveFeatureCutoffs(unsigned int FeatureNumber) {
#ifdef DEBUG_TIMING
  clock_t Start, End;
  Start = clock();
#endif

  if (FeatureNumber<Features.size()) {
    vector<FEATURE>::iterator CurrentFeature(Features.begin()+FeatureNumber);
    (*CurrentFeature).RemoveAllCutoffs();
  }
  
#ifdef DEBUG_TIMING
  End = clock();
  ExploreTiming.AddTime("POPULATION::RemoveFeatureCutoffs", Start, End);
#endif
}

/**********************************************************************
Function: RemoveCutoffRange()
Category: Modifiers
Scope: public
In: unsigned int, feature number
    float, cutoff value
Out: -
Description: Manually remove a cutoff range from a feature by value.
**********************************************************************/
void POPULATION::RemoveCutoffRange(unsigned int FeatureNumber) {
#ifdef DEBUG_TIMING
  clock_t Start, End;
  Start = clock();
#endif

  if (FeatureNumber<Features.size()) {
    vector<FEATURE>::iterator CurrentFeature(Features.begin()+FeatureNumber);
    (*CurrentFeature).RemoveCutoffRanges();
  }

#ifdef DEBUG_TIMING
  End = clock();
  ExploreTiming.AddTime("POPULATION::RemoveCutoffRange", Start, End);
#endif
}

/**********************************************************************
Function: SetOperator()
Category: Modifiers
Scope: public
In: -
Out: -
Description: Manually set the operator for a feature
**********************************************************************/
void POPULATION::SetOperator(unsigned int FeatureNumber, OPERATOR FeatureOperator) {
#ifdef DEBUG_TIMING
  clock_t Start, End;
  Start = clock();
#endif

  if (FeatureNumber<Features.size()) {
    vector<FEATURE>::iterator CurrentFeature(Features.begin()+FeatureNumber);
    (*CurrentFeature).SetOperator(FeatureOperator);
  }

#ifdef DEBUG_TIMING
  End = clock();
  ExploreTiming.AddTime("POPULATION::SetOperator", Start, End);
#endif
}

/**********************************************************************
Function: SetPrintOperatorMethod()
Category: Modifiers
Scope: public
In: bool, yes or no
Out: -
Description: Indicate whether operator method information has to be
displayed in the result file.
**********************************************************************/
void POPULATION::SetPrintOperatorMethod(bool Setting) {
  IsPrintOperatorMethod = Setting;
}

/**********************************************************************
Function: PrintObservationsPerClass()
Category: Modifiers
Scope: public
In: bool, yes or no
Out: -
Description: Prints the total amount of observations for each class
specified in the population.
**********************************************************************/
void POPULATION::PrintObservationsPerClass() {
  vector<unsigned int> ObservationsPerClass;

  FEATURE* FeatureReference = &Features[0];
  ObservationsPerClass = (*FeatureReference).GetObservationsPerClass(ClassNames.size());

  list<string>::iterator CurrentClassName(ClassNames.begin());

  vector<unsigned int>::iterator CurrentObservationsPerClass(ObservationsPerClass.begin());
  vector<unsigned int>::iterator LastObservationsPerClass(ObservationsPerClass.end());  

  cout << endl << "CLASSES" << endl << endl;
  for (unsigned int i=0; CurrentObservationsPerClass != LastObservationsPerClass; i++) {
    cout << (*CurrentClassName) << ": " << (*CurrentObservationsPerClass) << " observations." << endl;
  
    CurrentObservationsPerClass++;
    CurrentClassName++;
  }
}

/**********************************************************************
Function: SetPrintCutoffMethod()
Category: Modifiers
Scope: public
In: bool, yes or no
Out: -
Description: Indicate whether cutoff method information has to be
displayed in the result file.
**********************************************************************/
void POPULATION::SetPrintCutoffMethod(bool Setting) {
  IsPrintCutoffMethod = Setting;
}

/**********************************************************************
Function: GetCutoffValues()
Category: Modifiers
Scope: public
In: unsigned int, featurenumber
Out: list<float>, the cutoff values
Description: Get a list of cutoff values for a specific feature.
**********************************************************************/
list<string> POPULATION::GetCutoffValues(unsigned int FeatureNumber) {
  if (FeatureNumber<Features.size()) {
    vector<FEATURE>::iterator CurrentFeature(Features.begin()+FeatureNumber);
    return (*CurrentFeature).GetCutoffValues();
  }
  list<string> Dummy;
  return Dummy;
}


/**********************************************************************
Function: PrintOperatorMethod()
Category: Modifiers
Scope: public
In: -
Out: -
Description:
**********************************************************************/
void POPULATION::PrintOperatorMethod() {
  vector<FEATURE>::iterator CurrentFeature(Features.begin());
  vector<FEATURE>::iterator LastFeature(Features.end());

  cout << endl << "OPERATOR METHOD" << endl;
  for (; CurrentFeature != LastFeature; CurrentFeature++) {
    cout << (*CurrentFeature).PrintOperatorMethod();
  }
}

/**********************************************************************
Function: PrintCutoffMethod()
Category: Modifiers
Scope: public
In: -
Out: -
Description:
**********************************************************************/
void POPULATION::PrintCutoffMethod() {
  vector<FEATURE>::iterator CurrentFeature(Features.begin());
  vector<FEATURE>::iterator LastFeature(Features.end());

  cout << endl << "CUTOFF METHOD" << endl << endl;
  for (; CurrentFeature != LastFeature; CurrentFeature++) {
    cout << (*CurrentFeature).PrintCutoffMethod();
  }
}

/**********************************************************************
Function: PrintStratify()
Category: Modifiers
Scope: public
In: -
Out: -
Description:
**********************************************************************/
string POPULATION::PrintStratify() {
  cout << StratifyOutput;
  return StratifyOutput;
}
