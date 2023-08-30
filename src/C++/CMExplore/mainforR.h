//----------------------------------------------------------------------------//
// $Header: //depot/Explore/UIExplore/project.h#21 $
// $Author: Rijnbeek $
// $DateTime: 2008/09/09 14:16:02 $
// $Change: 87 $
//----------------------------------------------------------------------------//

#include "initialise.h"

#include "../Explore/explore.h"
#include "../IOExplore/IOExplore.h"


IOExplore      *MyIOExplore;                                              		// Input object
Explore        *MyExplore;                                                 		// Explore core object

// int main(int argc, char* argv[]);

int mainforR(int argc, char* argv[]);

bool get_parameters(int argc, char *argv[], ExploreSettings *c);

void ExploreProgress(double CurrentProgress);
void ExploreSummary(string CurrentRule,
								   string CurrentPerformance,
								   string BestRule,
								   string BestPerformance,
								   long CombinationsDone,
								   long FeatureSetsDone,
								   long ConditionSetsDone,
								   unsigned int PartitionsDone,
								   long CombinationsTotal,
								   long FeatureSetsTotal,
								   double RulesTotalLimit,
                                   unsigned int PartitionsTotal);
