//----------------------------------------------------------------------------//
// $Header: //depot/Explore/Explore/observation.h#9 $
// $Author: Rijnbeek $
// $DateTime: 2008/09/09 14:16:02 $
// $Change: 87 $
//----------------------------------------------------------------------------//

#ifndef OBSERVATION_H
#define OBSERVATION_H

#include "initialise.h"

class OBSERVATION {
  private:
	string Value;
	unsigned int Class;
	unsigned int Order;
	unsigned int Partition;


  public:
	OBSERVATION();
	OBSERVATION(const OBSERVATION& CObservation);
	OBSERVATION(string OValue, unsigned int OClass, unsigned int OOrder);
	~OBSERVATION();

	// Selectors
	unsigned int GetOrder() const;
	unsigned int GetClass() const;
	string GetValue() const;
	void SetClass(unsigned int OClass);
};

#endif

