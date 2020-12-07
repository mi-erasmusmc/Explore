//----------------------------------------------------------------------------//
// $Header: //depot/Explore/Explore/classdef.h#10 $
// $Author: Rijnbeek $
// $DateTime: 2008/09/09 14:16:02 $
// $Change: 87 $
//----------------------------------------------------------------------------//

#ifndef CLASSES_H
#define CLASSES_H
                                            
#include "initialise.h"
#include "observation.h"

class CLASSES {
  private:
    unsigned int Class;
    vector<OBSERVATION*> Observations;

    // Structure to sort observations based on their value (pointer)
    struct SortByValue {
      bool operator()(const OBSERVATION* a, const OBSERVATION* b) {
		return a->GetValue() < b->GetValue();
      }
    } ByValue;

    struct SortByOrder {
      bool operator()(const OBSERVATION* a, const OBSERVATION* b) {
        return a->GetOrder() < b->GetOrder();
      }
    } ByOrder;

    
  public:
    CLASSES();
    CLASSES(unsigned int ClassNumber);
    ~CLASSES();

    // Modifiers
    void AddObservation(OBSERVATION* NewObservation);
    
    void Clear();
    void SortOrder();
    void SortValue();

    // Selectors
    unsigned int GetSize();
    unsigned int GetNumber();

    OBSERVATION* GetObservation(unsigned int POrder);
    unsigned int GetNoObservations();

    float GetSensitivity(float OValue);
    float GetSpecificity(float OValue);

    void PrintObservations();

};

#endif
