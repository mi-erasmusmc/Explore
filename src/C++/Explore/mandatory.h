//----------------------------------------------------------------------------//
// $Header: //depot/Explore/Explore/mandatory.h#6 $
// $Author: Pepe $
// $DateTime: 2004/10/11 08:11:59 $
// $Change: 100 $
//----------------------------------------------------------------------------//

#ifndef MANDATORY_H
#define MANDATORY_H

#include "initialise.h"

class MANDATORY {
  private:
    list<unsigned int> Features;
    list<bool> Included;

  public:
    // Constructors and destructors
    MANDATORY();                                                              
    ~MANDATORY();                                                             

    // Modifiers
    void AddFeature(unsigned int Number);
    void RemoveFeature(unsigned int Number);
    void RemoveAll();

    // Selectors
    bool IsIncluded(unsigned int Number);
    unsigned int TimesIncluded(unsigned int Number);
    bool IsMandatory(unsigned int Number);
    unsigned int FeaturesLeft();
};

#endif
