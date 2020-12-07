//----------------------------------------------------------------------------//
// $Header: //depot/Explore/Explore/bin.h#8 $
// $Author: Rijnbeek $
// $DateTime: 2008/09/09 14:16:02 $
// $Change: 87 $
//----------------------------------------------------------------------------//

#ifndef BIN_H
#define BIN_H
#include "initialise.h"

class BIN {
  private:
	string Value;                                                               // The value of this bin (potentially a cutoff value)
    unsigned int NegativeCount;                                                 // Amount of negative observations with the same value in a feature
    unsigned int PositiveCount;                                                 // Amount of positive observations with the same value in a feature

  public:
    // Constructors and destructors
    BIN();
    BIN(string BValue);
    ~BIN();

    // Selectors
	string GetValue();                                                          // Get value of this bin (observation)

    unsigned int GetNegativeCount();                                            // Get negative count
    unsigned int GetPositiveCount();                                            // Get positive count

    // Modifiers
    void IncreaseNegativeCount();                                               // Increase negative count
    void IncreasePositiveCount();                                               // Increase positive count
};

#endif 
