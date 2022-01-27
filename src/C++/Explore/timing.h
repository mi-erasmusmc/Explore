//----------------------------------------------------------------------------//
// $Header: //depot/Explore/Explore/timing.h#8 $
// $Author: Rijnbeek $
// $DateTime: 2008/09/09 14:16:02 $
// $Change: 87 $
//----------------------------------------------------------------------------//

#ifndef TIMING_H
#define TIMING_H

#include "../stl.h"
#include <sstream>

class TIMING {
  private:
    static vector<string>         Descriptions;
    static vector<unsigned long>  Counters;
    static vector<clock_t>        TotalTimes;

	static bool ClkTck;
	static double Total;

  public:
    TIMING();
    ~TIMING();

    void AddTime(string TDescription, clock_t StartTime, clock_t EndTime);
    string PrintAll();
    string PrintTotal();
	string PrintLast();
	void Clear();
};

#endif
