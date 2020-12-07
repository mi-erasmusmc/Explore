//----------------------------------------------------------------------------//
// $Header: //depot/Explore/Explore/partition.h#9 $
// $Author: Rijnbeek $
// $DateTime: 2008/09/09 14:16:02 $
// $Change: 87 $
//----------------------------------------------------------------------------//

#ifndef PARTITION_H
#define PARTITION_H

#include "initialise.h"

class PARTITION {
  private:
    vector<unsigned int> Observations;                                          // Vector of observations enabling direct access
    PARTITION_TYPE Type;                                                        // Function of this partition (Learning, testing or both)    

  public:
    // Constructors and destructors
    PARTITION();
    PARTITION(PARTITION_TYPE PType);                                            // Create a partition with type LEARN, TEST, Validation
    ~PARTITION();                                                               // Destructor

    // Modifiers
    void Clear();                                                               // Clear this partition
    void AddObservation(unsigned int NewObservation);                           // Add an observation to this partition

    // Selectors
    unsigned int GetObservation(unsigned int POrder);                           // Get an observation which is part of this partition
    PARTITION_TYPE GetType();                                                   // Is this partition needed for learning, testing or both?
    void SetType(PARTITION_TYPE PType);                                         // Set the type for this partition

    unsigned int GetSize();                                                     // Get the size of observations in this partition
    string PrintObservations();                                                 // Print observations in this partition
};

#endif
