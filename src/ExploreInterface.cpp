
// [[Rcpp::depends(BH)]]
#include <Rcpp.h>
#include "C++/CMExplore/main.h"
#include "C++/CMExplore/initialise.h"
#include "../src/C++/Explore/explore.h"

#define DEBUG_TIMING

using namespace Rcpp;

//' @useDynLib Explore
//' @import Rcpp

//' @export
// [[Rcpp::export]]
void runExplore(Rcpp::CharacterVector input) {
  // Transfer data from CharacterVector to std::string
  std::string str = Rcpp::as<std::string>(input); 
  
  std::vector<char*> cstrings;
  
  cstrings.push_back(const_cast<char*> (str.c_str()));
  
  int argc = (int)cstrings.size()+1;
  char** argv = cstrings.data();
  
  int res = main(argc, argv);
}

/*** R
# library(Rcpp)
# Sys.setenv("PKG_CXXSTD"="-std=c++11")
# Sys.setenv("PKG_CXXFLAGS"="-I /Users/aniekmarkus/Documents/Documents/Code/C++/boost_1_74_0/")
# sourceCpp(paste0(getwd(), "/src/ExploreInterface.cpp"))

# input <- paste0(getwd(), "/src/C++/Test/test.project")
# runExplore(input)
*/

int main(int argc, char* argv[])
{
  ExploreSettings ProjectSettings;
  
  // Parse arguments
  if (get_parameters(argc, argv, &ProjectSettings)){
    MyExplore     = new Explore();
    MyIOExplore 	= new IOExplore();
    MyIOExplore->SetProject(MyExplore);                                           // Connect IOExplore with Explore
    // printf("Runs %d \n",ProjectSettings.Runs);
    
    if (MyIOExplore->SetupExploreFromProject(ProjectSettings.ProjectFile)){
      // printf("Running EXPLORE using %s\n",ProjectSettings.ProjectFile.c_str());
      for (unsigned int i=1; i <= ProjectSettings.Runs; i++) {
        if (ProjectSettings.Runs>1) {
          printf("Run %d of %d:\n",i,ProjectSettings.Runs);
        }
        // printf("Result are written to %s\n\n",MyIOExplore->GetResultFilename().c_str());
        MyExplore->SetOutput(MyIOExplore->GetResultBuffer());
        MyExplore->ClearPartitions();
        MyExplore->Partition();
        MyExplore->FindCutoffs();
        MyExplore->FindOperators();
        MyExplore->Initialise();                                                   // Initialise Explore rule-generation
        MyExplore->Start();
        // MyIOExplore->SetNextIncrementalResultFile();
        MyExplore->ResetSeed();
      }
    } else {
      printf("Project loaded unsuccesfully: \n %s \n",MyIOExplore->PrintProjectLoadErrors().c_str());
    }
  }
  
  return 0;
}

bool get_parameters(int argc, char *argv[], ExploreSettings *c)
{  bool   ok = true;
  char*  parm;
  
  //defaults
  c->Runs = 1;
  
  for (int i=0; ( (i<argc-1) && (ok) ); i++) {
    parm = argv[i];
    if (parm[0] != '-') {
      c->ProjectFile = parm;
    } else {
      if (strlen(parm) > 1) {
        switch (parm[1]) {
        case 'r' : i++;
          if (i<argc-1 && (atoi(argv[i])>0)) {
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
  
  
  if (!ok) {
    
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
  
  printf("Partition '%d' of '%d' ; Combination '%ld' of '%ld'", PartitionsDone, PartitionsTotal,
         CombinationsDone, CombinationsTotal);
  printf("Current best rule: '%s' '%s'\n", BestRule.c_str(), CurrentPerformance.c_str());
  
}


//---------------------------------------------------------------------------
