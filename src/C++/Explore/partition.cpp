//----------------------------------------------------------------------------//
// $Header: //depot/Explore/Explore/partition.cpp#11 $
// $Author: Rijnbeek $
// $DateTime: 2008/09/09 14:16:02 $
// $Change: 87 $
//----------------------------------------------------------------------------//

#include "partition.h"

/**********************************************************************
Function: PARTITION() 
Category: Constructors and Destructors
Scope: public
In: -
Out: -
Description: Default constructor for a partition.
**********************************************************************/
PARTITION::PARTITION() {                                                        // Constructor
}

/**********************************************************************
Function: PARTITION() 
Category: Constructors and Destructors
Scope: public
In: enum PARTITION_TYPE, type of this partition.
Out: -
Description: Constructor for a partition.
**********************************************************************/
PARTITION::PARTITION(PARTITION_TYPE PType) {                                    // Constructor
  Type = PType;                                                                 // Set partition type
}

/**********************************************************************
Function: ~PARTITION()
Category: Selectors
Scope: public
In: -
Out: -
Description: Destructor for a partition.
**********************************************************************/
PARTITION::~PARTITION() {                                                       // Destructor
}

/**********************************************************************
Function: AddObservation()
Category: Modifiers
Scope: public
In: -
Out: -
Description: Associate an observation with this partition.
**********************************************************************/
void PARTITION::AddObservation(unsigned int NewObservation) {                   // Add a pointer to an observation to this partition
  Observations.push_back(NewObservation);
}

/**********************************************************************
Function: Clear()
Category: Modifiers
Scope: public
In: -
Out: -
Description: Clears all observations associated with this partition.
**********************************************************************/
void PARTITION::Clear() {                                                       // Clear this partition
  Observations.clear();
}

/**********************************************************************
Function: GetObservation()
Category: Selectors
Scope: public
In: unsigned int, order of the observation.
Out: -
Description: Returns a reference to an original observation based
on it's order.
**********************************************************************/
unsigned int PARTITION::GetObservation(unsigned int POrder) {                   // Get next observation in this partition
  return Observations.at(POrder);
}

/**********************************************************************
Function: GetType()
Category: Selectors
Scope: public
In: -
Out: -
Description: Returns the type of this partition.
**********************************************************************/
PARTITION_TYPE PARTITION::GetType() {                                           // Is this partition needed for learning, testing or both?
  return Type;
}

/**********************************************************************
Function: SetType()
Category: Selectors
Scope: public
In: -
Out: -
Description: Sets the type of this partition.
**********************************************************************/
void PARTITION::SetType(PARTITION_TYPE PType) {
  Type = PType;
}

/**********************************************************************
Function: GetSize()
Category: Selectors
Scope: public
In: -
Out: -
Description: Returns the number of observations associated with this
partition.
**********************************************************************/
unsigned int PARTITION::GetSize() {                                             // Get the size of observations in this partition
  return Observations.size();
}

/**********************************************************************
Function: PrintObservation()
Category: Selectors
Scope: public
In: -
Out: -
Description: Prints all the observations associated with this partition.
**********************************************************************/
string PARTITION::PrintObservations() {
  vector<unsigned int>::iterator CurrentObservation(Observations.begin());
  vector<unsigned int>::iterator LastObservation(Observations.end());
  ostringstream Result;

  Result << "PARTITION OBSERVATIONS (Orders)" << endl;
  Result <<  "Type of partition: ";

  sort(Observations.begin(), Observations.end());
  switch (Type) {
    case LEARN:
      Result << "LEARN";
      break;
	case VALIDATION:
	  Result << "VALIDATION";
	  break;
	case TRAIN:
      Result << "TRAIN: LEARN AND VALIDATE";
      break;
  }
  Result << endl;

  for (; CurrentObservation != LastObservation; CurrentObservation++) {
    Result << (*CurrentObservation) << "\t";
  }
  Result << endl;

  return Result.str();
}

