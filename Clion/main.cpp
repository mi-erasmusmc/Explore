//---------------------------------------------------------------------------


#include "../src/C++/CMExplore/initialise.h"
#include "../src/C++/CMExplore/main.h"
#include "../src/C++/Explore/explore.h"

#pragma hdrstop

#ifdef DEBUG_TIMING
TIMING ExploreTiming;
#endif

//---------------------------------------------------------------------------

#pragma argsused
int main(int argc, char* argv[])
{
 char*		parm;
 char*      filename;
 ExploreSettings ProjectSettings;

 //Parse arguments
 if (get_parameters(argc, argv, &ProjectSettings)) {
     MyExplore = new Explore();
     MyIOExplore = new IOExplore();
     MyIOExplore->SetProject(MyExplore);                                           // Connect IOExplore with Explore
     if (MyIOExplore->SetupExploreFromProject(ProjectSettings.ProjectFile)) {
         printf("Running EXPLORE using %s\n", ProjectSettings.ProjectFile.c_str());
         for (unsigned int i = 1; i <= ProjectSettings.Runs; i++) {
             if (ProjectSettings.Runs > 1) {
                 printf("Run %d of %d:\n", i, ProjectSettings.Runs);
                 MyIOExplore->IOExplore::SetNextIncrementalResultFile(); // Only when multiple runs this is required
             }
             printf("Result are written to %s\n\n", MyIOExplore->GetResultFilename().c_str());
             MyExplore->Explore::SetOutput(MyIOExplore->GetResultBuffer());
             MyExplore->Explore::ClearPartitions();
             MyExplore->Explore::Partition();
             MyExplore->Explore::FindCutoffs();
             MyExplore->Explore::FindOperators();
             MyExplore->Explore::Initialise();                                                   // Initialise Explore rule-generation
             MyExplore->Explore::Start();
             MyExplore->Explore::ResetSeed();
         }
     } else {
         printf("Project loaded unsuccesfully: \n %s \n", MyIOExplore->PrintProjectLoadErrors().c_str());
     }
 }
  return 0;
}


bool get_parameters(int argc, char *argv[], ExploreSettings *c)
{  bool   ok = true;
   char*  parm;

   //defaults
   c->Runs = 1;

   for (int i=1; ( (i<argc) && (ok) ); i++) {
	  parm = argv[i];
	  if (parm[0] != '-') {
		 c->ProjectFile = parm;
	  } else {
		 if (strlen(parm) > 1) {
			switch (parm[1]) {
			   case 'r' : i++;
						  if (i<argc && (atoi(argv[i])>0)) {
							c->Runs = atoi(argv[i]);
						  }  else {
						    ok = false;
						  }
						  break;

			   default  : printf("Unrecognized option '%s' ignored.\n", parm);
						  ok = false;
						  break;
			}
		 } else {
			printf("Unrecognized option '%s' ignored.\n", parm);
			ok = false;
		 }
	  }
   }


   if (!ok | (argc==1)) {

	  printf("Usage: CMExplore <projectfile> \n");
	  printf("   <projectfile> : Name of the inputfile.\n"	);
	  printf("   -r <Runs> : Number of runs <default = 1>.\n"	);
	  return(false);
   }
   return(true);
}



/**********************************************************************

Function: ExploreProgress()
Category: Function Handlers
Scope: public
In: -
Out: -
Description: Progress handler for explore.
**********************************************************************/
void ExploreProgress(double CurrentProgress) {

}

/**********************************************************************
Function: ExploreSummary()
Category: Function Handlers
Scope: public
In: -
Out: -
Description: Periodic summary of explore.
**********************************************************************/
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
								   unsigned int PartitionsTotal) {

  printf("Partition '%d' of '%d' ; Combination '%d' of '%d'", PartitionsDone, PartitionsTotal,
		 CombinationsDone, CombinationsTotal);
    printf("Current best rule: '%s' '%s'\n", BestRule.c_str(), CurrentPerformance.c_str());

}



//---------------------------------------------------------------------------
