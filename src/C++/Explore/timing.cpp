//----------------------------------------------------------------------------//
// $Header: //depot/Explore/Explore/timing.cpp#7 $
// $Author: Rijnbeek $
// $DateTime: 2008/09/09 14:16:02 $
// $Change: 87 $
//----------------------------------------------------------------------------//

#include "timing.h"

vector<string>         TIMING::Descriptions;
vector<unsigned long>  TIMING::Counters;
vector<clock_t>        TIMING::TotalTimes;
bool                   TIMING::ClkTck;
double				   TIMING::Total;

/**********************************************************************
Function: TIMING()
Category: Constructors and Destructors
Scope: public
In: -
Out: -
Description: Default constructor for a timing object.
**********************************************************************/
TIMING::TIMING() {                                                              // Constructor

//  ClkTck = false;
//  if (CLK_TCK != -1) {
	ClkTck = false;
	Total = 0;
//  }
  
}

/**********************************************************************
Function: ~TIMING()
Category: Constructors and Destructors
Scope: public
In: -
Out: -
Description: Default destructor for a timing object.
**********************************************************************/
TIMING::~TIMING() {
}

/**********************************************************************
Function: AddTime()
Category: Modifiers
Scope: public
In: -
Out: -
Description: Add a timing session.
**********************************************************************/
void TIMING::AddTime(string Description, clock_t StartTime, clock_t EndTime) {
  vector<string>::iterator CurrentDescription(TIMING::Descriptions.begin());
  vector<string>::iterator LastDescription(TIMING::Descriptions.end());

  bool Found = false;
  clock_t DifferenceTime;

  DifferenceTime = (EndTime - StartTime);
  Total = 0;

  for (unsigned int i=0; CurrentDescription != LastDescription; CurrentDescription++) {
    if ((*CurrentDescription).compare(Description)==0) { // compare equal
      TotalTimes[i] += DifferenceTime;
	  Counters[i]++;
	  Found = true;
	}
	Total += TotalTimes[i];
	i++;
  }

  if (!Found) {
    Descriptions.push_back(Description);
    Counters.push_back(1);
	TotalTimes.push_back(DifferenceTime);
	Total += DifferenceTime;
  }
}

/**********************************************************************
Function: PrintAll()
Category: Modifiers
Scope: public
In: -
Out: -
Description: Returns all timers as a string.
**********************************************************************/
string TIMING::PrintAll() {
  vector<string>::iterator CurrentDescription(TIMING::Descriptions.begin());
  vector<string>::iterator LastDescription(TIMING::Descriptions.end());
  ostringstream Result;
  unsigned int DescriptionPads;

  Result << endl << "TIMING (" << Total << ") ";
  if (ClkTck) {
	Result << "Ticks" << endl << endl;
  } else {
	Result << "Seconds" << endl;
  }

  Result << "Calculation time of each rulelength" << endl;
  for (unsigned int i=0; CurrentDescription != LastDescription; i++) {
	DescriptionPads = 70-(*CurrentDescription).length();
	Result << (double)(TotalTimes[i]/CLOCKS_PER_SEC) << "\t";
	CurrentDescription++;
  }
  Result << endl;
  Result << "Total:" << Total /CLOCKS_PER_SEC << " sec." << endl << endl;
  return Result.str();
}


/**********************************************************************
Function: PrintAll()
Category: Modifiers
Scope: public
In: -
Out: -
Description: Returns all timers as a string.
**********************************************************************/
string TIMING::PrintTotal() {
    ostringstream Result;
    Result << endl;
    Result << "Total:" << Total /CLOCKS_PER_SEC << " sec." << endl;
    return Result.str();
}


/**********************************************************************
Function: PrintAll()
Category: Modifiers
Scope: public
In: -
Out: -
Description: Returns all timers as a string.
**********************************************************************/
string TIMING::PrintLast() {
  vector<string>::iterator CurrentDescription(TIMING::Descriptions.begin());
  vector<string>::iterator LastDescription(TIMING::Descriptions.end());
  ostringstream Result;
  unsigned int DescriptionPads;

  Result << endl << "TIMING (" << Total << ") ";
  if (ClkTck) {
	Result << "Ticks" << endl << endl;
  } else {
	Result << "Seconds" << endl;
  }

  Result << "Description,Counter,Total click,\tTotal time" << endl;
  for (unsigned int i=0; CurrentDescription != LastDescription; i++) {
	if (CurrentDescription == LastDescription-1){
	  DescriptionPads = 70-(*CurrentDescription).length();
	  Result <<  (*CurrentDescription) << ",";
	  Result << Counters[i] << "," << (double)(TotalTimes[i]);
	  Result << ",\t"  << (double)(TotalTimes[i]/CLOCKS_PER_SEC) << endl;
	}
	CurrentDescription++;
  }
  Result << endl;
  return Result.str();
}

/**********************************************************************
Function: Clear()
Category: Modifiers
Scope: public
In: -
Out: -
Description: Clears all timers
**********************************************************************/
void TIMING::Clear() {
  Descriptions.clear();
  Counters.clear();
  TotalTimes.clear();
}
