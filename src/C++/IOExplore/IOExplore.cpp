//----------------------------------------------------------------------------//
// $Header: //depot/Explore/IOExplore/IOExplore.cpp#24 $
// $Author: Rijnbeek $
// $DateTime: 2008/09/09 14:16:02 $
// $Change: 87 $
//----------------------------------------------------------------------------//

#include "IOExplore.h"
#include "../common.h"
//#include <string.h>

#define COMMANDVERSION

/**********************************************************************
Function: ~IOExplore()
Category: Constructors and Destructors
Scope: public
In: -
Out: -
Description: Default constructor for IOExplore.
**********************************************************************/
IOExplore::IOExplore() {
  vector<string> Dummy;
  Dummy.push_back("[General]");
  Dummy.push_back("[Setup]");
  Dummy.push_back("[Restrictions]");
  Dummy.push_back("[Constraints]");
  Dummy.push_back("[Output]");
  Dummy.push_back("[Run]");
  Sections.push_back(Dummy);
  Dummy.clear();

  Dummy.push_back("Name");
  Dummy.push_back("Remarks");
  Dummy.push_back("DataFile");
  Dummy.push_back("Output");
  Dummy.push_back("OutputFile");
  Sections.push_back(Dummy);
  Dummy.clear();

  Dummy.push_back("PartitionMethod");
  Dummy.push_back("Randomize");
  Dummy.push_back("StartRulelength");
  Dummy.push_back("EndRulelength");
  Dummy.push_back("LearnRatio");
  Dummy.push_back("NumberOfPartitions");
  Dummy.push_back("PositiveClass");
  Dummy.push_back("OperatorMethod");
  Dummy.push_back("CutoffMethod");
  Dummy.push_back("CutoffFile");
  Sections.push_back(Dummy);
  Dummy.clear();

  Dummy.push_back("Restriction");
  Sections.push_back(Dummy);
  Dummy.clear();

  Dummy.push_back("Maximize");
  Dummy.push_back("Sensitivity");
  Dummy.push_back("Specificity");
  Dummy.push_back("Accuracy");
  Dummy.push_back("PPV");
  Dummy.push_back("NPV");
  Sections.push_back(Dummy);
  Dummy.clear();

  Dummy.push_back("Outputmethod");
  Dummy.push_back("Settings");
  Dummy.push_back("Partitions");
  Dummy.push_back("FeatureOperators");
  Dummy.push_back("CutoffMethod");
  Dummy.push_back("CutoffValues");
  Dummy.push_back("OperatorMethod");
  Dummy.push_back("OperatorValues");
  Dummy.push_back("Combinations");
  Dummy.push_back("FeatureSets");
  Dummy.push_back("CutoffSets");
  Dummy.push_back("Performance");
  Dummy.push_back("Sets");
  Sections.push_back(Dummy);
  Dummy.clear();

  Dummy.push_back("OptimizeRules");
  Sections.push_back(Dummy);
  Dummy.clear();
}

/**********************************************************************
Function: ~IOExplore()
Category: Constructors and Destructors
Scope: public
In: -
Out: -
Description: Default destructor for IOExplore.
**********************************************************************/
IOExplore::~IOExplore() {
}

/**********************************************************************
Function: ClearSettings()
Category: Modifiers
Scope: public
In: -
Out: -
Description: Clear the struct with current project-settings.
**********************************************************************/
void IOExplore::ClearSettings() {
  ProjectSettings.Name                   = "";
  ProjectSettings.Remarks                = "";
  ProjectSettings.DataFile               = "";
  ProjectSettings.OutputType             = "";
  ProjectSettings.OutputFile             = "";
  ProjectSettings.IncrementalOutputFile  = false;
  ProjectSettings.PartitionMethod        = HOLDOUT;
  ProjectSettings.Randomize              = false;
  ProjectSettings.StartRulelength        = 0;
  ProjectSettings.EndRulelength          = 0;
  ProjectSettings.Runs			         = 1;
  ProjectSettings.LearnRatio             = 0;
  ProjectSettings.NumberofPartitions     = 0;
  ProjectSettings.PositiveClass          = "";
  ProjectSettings.OperatorMethod         = EXHAUSTIVE;
  ProjectSettings.CutoffMethod           = RVAC;
  ProjectSettings.CutoffFile             = "";
  ProjectSettings.Rule                   = "";
  ProjectSettings.FeatureRestrictions.clear();
  ProjectSettings.Maximize               = SENSITIVITY;
  ProjectSettings.Sensitivity            = 0;
  ProjectSettings.Specificity            = 0;
  ProjectSettings.NPV                    = 0;
  ProjectSettings.PPV                    = 0;
  ProjectSettings.Accuracy               = 0;
  ProjectSettings.OutputMethod           = INCREMENT;
  ProjectSettings.PrintSettings          = false;
  ProjectSettings.PrintPartitions        = false;
  ProjectSettings.PrintFeatureOperators  = false;
  ProjectSettings.PrintCutoffMethod      = false;
  ProjectSettings.PrintCutoffValues      = false;
  ProjectSettings.PrintOperatorMethod    = false;
  ProjectSettings.PrintOperatorValues    = false;
  ProjectSettings.PrintCombinations      = false;
  ProjectSettings.PrintFeatureSets       = false;
  ProjectSettings.PrintCutoffSets        = false;
  ProjectSettings.PrintCutoffSetsBestLength        = false;
  ProjectSettings.PrintPerformance       = false;
  ProjectSettings.PrintSets              = false;
  ProjectSettings.BranchBound            = false;
  ProjectSettings.SubSumption            = false;
}

/**********************************************************************
Function: SetProject()
Category: Modifiers
Scope: public
In: Explore*, pointer to an explore project.
Out: -
Description: Associates an Explore project with IOExplore.
**********************************************************************/
void IOExplore::SetProject(Explore* NewProject) {
    Project = NewProject;
#ifndef COMMANDVERSION
    Project->SavePartitionFileFunction = &Explore::SavePartitionFile;
#endif
}

/**********************************************************************
Function: ReadDataFile()
Category: Modifiers
Scope: public
In: -
Out: -
Description: Reads a data file and sets-up Explore. Extracts
ClassNames and FeatureNames.
**********************************************************************/
bool IOExplore::ReadDataFile() {
/*  if (DataFile.is_open()) {
	unsigned int NoObservations;                                                // Variable to store class of current observations
	unsigned int Class;
	list<float> Observations;
	char Separators[] = ",;|#!:+";
	char* MyToken;
	char Buffer[MAX_LINESIZE];
	char* Dummy;
	char *ptr;
	bool MissingValue;

	DataFile.getline(Buffer,MAX_LINESIZE,ENDL_SEPARATOR);                       // Read Classnames
	Dummy = Buffer;
	MyToken = str_parse(&Dummy,Separators);
	while (MyToken) {                                                           // Extract featurenames from data file
	  Project->AddFeature(MyToken,0);                                             // Add featurename to explore
	  FeatureNames.push_back(MyToken);                                          // Add featurename to list
	  MyToken = str_parse(&Dummy,Separators);
	}

	if (FeatureNames.size()<=1) {                                               // Make sure we have at least 2 feature
	  return false;
	}

	NoObservations=0;                                                           // Read observations
	for (unsigned int j=0; DataFile.getline(Buffer,MAX_LINESIZE,ENDL_SEPARATOR); j++) {
	  if (!DataFile.fail()) {
		Dummy = Buffer;

		// Check Missing Values
		MissingValue=false;
		ptr = strchr(Buffer,'?');
		if (ptr) {
		  MissingValue=true;
		}

		if (!MissingValue){                                                     // line with missingvalies are skipped
		  Dummy = Buffer;
		  MyToken = str_parse(&Dummy,Separators);
		  for (unsigned int i=0; MyToken && i <= FeatureNames.size(); i++) {
			if (i==FeatureNames.size()) {
			  list<string>::iterator CurrentClass(ClassNames.begin());
			  list<string>::iterator LastClass(ClassNames.end());
			  bool Found = false;
			  for (unsigned int k=0; CurrentClass != LastClass && !Found; k++) {
				int Result = strcmp((*CurrentClass).c_str(),MyToken);
				if (Result==0) {
				  Found = true;
				  Class = k;
				}
				CurrentClass++;
			  }
			  if (!Found) {
				ClassNames.push_back(MyToken);                                  // Add Classname to list if needed
				Class = ClassNames.size()-1;
			  }
	//          Project->AddObservation(Observations,Class);                      // Add class and observation to the feature
			  Observations.clear();
			} else {
			  Observations.push_back(atof(MyToken));
			}
			MyToken = str_parse(&Dummy,Separators);
		  }
		  NoObservations++;                                                     // Count observations
		}
	  } else {
		return false;
	  }
	}
  }
  Project->SetNoClasses(ClassNames.size());
  return true;
 */

  return false;
}

/**********************************************************************
Function: ReadArffDataFile()
Category: Modifiers
Scope: public
In: -
Out: -
Description: Reads an ARFF data file and sets-up Explore. Extracts
ClassNames and FeatureNames etc.
**********************************************************************/
bool IOExplore::ReadArffDataFile() {
  if (DataFile.is_open()) {
	unsigned int NoObservations;                                                // Variable to store class of current observations
	unsigned int Class;
	list<string> Observations;
	char Separator[]= " ";
	char* MyToken;
	char* FeatureName;
	char* FeatureType;
	char Buffer[MAX_LINESIZE];
	char* Dummy;
	char *ptr;
	bool MissingValue;
	bool StartDataBlock;
	string Mydum;

	StartDataBlock = false;

	for (unsigned int j=0; !StartDataBlock && DataFile.getline(Buffer,MAX_LINESIZE,ENDL_SEPARATOR); j++) {
	  Dummy = Buffer;
	  MyToken = str_parse(&Dummy,Separator);
	  if (!strchr(Buffer,'%') && MyToken){									   	// skip any empty or comment lines
		for (int i=0; i<strlen(MyToken); i++)								    //Uppercase Token
		{
			MyToken[i] = toupper(MyToken[i]);
		}
		if (strcmp(MyToken,"@RELATION")==0) {
		  //add population name
		}

		if (strcmp(MyToken,"@ATTRIBUTE")==0) {
		  FeatureName = str_parse(&Dummy," \t");
		  FeatureType = str_parse(&Dummy," \t");
		  for (int i=0; (i<strlen(FeatureType)) && FeatureType; i++)											//Uppercase Token
		  {
			FeatureType[i] = toupper(FeatureType[i]);
		  }
		  if ((strcmp(FeatureType,"NUMERIC")==0) ||
			 (strcmp(FeatureType,"REAL")==0))
	     	  Project->AddFeature(FeatureName,true);
		  else
			Project->AddFeature(FeatureName,false);
		  FeatureNames.push_back(FeatureName);                                  // Add featurename to list
		}
		if (strcmp(MyToken,"@DATA")==0) {
		  StartDataBlock = true;
		}
	  }
	}

	if (FeatureNames.size()<=1) {                                               // Make sure we have at least 2 feature
	  return false;
	}

	NoObservations=0;                                                           // Read observations
	if (StartDataBlock) {
		for (unsigned int j=0; DataFile.getline(Buffer,MAX_LINESIZE,ENDL_SEPARATOR); j++) {
		  if (!DataFile.fail()) {
			Dummy = Buffer;

			// Check Missing Values
			MissingValue=false;
			ptr = strchr(Buffer,'?');
			if (ptr) {
			  MissingValue=true;
			}

			MyToken = str_parse(&Dummy,",");
			if ((!MissingValue) && MyToken && (!strchr(Buffer,'%'))){                   					// line with missingvalies are skipped
			  for (unsigned int i=1; MyToken && i <= FeatureNames.size(); i++) {
				Observations.push_back(MyToken);
				if (i==FeatureNames.size()) {
				  Project->AddObservation(Observations,0);                      // Add class and observation to the feature
				  Observations.clear();
				}
     			MyToken = str_parse(&Dummy,",");
			  }
			  NoObservations++;                                                 // Count observations
			}
		  } else {
			return false;
		  }
	   }
	}
	if (NoObservations == 0) return false;
	else return true;
  }
  return false;
}


/**********************************************************************
Function: ReadCutoffFile()
Category: Modifiers
Scope: public
In: -
Out: -
Description: Reads a cutoff file and adds these to Explore.
**********************************************************************/
bool IOExplore::ReadCutoffFile() {
/*  if (CutoffFile.is_open()) {
	char Separators[] = ",;|#!:+";
	char* MyToken;
	char Buffer[MAX_LINESIZE];
	char* Dummy;

	CutoffFile.getline(Buffer,MAX_LINESIZE,ENDL_SEPARATOR);
	CutoffFile.getline(Buffer,MAX_LINESIZE,ENDL_SEPARATOR);
	while (!CutoffFile.fail()) {
	  Dummy = Buffer;
	  MyToken = str_parse(&Dummy,Separators);
	  for (unsigned int i=0; MyToken; i++) {
		if (strlen(MyToken)>0) {
		  Project->AddCutoff(i,atof(MyToken));
		}
		MyToken = str_parse(&Dummy,Separators);
	  }
	  CutoffFile.getline(Buffer,MAX_LINESIZE,ENDL_SEPARATOR);
	}
  }*/
  return true;
}

/**********************************************************************
Function: SavePartitionFile()
Category: Modifiers
Scope: public
In: -
Out: -
Description: Saves a PartitionFile as datafile-nr.learn or test
**********************************************************************/
void IOExplore::SavePartitionFile(string Output, PARTITION_TYPE Type, unsigned int PartNr) {

   ostringstream Prefix;

   // change file extension
   if (Type==LEARN) {
	  Prefix << "-" << PartNr << ".learn";
   }
   else
	 if (Type==VALIDATION) {
	  Prefix << "-" << PartNr << ".test";
	 }
   char* dummy = (char*)ResultFilename.c_str();
   string newfilename = str_parse(&dummy,".");
   newfilename += Prefix.str();

   // save to file
   ClosePartitionFile();
   if (SetPartitionFile(newfilename.c_str())) {
	 PartitionFile << Output;
	 PartitionFile.flush();
	 PartitionFile.close();
   }
}


/**********************************************************************
Function: SaveExploreToProject()
Category: Selectors
Scope: public
In: string, filename of project-file
Out: -
Description: Saves all settings associated with Explore reference to
a project file.
**********************************************************************/
bool IOExplore::SaveExploreToProject(string IOFilename) {
    ClearSettings();

    FeatureRestriction CurrentRestriction;

    // General Settings
    ProjectSettings.Name       = Project->GetName();
    ProjectSettings.Remarks    = Project->GetDescription();
    ProjectSettings.DataFile   = DataFilename;
    ProjectSettings.OutputType = ProjectLoadOutputType;
    ProjectSettings.OutputFile = ResultFilename;    

    if (IncrementalOutputFile) {
      ProjectSettings.IncrementalOutputFile = true;
    } else {
      ProjectSettings.IncrementalOutputFile = false;
    }

    // Setup Settings
	switch (Project->GetPartitionMethod()){
	  case RESUBSTITUTION:
		ProjectSettings.PartitionMethod = RESUBSTITUTION;
        break;
      case HOLDOUT:
        ProjectSettings.PartitionMethod = HOLDOUT;
        break;
      case CROSS_VALIDATION:
        ProjectSettings.PartitionMethod = CROSS_VALIDATION;
        break;
    }

    if (Project->GetRandomize()) {
      ProjectSettings.Randomize = true;
    } else {
      ProjectSettings.Randomize = false;
    }
    
    ProjectSettings.StartRulelength     = Project->GetMinRuleLength();
    ProjectSettings.EndRulelength       = Project->GetMaxRuleLength();
    ProjectSettings.LearnRatio          = Project->GetLearnRatio();
    ProjectSettings.NumberofPartitions  = Project->GetNoPartitions();

    list<string>::iterator CurrentClassName(ClassNames.begin());
    list<string>::iterator LastClassName(ClassNames.end());
    for (unsigned int i=0; CurrentClassName != LastClassName; CurrentClassName++) {
      if (i==Project->GetPositiveClass()) {
        ProjectSettings.PositiveClass = (*CurrentClassName);
      }
      i++;
    }

    switch (Project->GetOperatorMethod()){
      case MANUAL:
        ProjectSettings.OperatorMethod = MANUAL;
        break;
      case EXHAUSTIVE:
        ProjectSettings.OperatorMethod = EXHAUSTIVE;
        break;
      case MEDIAN:
        ProjectSettings.OperatorMethod = MEDIAN;
        break;
      case ROCAREA:
        ProjectSettings.OperatorMethod = ROCAREA;
        break;
      case ROCDIAGONAL:
        ProjectSettings.OperatorMethod = ROCDIAGONAL;
        break;
    }

    switch (Project->GetCutoffMethod()){
      case USER:
        ProjectSettings.CutoffMethod = USER;
        break;
      case ALL:
        ProjectSettings.CutoffMethod = ALL;
        break;
	  case RVAC:
		ProjectSettings.CutoffMethod = RVAC;
        break;
    }

    ProjectSettings.CutoffFile = CutoffFilename;

    // Restrictions
    ProjectSettings.Rule = Project->GetRestriction();

    // Iterate through featurenames to find the right feature order
    list<string>::iterator CurrentFeatureName(FeatureNames.begin());
    list<string>::iterator LastFeatureName(FeatureNames.end());
    ostringstream FeatureLine;
    vector<Range>* NewRange;
    for (unsigned int i=0; CurrentFeatureName != LastFeatureName; CurrentFeatureName++) {
      CurrentRestriction.Name = (*CurrentFeatureName).c_str();
      CurrentRestriction.Mandatory = Project->GetMandatoryFeature(i);

      NewRange = Project->GetCutoffRanges(i);                                   // Get cutoff-ranges for specified feature
      vector<Range>::iterator CurrentRange((*NewRange).begin());                // Range iterator (might be more than one range)
      vector<Range>::iterator LastRange((*NewRange).end());                     // Range iterator (might be more than one range)
      unsigned int j=0;
      for (; CurrentRange != LastRange; CurrentRange++){
        if (j==0) {
          CurrentRestriction.Range1Start = (*CurrentRange).Min;
          CurrentRestriction.Range1End = (*CurrentRange).Max;
        } else {
          CurrentRestriction.Range2Start = (*CurrentRange).Min;
          CurrentRestriction.Range2End = (*CurrentRange).Max;
        }
        j++;
      }
      for (;j<2;j++){                                                           // Fill in any ranges not set
        if (j==0) {
          CurrentRestriction.Range1Start = MINDOUBLE;
          CurrentRestriction.Range1End = MAXDOUBLE;
        } else {
          CurrentRestriction.Range2Start = MINDOUBLE;
          CurrentRestriction.Range2End = MAXDOUBLE;
        }
      }
      switch (Project->GetOperator(i)){
        case GREATER:
          CurrentRestriction.Operator = GREATER;
          break;
        case LESS:
          CurrentRestriction.Operator = LESS;
          break;
        case GREATERLESS:
          CurrentRestriction.Operator = GREATERLESS;
          break;
        case NONE:
          CurrentRestriction.Operator = NONE;
          break;
      }
      ProjectSettings.FeatureRestrictions.push_back(CurrentRestriction);
      i++;
    }

    // Constraints
    switch (Project->GetMaximizeMeasure()){
      case SENSITIVITY:
        ProjectSettings.Maximize = SENSITIVITY;
        break;
      case SPECIFICITY:
        ProjectSettings.Maximize = SPECIFICITY;
        break;
      case ACCURACY:
        ProjectSettings.Maximize = ACCURACY;
        break;
      case PPV:
        ProjectSettings.Maximize = PPV;
        break;
      case NPV:
        ProjectSettings.Maximize = NPV;        
        break;
    }

    vector<CONSTRAINT> Constraints = Project->GetConstraints();
    vector<CONSTRAINT>::iterator CurrentConstraint(Constraints.begin());
    vector<CONSTRAINT>::iterator LastConstraint(Constraints.end());
    for (; CurrentConstraint != LastConstraint; CurrentConstraint++) {
      switch ((*CurrentConstraint).Measure) {
        case SENSITIVITY:
          if ((*CurrentConstraint).Value != 0){
            ProjectSettings.Sensitivity = (*CurrentConstraint).Value;
          }
          break;
        case SPECIFICITY:
          if ((*CurrentConstraint).Value != 0){
            ProjectSettings.Specificity = (*CurrentConstraint).Value;
          }
          break;
        case PPV:
          if ((*CurrentConstraint).Value != 0){
            ProjectSettings.PPV = (*CurrentConstraint).Value;
          }
          break;
        case NPV:
          if ((*CurrentConstraint).Value != 0){
            ProjectSettings.NPV = (*CurrentConstraint).Value;
          }
          break;
        case ACCURACY:
          if ((*CurrentConstraint).Value != 0){
            ProjectSettings.Accuracy = (*CurrentConstraint).Value;
          }
          break;
      }
    }
    ProjectFile.flush();

    // Output Settings
    switch (Project->GetRuleOutputMethod()){
      case EVERY:
        ProjectSettings.OutputMethod = EVERY;
        break;
      case INCREMENT:
        ProjectSettings.OutputMethod = INCREMENT;
        break;
      case BEST:
        ProjectSettings.OutputMethod = BEST;
        break;
    }

    ProjectSettings.PrintSettings = false;
    if (Project->GetPrintSettings()) {
      ProjectSettings.PrintSettings = true;
    } 
    ProjectSettings.PrintPartitions = false;
    if (Project->GetPrintPartitions()) {
      ProjectSettings.PrintPartitions = true;
    } 
    ProjectSettings.PrintFeatureOperators = false;
    if (Project->GetPrintFeatureOperators()) {
      ProjectSettings.PrintFeatureOperators = true;
    } 
    ProjectSettings.PrintCutoffMethod = false;
    if (Project->GetPrintCutoffMethod()) {
      ProjectSettings.PrintCutoffMethod = true;
    } 
    ProjectSettings.PrintCutoffValues = false;
    if (Project->GetPrintCutoffValues()) {
      ProjectSettings.PrintCutoffValues = true;
    } 
    ProjectSettings.PrintOperatorMethod = false;
    if (Project->GetPrintOperatorMethod()) {
      ProjectSettings.PrintOperatorMethod = true;
    } 
    ProjectSettings.PrintOperatorValues = false;
    if (Project->GetPrintOperatorValues()) {
      ProjectSettings.PrintOperatorValues = true;
    } 
    ProjectSettings.PrintCombinations = false;
    if (Project->GetPrintCombinations()) {
      ProjectSettings.PrintCombinations = true;
    } 
    ProjectSettings.PrintFeatureSets = false;
    if (Project->GetPrintFeatureSets()) {
      ProjectSettings.PrintFeatureSets = true;
    } 
    ProjectSettings.PrintCutoffSets = false;
    if (Project->GetPrintCutoffSets()) {
      ProjectSettings.PrintCutoffSets = true;
    }
    ProjectSettings.PrintCutoffSetsBestLength = false;
    if (Project->GetPrintCutoffSetsBestLength()) {
        ProjectSettings.PrintCutoffSetsBestLength = true;
    }
    ProjectSettings.PrintPerformance = false;
    if (Project->GetPrintPerformance()) {
      ProjectSettings.PrintPerformance = true;
    } 
    ProjectSettings.PrintSets = false;
    if (Project->GetPrintSets()) {
      ProjectSettings.PrintSets = true;
    } 
    ProjectSettings.SavePartitions = false;
    if (Project->GetSavePartitions()) {
      ProjectSettings.SavePartitions = true;
    }
    // Run Settings
	ProjectSettings.BranchBound = false;
	if (Project->GetBranchBound()) {
	  ProjectSettings.BranchBound = true;
	}
	ProjectSettings.SubSumption = false;
	if (Project->GetSubSumption()) {
	  ProjectSettings.SubSumption = true;
	}

	return SaveSettingsToFile(IOFilename);
}

bool IOExplore::SaveSettingsToFile(string IOFilename) {
  char Buffer[MAX_LINESIZE];
  ProjectFilename = IOFilename;
  ProjectFile.open(ProjectFilename.c_str(),ios::out);

  if (ProjectFile.is_open()) {
    ProjectFile.clear();
    ProjectFile << "[General]" << endl;
    ProjectFile << "Name="        << ProjectSettings.Name << endl;
    ProjectFile << "Remarks="     << ProjectSettings.Remarks << endl;
    ProjectFile << "DataFile="    << ProjectSettings.DataFile << endl;
    ProjectFile << "Output="      << ProjectSettings.OutputType << endl;
    ProjectFile << "OutputFile="  << ProjectSettings.OutputFile << endl;
    if (ProjectSettings.IncrementalOutputFile) {
      ProjectFile << "IncrementalOutputFile=true" << endl;
    } else {
      ProjectFile << "IncrementalOutputFile=false" << endl;    
    }
    ProjectFile << "[Setup]" << endl;
    switch (ProjectSettings.PartitionMethod) {
      case RESUBSTITUTION:
        ProjectFile << "PartitionMethod=RESUBSTITUTION" << endl;
        break;
      case HOLDOUT:
        ProjectFile << "PartitionMethod=HOLDOUT" << endl;
        break;
      case CROSS_VALIDATION:
        ProjectFile << "PartitionMethod=CROSS_VALIDATION" << endl;
        break;
    }
    if (ProjectSettings.Randomize) {
      ProjectFile << "Randomize=yes" << endl;
    } else {
      ProjectFile << "Randomize=no" << endl;    
    }
    ProjectFile << "StartRulelength=" << ProjectSettings.StartRulelength << endl;
    ProjectFile << "EndRulelength=" << ProjectSettings.EndRulelength << endl;
	ProjectFile << "Runs=" << ProjectSettings.Runs << endl;
	ProjectFile << "LearnRatio=" << ProjectSettings.LearnRatio << endl;
    ProjectFile << "NumberofPartitions=" << ProjectSettings.NumberofPartitions << endl;
    switch (ProjectSettings.OperatorMethod) {
      case MANUAL:
        ProjectFile << "OperatorMethod=MANUAL" << endl;
        break;
      case EXHAUSTIVE:
        ProjectFile << "OperatorMethod=EXHAUSTIVE" << endl;
        break;
      case MEDIAN:
        ProjectFile << "OperatorMethod=MEDIAN" << endl;
        break;
      case ROCAREA:
        ProjectFile << "OperatorMethod=ROCAREA" << endl;
        break;
      case ROCDIAGONAL:
        ProjectFile << "OperatorMethod=ROCDIAGONAL" << endl;
        break;
    }
    switch (ProjectSettings.CutoffMethod) {
      case USER:
        ProjectFile << "CutoffMethod=USER" << endl;
        break;
      case ALL:
        ProjectFile << "CutoffMethod=ALL" << endl;
        break;
	  case RVAC:
        ProjectFile << "CutoffMethod=RVA" << endl;
        break;
    }
    ProjectFile << "CutoffFile=" << ProjectSettings.CutoffFile << endl;
    ProjectFile << "PositiveClass=" << ProjectSettings.PositiveClass << endl;
    ProjectFile << "[Restrictions]" << endl;
    ProjectFile << "Rule=" << ProjectSettings.Rule << endl;

    vector<FeatureRestriction>::iterator CurrentRestriction(ProjectSettings.FeatureRestrictions.begin());
    vector<FeatureRestriction>::iterator LastRestriction(ProjectSettings.FeatureRestrictions.end());
    for (; CurrentRestriction != LastRestriction; CurrentRestriction++) {
      ProjectFile << "Feature=";
      ProjectFile << (*CurrentRestriction).Name << ";";
      if ((*CurrentRestriction).Mandatory) {
        ProjectFile << "yes;";
      } else {
        ProjectFile << "no;";
      }
      if ((*CurrentRestriction).Range1Start != MINDOUBLE) {
        ProjectFile << (*CurrentRestriction).Range1Start;
      }
      ProjectFile << ";";
      if ((*CurrentRestriction).Range1End != MAXDOUBLE) {
        ProjectFile << (*CurrentRestriction).Range1End;
      }
      ProjectFile << ";";
      if ((*CurrentRestriction).Range2Start != MINDOUBLE) {
        ProjectFile << (*CurrentRestriction).Range2Start;
      }
      ProjectFile << ";";
      if ((*CurrentRestriction).Range2End != MAXDOUBLE) {
        ProjectFile << (*CurrentRestriction).Range1End;
      }
      ProjectFile << ";";
      switch ((*CurrentRestriction).Operator) {
        case GREATER:
          ProjectFile << "GREATER";
          break;
        case LESS:
          ProjectFile << "LESS";
          break;
        case GREATERLESS:
          ProjectFile << "GREATERLESS";
          break;
        case NONE:
          ProjectFile << "NONE";        
          break;
      }
      ProjectFile << endl;
    }

    ProjectFile << "[Constraints]" << endl;
    switch (ProjectSettings.Maximize) {
      case SENSITIVITY:
        ProjectFile << "Maximize=SENSITIVITY" << endl;
        break;
      case SPECIFICITY:
        ProjectFile << "Maximize=SPECIFICITY" << endl;
        break;
      case ACCURACY:
        ProjectFile << "Maximize=ACCURACY" << endl;
        break;
      case PPV:
        ProjectFile << "Maximize=PPV" << endl;
        break;
      case NPV:
        ProjectFile << "Maximize=NPV" << endl;
        break;
    }
    if (ProjectSettings.Sensitivity>0) {
      ProjectFile << "Sensitivity=" << ProjectSettings.Sensitivity << endl;
    }
    if (ProjectSettings.Specificity>0) {
      ProjectFile << "Specificity=" << ProjectSettings.Specificity << endl;
    }
    if (ProjectSettings.PPV>0) {
      ProjectFile << "PPV=" << ProjectSettings.PPV << endl;
    }
    if (ProjectSettings.NPV>0) {
      ProjectFile << "NPV=" << ProjectSettings.NPV << endl;
    }
    if (ProjectSettings.Accuracy>0) {
      ProjectFile << "Accuracy=" << ProjectSettings.Accuracy << endl;
    }
    ProjectFile << "[Output]" << endl;
    switch (ProjectSettings.OutputMethod) {
      case EVERY:
        ProjectFile << "OutputMethod=EVERY" << endl;
        break;
      case INCREMENT:
        ProjectFile << "OutputMethod=INCREMENT" << endl;
        break;
      case BEST:
        ProjectFile << "OutputMethod=BEST" << endl;
        break;
    }
    if (ProjectSettings.PrintSettings) {
      ProjectFile << "PrintSettings=yes" << endl;
    } else {
      ProjectFile << "PrintSettings=no" << endl;
    }
    if (ProjectSettings.PrintPartitions) {
      ProjectFile << "PrintPartitions=yes" << endl;
    } else {
      ProjectFile << "PrintPartitions=no" << endl;
    }
    if (ProjectSettings.PrintFeatureOperators) {
      ProjectFile << "PrintFeatureOperators=yes" << endl;
    } else {
      ProjectFile << "PrintFeatureOperators=no" << endl;
    }
    if (ProjectSettings.PrintCutoffMethod) {
      ProjectFile << "PrintCutoffMethod=yes" << endl;
    } else {
      ProjectFile << "PrintCutoffMethod=no" << endl;
    }
    if (ProjectSettings.PrintCutoffValues) {
      ProjectFile << "PrintCutoffValues=yes" << endl;
    } else {
      ProjectFile << "PrintCutoffValues=no" << endl;
    }
    if (ProjectSettings.PrintOperatorMethod) {
      ProjectFile << "PrintOperatorMethod=yes" << endl;
    } else {
      ProjectFile << "PrintOperatorMethod=no" << endl;
    }
    if (ProjectSettings.PrintOperatorValues) {
      ProjectFile << "PrintOperatorValues=yes" << endl;
    } else {
      ProjectFile << "PrintOperatorValues=no" << endl;
    }
    if (ProjectSettings.PrintCombinations) {
      ProjectFile << "PrintCombinations=yes" << endl;
    } else {
      ProjectFile << "PrintCombinations=no" << endl;
    }
    if (ProjectSettings.PrintFeatureSets) {
      ProjectFile << "PrintFeatureSets=yes" << endl;
    } else {
      ProjectFile << "PrintFeatureSets=no" << endl;
    }
    if (ProjectSettings.PrintCutoffSets) {
      ProjectFile << "PrintCutoffSets=yes" << endl;
    } else {
      ProjectFile << "PrintCutoffSets=no" << endl;
    }
    if (ProjectSettings.PrintPerformance) {
      ProjectFile << "PrintPerformance=yes" << endl;
    } else {
      ProjectFile << "PrintPerformance=no" << endl;
    }
    if (ProjectSettings.PrintSets) {
      ProjectFile << "PrintSets=yes" << endl;
    } else {
      ProjectFile << "PrintSets=no" << endl;
    }
    if (ProjectSettings.SavePartitions) {
      ProjectFile << "SavePartitions=yes" << endl;
    } else {
      ProjectFile << "SavePartitions=no" << endl;
    }
    ProjectFile << "[Run]" << endl;
	if (ProjectSettings.BranchBound) {
	  ProjectFile << "BranchBound=yes" << endl;
	} else {
	  ProjectFile << "BranchBound=no" << endl;
	}
	if (ProjectSettings.SubSumption) {
	  ProjectFile << "SubSumption=yes" << endl;
	} else {
	  ProjectFile << "SubSumption=no" << endl;
	}
	ProjectFile.flush();
	ProjectFile.close();
	return true;
  }
  return false;
}

/**********************************************************************
Function: SetupExploreFromProject()
Category: Selectors
Scope: public
In: string, filename of project-file
Out: -
Description: Saves all settings associated with Explore reference to
a project file.
**********************************************************************/
bool IOExplore::SetupExploreFromProject(string IOFilename) {

  ProjectFilename = (char*)IOFilename.c_str();

  ProjectFile.open(ProjectFilename.c_str(),ios::in);

  ClearSettings();

  string CurrentHeading;
  string CurrentValue;

  bool ParameterDone;
  vector<string> FeatureParameters;                                             // Restrictions for features (mandatory, minrange1, maxrange1, minrange2, maxrange2, operator)
  FeatureRestriction CurrentRestriction;                                        // Restrictions per feature

  // Clear any existing error-messages from previous loads
  ProjectLoadErrors.clear();

  // Is the project-file open
  if (ProjectFile.is_open()) {
	char* MyToken;
	char Buffer[MAX_LINESIZE];
	char* Dummy;

	ProjectFile.getline(Buffer,MAX_LINESIZE,ENDL_SEPARATOR);
	while (!ProjectFile.fail()) {
	  // Explode buffer into parameter and value
	  ParameterDone = false;

	  while (!ParameterDone) {
		Dummy = Buffer;

		if (Dummy[0] == '\0') {
		    ParameterDone = true;
		}

		MyToken = str_parse(&Dummy,"=");
		for (unsigned int i=0; MyToken; i++) {
		  if (!i) {
			CurrentHeading = MyToken;
		  } else {
			ParameterDone = true;
			CurrentValue = MyToken;
		  }
		  MyToken = str_parse(&Dummy,"=");
		}
		if (!ParameterDone) {
		  ProjectFile.getline(Buffer,MAX_LINESIZE,ENDL_SEPARATOR);
		}
	  }
	  //printf("heading: '%s' value: '%s'\n", CurrentHeading.c_str(), CurrentValue.c_str());
	  // Read parameters based on section and setup explore
	  // General Settings
	  if (CurrentHeading.compare("Name")==0) {                                  // Project name
		ProjectSettings.Name = CurrentValue;
	  }
	  if (CurrentHeading.compare("Remarks")==0) {                               // Project remarks
		ProjectSettings.Remarks = CurrentValue;
	  }
	  if (CurrentHeading.compare("DataFile")==0) {                              // Population data-file
		// FeatureNames stored in UIExplore
		// ClassNames stored in UIExplore
		ProjectSettings.DataFile = CurrentValue;
	  }
	  if (CurrentHeading.compare("Output")==0) {                                // Output type (screen/file)
		if (CurrentValue.compare("Screen")==0) {
		  // Nothing (UIExplore local settings)
		  ProjectSettings.OutputType = "Screen";
		} else if (CurrentValue.compare("File")==0) {
		  ProjectSettings.OutputType = "File";
		} else {
		  ProjectLoadErrors.push_back("Invalid value for result output-type.");
		  return false;
		}
	  }
	  if (CurrentHeading.compare("OutputFile")==0) {                            // Output filename
		ProjectSettings.OutputFile = CurrentValue;
	  }

	  if (CurrentHeading.compare("IncrementalOutputFile")==0) {                 // Incremental Output filename
		if (CurrentValue.compare("true")==0) {
		  ProjectSettings.IncrementalOutputFile = true;
		} else {
		  ProjectSettings.IncrementalOutputFile = false;
		}
	  }

	  // Setup Settings
	  if (CurrentHeading.compare("PartitionMethod")==0) {                       // Partitionmethod
		if (CurrentValue.compare("RESUBSTITUTION")==0) {
		  ProjectSettings.PartitionMethod = RESUBSTITUTION;
		} else if (CurrentValue.compare("HOLDOUT")==0) {
		  ProjectSettings.PartitionMethod = HOLDOUT;
		} else if (CurrentValue.compare("CROSS_VALIDATION")==0) {
		  ProjectSettings.PartitionMethod = CROSS_VALIDATION;
		} else {
		  // Error
		  ProjectLoadErrors.push_back("Invalid value for partition-method.");
		  return false;
		}
	  }
	  if (CurrentHeading.compare("Randomize")==0) {                             // Randomize population
		if (CurrentValue.compare("yes")==0) {
		  ProjectSettings.Randomize = true;
		} else if (CurrentValue.compare("no")==0) {
		  ProjectSettings.Randomize = false;
		} else {
		  ProjectLoadErrors.push_back("Invalid value for population randomization.");
		  return false;
		}
	  }
	  if (CurrentHeading.compare("Seed")==0) {                             // Randomize population
		if (atoi(CurrentValue.c_str())==0) {
		  ProjectSettings.Seed = 0;
		} else {
			if (atoi(CurrentValue.c_str())>0) {
			  ProjectSettings.Seed = atoi(CurrentValue.c_str());
			} else {

			  ProjectLoadErrors.push_back("Invalid value for seed.");
			  return false;
			}
	    }
	  }
	  if (CurrentHeading.compare("StartRulelength")==0) {                       // Starting rulelength
		if (atoi(CurrentValue.c_str())>0) {
		  ProjectSettings.StartRulelength = atoi(CurrentValue.c_str());
		} else {
		  ProjectLoadErrors.push_back("Invalid value for starting rulelength.");
		  return false;
		}
	  }
	  if (CurrentHeading.compare("EndRulelength")==0) {                         // Ending rulelength
		if (atoi(CurrentValue.c_str())>0) {
		  ProjectSettings.EndRulelength = atoi(CurrentValue.c_str());
		} else {
		  ProjectLoadErrors.push_back("Invalid value for ending rulelength.");
		  return false;
		}
	  }
	  if (CurrentHeading.compare("Runs")==0) {                         			// Ending rulelength
		if (atoi(CurrentValue.c_str())>0) {
		  ProjectSettings.Runs = atoi(CurrentValue.c_str());
		  if (atoi(CurrentValue.c_str())>1){                                    // override conflicting settings
			ProjectSettings.IncrementalOutputFile = true;
			ProjectSettings.Randomize = true;}
		} else {
		  ProjectLoadErrors.push_back("Invalid value for number of runs.");
		  return false;
		}
	  }

	  if (CurrentHeading.compare("LearnRatio")==0) {                            // Learn ratio
		if (atof(CurrentValue.c_str())>0 && atof(CurrentValue.c_str())<1) {
		  ProjectSettings.LearnRatio = atof(CurrentValue.c_str());
		} else {
		  ProjectLoadErrors.push_back("Invalid value for population learn-ratio.");
		  return false;
		}
	  }
	  if (CurrentHeading.compare("NumberofPartitions")==0) {                    // Number of partitions
		if (atoi(CurrentValue.c_str())>0) {
		  ProjectSettings.NumberofPartitions = atoi(CurrentValue.c_str());
		} else {
		  ProjectLoadErrors.push_back("Invalid value for number of partitions.");
		  return false;
		}
	  }
	  if (CurrentHeading.compare("ClassFeature")==0) {                          // Positive class
		ProjectSettings.ClassFeature = CurrentValue;
	  }
	  if (CurrentHeading.compare("PositiveClass")==0) {                         // Positive class
		ProjectSettings.PositiveClass = CurrentValue;
	  }
	  if (CurrentHeading.compare("OperatorMethod")==0) {                        // Operator-method
		if (CurrentValue.compare("MANUAL")==0) {
		  ProjectSettings.OperatorMethod = MANUAL;
		} else if (CurrentValue.compare("EXHAUSTIVE")==0) {
		  ProjectSettings.OperatorMethod = EXHAUSTIVE;

		} else if (CurrentValue.compare("MEDIAN")==0) {
		  ProjectSettings.OperatorMethod = MEDIAN;
		} else if (CurrentValue.compare("ROCAREA")==0) {
		  ProjectSettings.OperatorMethod = ROCAREA;
		} else if (CurrentValue.compare("ROCDIAGONAL")==0) {
		  ProjectSettings.OperatorMethod = ROCDIAGONAL;
		} else {
		  ProjectLoadErrors.push_back("Invalid value for operator-method.");
		  return false;
		}
	  }
	  if (CurrentHeading.compare("CutoffMethod")==0) {                          // Cutoff-method
		if (CurrentValue.compare("USER")==0) {
		  ProjectSettings.CutoffMethod = USER;
		} else if (CurrentValue.compare("ALL")==0) {
		  ProjectSettings.CutoffMethod = ALL;
		} else if (CurrentValue.compare("RVAC")==0) {
		  ProjectSettings.CutoffMethod = RVAC;
		} else {
		  ProjectLoadErrors.push_back("Invalid value for cutoff-method.");
		  return false;
		}
	  }
	  if (CurrentHeading.compare("CutoffFile")==0) {                            // Cutoff-file
		ProjectSettings.CutoffFile = CurrentValue;
	  }
	  // Restrictions
	  if (CurrentHeading.compare("Rule")==0) {                                  // Rule/combination restriction
		ProjectSettings.Rule = CurrentValue;
	  }

        if (CurrentHeading.compare("FeatureInclude")==0) {                                  // Include mandatory features
            ProjectSettings.FeatureInclude = CurrentValue;
        }

	  if (CurrentHeading.compare("FeatureRule")==0) {                               // Feature restriction (feature, mandatory, minrange1, maxrange1, minrange2, maxrange2, operator)
		FeatureParameters.clear();
		char* FeatureRestriction = (char*)CurrentValue.c_str();
		MyToken = str_parse(&FeatureRestriction,";");
		while (MyToken) {
		  FeatureParameters.push_back(MyToken);
		  MyToken = str_parse(&FeatureRestriction,";");
		}
		if (FeatureParameters.size()==7) {
		  // Iterate through feature names to find the right feature order
		  list<string>::iterator CurrentFeatureName(FeatureNames.begin());
		  list<string>::iterator LastFeatureName(FeatureNames.end());
		  for (unsigned int i=0; CurrentFeatureName != LastFeatureName; CurrentFeatureName++) {
			if ((*CurrentFeatureName)==FeatureParameters[0]) {
			  CurrentRestriction.Name = (*CurrentFeatureName);
			  if (FeatureParameters[1]=="yes") {
				CurrentRestriction.Mandatory = true;
			  } else {
				CurrentRestriction.Mandatory = false;
			  }
			  if (FeatureParameters[2].length()>0 && FeatureParameters[3].length()>0) {
				Range NewRange;
				NewRange.Min = atof(FeatureParameters[2].c_str());
				NewRange.Max = atof(FeatureParameters[3].c_str());
                if (NewRange.Max<=NewRange.Min) {
                  string Error = "Invalid cutoff-range for feature ";
                  Error += FeatureParameters[0];
                  ProjectLoadErrors.push_back(Error);
                  return false;
                } else {
                  CurrentRestriction.Range1Start = NewRange.Min;
                  CurrentRestriction.Range1End = NewRange.Max;
                }
              }
              if (FeatureParameters[4].length()>0 && FeatureParameters[5].length()>0) {
                Range NewRange;
                NewRange.Min = atof(FeatureParameters[4].c_str());
                NewRange.Max = atof(FeatureParameters[5].c_str());
                if (NewRange.Max<=NewRange.Min) {
                  string Error = "Invalid cutoff-range for feature ";
                  Error += FeatureParameters[0];
                  ProjectLoadErrors.push_back(Error);
                  return false;
                } else {
                  CurrentRestriction.Range2Start = NewRange.Min;
                  CurrentRestriction.Range2End = NewRange.Max;
                }
              }
              if (FeatureParameters[6].length()>0) {
                if (FeatureParameters[6]=="GREATER") {
                  CurrentRestriction.Operator = GREATER;
                } else if (FeatureParameters[6]=="LESS") {
                  CurrentRestriction.Operator = LESS;
                } else if (FeatureParameters[6]=="GREATERLESS") {
                  CurrentRestriction.Operator = GREATERLESS;
                } else {
                  string Error = "Invalid operator for feature ";
                  Error += FeatureParameters[0];
                  ProjectLoadErrors.push_back(Error);
                  return false;
                }
              }
              break;
            }
            i++;
          }
        } else {
		  ProjectLoadErrors.push_back("An error occurred while reading feature-restrictions.");
          return false;
        }
      }
      // Constraints
      if (CurrentHeading.compare("Maximize")==0) {                              // Maximize measure
        if (CurrentValue.compare("SENSITIVITY")==0) {
          ProjectSettings.Maximize = SENSITIVITY;
        } else if (CurrentValue.compare("SPECIFICITY")==0) {
          ProjectSettings.Maximize = SPECIFICITY;
        } else if (CurrentValue.compare("ACCURACY")==0) {
          ProjectSettings.Maximize = ACCURACY;
        } else if (CurrentValue.compare("PPV")==0) {
          ProjectSettings.Maximize = PPV;
        } else if (CurrentValue.compare("NPV")==0) {
          ProjectSettings.Maximize = NPV;
        } else {
          ProjectLoadErrors.push_back("Invalid value for maximize measure.");
          return false;
        }
      }
      if (CurrentHeading.compare("Sensitivity")==0) {                           // Sensitivity constraint
        if (atof(CurrentValue.c_str())>0 && atof(CurrentValue.c_str())<1) {
          ProjectSettings.Sensitivity = atof(CurrentValue.c_str());
        } else {
          ProjectLoadErrors.push_back("Invalid value for constraint sensitivity.");
          return false;
        }
      }
      if (CurrentHeading.compare("Specificity")==0) {                           // Specificity constraint
        if (atof(CurrentValue.c_str())>0 && atof(CurrentValue.c_str())<1) {
          ProjectSettings.Specificity = atof(CurrentValue.c_str());
        } else {
          ProjectLoadErrors.push_back("Invalid value for constraint specificity.");
          return false;
        }
      }
      if (CurrentHeading.compare("Accuracy")==0) {                              // Accuracy constraint
        if (atof(CurrentValue.c_str())>0 && atof(CurrentValue.c_str())<1) {
          ProjectSettings.Accuracy = atof(CurrentValue.c_str());
        } else {
          ProjectLoadErrors.push_back("Invalid value for constraint accuracy.");
          return false;
        }
      }
      if (CurrentHeading.compare("PPV")==0) {                                   // PPV constraint
        if (atof(CurrentValue.c_str())>0 && atof(CurrentValue.c_str())<1) {
          ProjectSettings.PPV = atof(CurrentValue.c_str());
        } else {
          ProjectLoadErrors.push_back("Invalid value for constraint PPV.");
          return false;
        }
      }
      if (CurrentHeading.compare("NPV")==0) {                                   // NPV constraint
        if (atof(CurrentValue.c_str())>0 && atof(CurrentValue.c_str())<1) {
          ProjectSettings.NPV = atof(CurrentValue.c_str());
        } else {
          ProjectLoadErrors.push_back("Invalid value for constraint NPV.");
          return false;
        }
      }
      // Output Settings
      if (CurrentHeading.compare("OutputMethod")==0) {                          // Output method (ALL, INCREMENTAL or BEST)
        if (CurrentValue.compare("EVERY")==0) {
          ProjectSettings.OutputMethod = EVERY;
        } else if (CurrentValue.compare("INCREMENT")==0) {
          ProjectSettings.OutputMethod = INCREMENT;
        } else if (CurrentValue.compare("BEST")==0) {
          ProjectSettings.OutputMethod = BEST;
        } else {
          ProjectLoadErrors.push_back("Invalid value for output method.");
          return false;
        }
      }
      if (CurrentHeading.compare("PrintSettings")==0) {                         // Print project settings?
        if (CurrentValue.compare("yes")==0) {
          ProjectSettings.PrintSettings = true;
        } else if (CurrentValue.compare("no")==0) {
          ProjectSettings.PrintSettings = false;
        } else {
          ProjectLoadErrors.push_back("Invalid value for print settings.");
          return false;
        }
      }
      if (CurrentHeading.compare("PrintPartitions")==0) {                       // Print partitions?
        if (CurrentValue.compare("yes")==0) {
          ProjectSettings.PrintPartitions = true;
        } else if (CurrentValue.compare("no")==0) {
          ProjectSettings.PrintPartitions = false;
        } else {
          ProjectLoadErrors.push_back("Invalid value for print partitions.");
          return false;
        }
      }
      if (CurrentHeading.compare("PrintFeatureOperators")==0) {                 // Print featureoperators?
        if (CurrentValue.compare("yes")==0) {
          ProjectSettings.PrintFeatureOperators = true;
        } else if (CurrentValue.compare("no")==0) {
          ProjectSettings.PrintFeatureOperators = false;
        } else {
          ProjectLoadErrors.push_back("Invalid value for print featureoperators.");
          return false;
        }
      }
      if (CurrentHeading.compare("PrintCutoffMethod")==0) {                     // Print debug cutoff method
        if (CurrentValue.compare("yes")==0) {
          ProjectSettings.PrintCutoffMethod = true;
        } else if (CurrentValue.compare("no")==0) {
          ProjectSettings.PrintCutoffMethod = false;
        } else {
          ProjectLoadErrors.push_back("Invalid value for print cutoff-method.");
          return false;
        }
      }
      if (CurrentHeading.compare("PrintCutoffValues")==0) {                     // Print cutoff values
        if (CurrentValue.compare("yes")==0) {
          ProjectSettings.PrintCutoffValues = true;
        } else if (CurrentValue.compare("no")==0) {
          ProjectSettings.PrintCutoffValues = false;
        } else {
          ProjectLoadErrors.push_back("Invalid value for print cutoff-values.");
          return false;
        }
      }
      if (CurrentHeading.compare("PrintOperatorMethod")==0) {                   // Print debug operator method
        if (CurrentValue.compare("yes")==0) {
          ProjectSettings.PrintOperatorMethod = true;
        } else if (CurrentValue.compare("no")==0) {
          ProjectSettings.PrintOperatorMethod = false;
        } else {
          ProjectLoadErrors.push_back("Invalid value for print operator-method.");
          return false;
        }
      }
      if (CurrentHeading.compare("PrintOperatorValues")==0) {                   // Print operator values
        if (CurrentValue.compare("yes")==0) {
          ProjectSettings.PrintOperatorValues = true;
        } else if (CurrentValue.compare("no")==0) {
          ProjectSettings.PrintOperatorValues = false;
        } else {
          ProjectLoadErrors.push_back("Invalid value for print operator-values.");
          return false;
        }
      }
      if (CurrentHeading.compare("PrintCombinations")==0) {                     // Print combinations
        if (CurrentValue.compare("yes")==0) {
          ProjectSettings.PrintCombinations = true;
        } else if (CurrentValue.compare("no")==0) {
          ProjectSettings.PrintCombinations = false;
        } else {
          ProjectLoadErrors.push_back("Invalid value for print combinations.");
          return false;
        }
      }
      if (CurrentHeading.compare("PrintFeatureSets")==0) {                      // Print featuresets
        if (CurrentValue.compare("yes")==0) {
          ProjectSettings.PrintFeatureSets = true;
        } else if (CurrentValue.compare("no")==0) {
          ProjectSettings.PrintFeatureSets = false;
        } else {
          ProjectLoadErrors.push_back("Invalid value for print featuresets.");
          return false;
        }
      }
      if (CurrentHeading.compare("PrintCutoffSets")==0) {                       // Print cutoffsets
        if (CurrentValue.compare("yes")==0) {
          ProjectSettings.PrintCutoffSets = true;
        } else if (CurrentValue.compare("no")==0) {
          ProjectSettings.PrintCutoffSets = false;
        } else {
          ProjectLoadErrors.push_back("Invalid value for print cutoffsets.");
          return false;
        }
      }

        if (CurrentHeading.compare("PrintCutoffSetsBestLength")==0) {                       // Print cutoffsets
            if (CurrentValue.compare("yes")==0) {
                ProjectSettings.PrintCutoffSetsBestLength = true;
            } else if (CurrentValue.compare("no")==0) {
                ProjectSettings.PrintCutoffSetsBestLength = false;
            } else {
                ProjectLoadErrors.push_back("Invalid value for print cutoffsets bestlength.");
                return false;
            }
        }
      if (CurrentHeading.compare("PrintPerformance")==0) {                      // Print performance
        if (CurrentValue.compare("yes")==0) {
          ProjectSettings.PrintPerformance = true;
        } else if (CurrentValue.compare("no")==0) {
          ProjectSettings.PrintPerformance = false;
        } else {
          ProjectLoadErrors.push_back("Invalid value for print performance.");
          return false;
        }
      }
      if (CurrentHeading.compare("PrintSets")==0) {                             // Print performance-sets
        if (CurrentValue.compare("yes")==0) {
          ProjectSettings.PrintSets = true;
        } else if (CurrentValue.compare("no")==0) {
          ProjectSettings.PrintSets = false;
        } else {
          ProjectLoadErrors.push_back("Invalid value for print performance-sets.");
          return false;
        }
      }
      if (CurrentHeading.compare("SavePartitions")==0) {
        if (CurrentValue.compare("yes")==0) {
          ProjectSettings.SavePartitions = true;
        } else if (CurrentValue.compare("no")==0) {
          ProjectSettings.SavePartitions = false;
        } else {
          ProjectLoadErrors.push_back("Invalid value for save partitions.");
          return false;
        }
      }
      // Run Settings
	  if (CurrentHeading.compare("BranchBound")==0) {                         // Use optimization if possible for rule-generation?
		if (CurrentValue.compare("yes")==0) {
		  ProjectSettings.BranchBound = true;
		} else if (CurrentValue.compare("no")==0) {
		  ProjectSettings.BranchBound = false;
		} else {
		  ProjectLoadErrors.push_back("Invalid value for branch and bound optimization.");
		  return false;
		}
	  }
	  if (CurrentHeading.compare("Subsumption")==0) {                         // Use optimization if possible for rule-generation?
		if (CurrentValue.compare("yes")==0) {
		  ProjectSettings.SubSumption = true;
		} else if (CurrentValue.compare("no")==0) {
		  ProjectSettings.SubSumption = false;
		} else {
		  ProjectLoadErrors.push_back("Invalid value for subsumption optimization.");
		  return false;
		}
	  }
	  ProjectFile.getline(Buffer,MAX_LINESIZE,ENDL_SEPARATOR);
	}
    ProjectFile.close();
	return SetupExploreFromStruct();
  }
  return false;
}

bool IOExplore::SetupExploreFromStruct() {
  Project->SetName(ProjectSettings.Name);
  Project->SetDescription(ProjectSettings.Remarks);
  Project->ClearPopulation();
  if (SetDataFile(ProjectSettings.DataFile.c_str())) {
	if (!ReadArffDataFile()) {
      // Error
      ProjectLoadErrors.push_back("An error occured while importing population.");
	  return false;
    }
	CloseDataFile();                                                            // Close input file for reading
  }
  ProjectLoadOutputType = ProjectSettings.OutputType;
  if (ProjectSettings.OutputType.compare("File")==0) {
    CloseResultFile();
    if (!SetResultFile(ProjectSettings.OutputFile)) {
      ProjectLoadErrors.push_back("Couldn't set result file.");
      return false;
    }
  }
  Project->SetPartitionMethod(ProjectSettings.PartitionMethod);
  Project->SetRandomize(ProjectSettings.Randomize);
  if (!ProjectSettings.Randomize) {
	Project->SetSeed(ProjectSettings.Seed);
  }
  SetIncrementalOutputFile(ProjectSettings.IncrementalOutputFile);
  if (ProjectSettings.Runs>1) {                                                 //override seed for multiple runs
	Project->ResetSeed();
  }

  Project->SetMinRuleLength(ProjectSettings.StartRulelength);
  Project->SetMaxRuleLength(ProjectSettings.EndRulelength);
  Project->SetLearnRatio(ProjectSettings.LearnRatio);
  Project->SetNoPartitions(ProjectSettings.NumberofPartitions);


  //set and check the class feature and the positive class

  if (Project->GetFeatureNumber(ProjectSettings.ClassFeature)>-1){
	 Project->SetClassNamesByFName(ProjectSettings.ClassFeature);
  } else {
	ProjectLoadErrors.push_back("Invalid value for class feature");
	return false;
  }

  if (Project->GetClassValueByName(ProjectSettings.PositiveClass)>-1){
	 Project->SetPositiveClass(Project->GetFeatureNumber(ProjectSettings.ClassFeature),
	 Project->GetClassValueByName(ProjectSettings.PositiveClass));
  } else {
	ProjectLoadErrors.push_back("Invalid value for positive class");
	return false;
  }


  Project->SetOperatorMethod(ProjectSettings.OperatorMethod);
  Project->SetCutoffMethod(ProjectSettings.CutoffMethod);

  if (SetCutoffFile(ProjectSettings.CutoffFile.c_str())) {
    if (!ReadCutoffFile()) {
      // Error
      ProjectLoadErrors.push_back("An error occured while importing cutoff fle.");
      return false;
    }
    CloseCutoffFile();                                                      // Close input file for reading
  }
  if (strlen(ProjectSettings.Rule.c_str())>0) {
    if (!Project->SetRestriction(ProjectSettings.Rule)) {
      ProjectLoadErrors.push_back("Invalid restriction.");
      return false;
    }
  }

    if (Project->GetFeatureNumber(ProjectSettings.FeatureInclude)>-1){
        Project->SetMandatoryFeature(Project->GetFeatureNumber(ProjectSettings.FeatureInclude), true);
    }


  Project->SetMaximizeMeasure(ProjectSettings.Maximize);
  if (ProjectSettings.Sensitivity>0 && ProjectSettings.Sensitivity < 1) {
    Project->SetConstraint(SENSITIVITY,ProjectSettings.Sensitivity);
  }
  if (ProjectSettings.Specificity>0 && ProjectSettings.Specificity < 1) {
    Project->SetConstraint(SPECIFICITY,ProjectSettings.Specificity);
  }
  if (ProjectSettings.Accuracy>0 && ProjectSettings.Accuracy < 1) {
    Project->SetConstraint(ACCURACY,ProjectSettings.Accuracy);
  }
  if (ProjectSettings.PPV>0 && ProjectSettings.PPV < 1) {
    Project->SetConstraint(PPV,ProjectSettings.PPV);
  }
  if (ProjectSettings.NPV>0 && ProjectSettings.NPV < 1) {
    Project->SetConstraint(NPV,ProjectSettings.NPV);
  }

  Project->SetRuleOutputMethod(ProjectSettings.OutputMethod);

  Project->SetPrintSettings(ProjectSettings.PrintSettings);
  Project->SetPrintPartitions(ProjectSettings.PrintPartitions);
  Project->SetPrintFeatureOperators(ProjectSettings.PrintFeatureOperators);
  Project->SetPrintCutoffMethod(ProjectSettings.PrintCutoffMethod);
  Project->SetPrintCutoffValues(ProjectSettings.PrintCutoffValues);
  Project->SetPrintOperatorMethod(ProjectSettings.PrintOperatorMethod);
  Project->SetPrintOperatorValues(ProjectSettings.PrintOperatorValues);
  Project->SetPrintCombinations(ProjectSettings.PrintCombinations);
  Project->SetPrintFeatureSets(ProjectSettings.PrintFeatureSets);
  Project->SetPrintCutoffSets(ProjectSettings.PrintCutoffSets);
  Project->SetPrintCutoffSetsBestLength(ProjectSettings.PrintCutoffSetsBestLength);
  Project->SetPrintPerformance(ProjectSettings.PrintPerformance);
  Project->SetPrintSets(ProjectSettings.PrintSets);
  Project->SetSavePartitions(ProjectSettings.SavePartitions);
  Project->SetBranchBound(ProjectSettings.BranchBound);
  Project->SetSubSumption(ProjectSettings.SubSumption);

  return true;
}

/**********************************************************************
Function: GetClassNames()
Category: Selectors
Scope: public
In: -
Out: -
Description: Returns a pointer to a <list> of classnames.
**********************************************************************/
list<string>* IOExplore::GetClassNames() {
  return &ClassNames;
}

/**********************************************************************
Function: GetFeatureNames()
Category: Selectors
Scope: public
In: -
Out: -
Description: Returns a pointer to a <list> of featurenames.
**********************************************************************/
list<string>* IOExplore::GetFeatureNames() {
  return &FeatureNames;
}

/**********************************************************************
Function: GetResultBuffer()
Category: Modifiers
Scope: public
In: -
Out: -
Description: Sets output for an Explore project to the result file.
**********************************************************************/
streambuf* IOExplore::GetResultBuffer() {
  return ResultFile.rdbuf();
}

/**********************************************************************
Function: CloseDataFile()
Category: Modifiers
Scope: public
In: -
Out: -
Description: Closes the data file for reading.
**********************************************************************/
void IOExplore::CloseDataFile() {
  if (DataFile.is_open()) {
    DataFile.close();
  }
}

/**********************************************************************
Function: CloseResultFile()
Category: Modifiers
Scope: public
In: -
Out: -
Description: Closes the result file for writing.
**********************************************************************/
void IOExplore::CloseResultFile() {
  ResultFile.flush();
  if (ResultFile.is_open()) {
    ResultFile.close();
  }
}

/**********************************************************************
Function: CloseCutoffFile()
Category: Modifiers
Scope: public
In: -
Out: -
Description: Closes the Cutoff file for reading.
**********************************************************************/
void IOExplore::CloseCutoffFile() {
  if (CutoffFile.is_open()) {
    CutoffFile.close();
  }
}

/**********************************************************************
Function: ClosePartitionFile()
Category: Modifiers
Scope: public
In: -
Out: -
Description: Closes the partition file.
**********************************************************************/
void IOExplore::ClosePartitionFile() {
  if (PartitionFile.is_open()) {
    PartitionFile.close();
  }
}

/**********************************************************************
Function: Reset()
Category: Modifiers
Scope: public
In: -
Out: -
Description: Resets IOExplore.
**********************************************************************/
void IOExplore::Reset() {
  CloseDataFile();
  CloseResultFile();

  ClassNames.clear();
  FeatureNames.clear();

  DataFilename.clear();
  ResultFilename.clear();
  ProjectFilename.clear();
  PartitionFilename.clear();
}

/**********************************************************************
Function: SetDataFile()
Category: Modifiers
Scope: public
In: string, filename
Out: bool, success or not
Description: Sets the data file and opens the file.
**********************************************************************/
bool IOExplore::SetDataFile(string IOFilename) {
  DataFilename = IOFilename;
  DataFile.open(DataFilename.c_str(),ios::in);
  if (DataFile.is_open()) {
    return true;
  }
  return false;
}

/**********************************************************************
Function: SetLoadOutputType(string LoadOutputType)
Category: Modifiers
Scope: public
In: string, LoadOutputType
Out:
Description: Sets the Loadoutputtype.
**********************************************************************/
void IOExplore::SetLoadOutputType(string LoadOutputType) {
  ProjectLoadOutputType = LoadOutputType;
}

/**********************************************************************
Function: SetResultFile()
Category: Modifiers
Scope: public
In: string, filename
Out: bool, succes or not
Description: Sets the result file and opens it for writing.
**********************************************************************/
bool IOExplore::SetResultFile(string IOFilename) {
  int OutputNumber=1;
  char Prefix[10];

  ResultFilename = IOFilename;
  if (ProjectSettings.IncrementalOutputFile) {                                  // Incremental numbering of files
   sprintf(Prefix,"-%d.result",OutputNumber);
   string temp=Prefix;
   ResultFilename.replace(ResultFilename.find('.'),ResultFilename.length(),temp);
   while (ExistsResultFile(ResultFilename)){
	OutputNumber++;
	sprintf(Prefix,"-%d.result",OutputNumber);
	string temp=Prefix;
	ResultFilename = IOFilename;
    ResultFilename.replace(ResultFilename.find('.'),ResultFilename.length(),temp);
	}
  }

  CloseResultFile();

  ResultFile.open(ResultFilename.c_str(),ios::out);
  if (ResultFile.is_open()) {
	return true;
  }
  return false;
}

/**********************************************************************
Function: SetPartitionFile()
Category: Modifiers
Scope: public
In: string, filename
Out: bool, succes or not
Description: Sets the partition file and opens it for writing.
Couples the savepartitionfilefunction of explore to savepartionfile of
ioexplore
**********************************************************************/
bool IOExplore::SetPartitionFile(string IOFilename) {
  PartitionFile.open(IOFilename.c_str(),ios::out);
  if (PartitionFile.is_open()) {
    return true;
  }
  return false;
}

/**********************************************************************
Function: SetIncrementalOutputFile()
Category: Modifiers
Scope: public
In: -
Out: -
Description: Sets Incremental OuputFile Selection
**********************************************************************/
void IOExplore::SetIncrementalOutputFile(bool Value) {
  IncrementalOutputFile = Value;
}

/**********************************************************************
Function: SetCutoffFile()
Category: Modifiers
Scope: public
In: string, filename
Out: bool, succes or not
Description: Sets the cutoff file and opens it for writing.
**********************************************************************/
bool IOExplore::SetCutoffFile(string IOFilename) {
  CutoffFilename = IOFilename;
  CutoffFile.open(CutoffFilename.c_str(),ios::in);
  if (CutoffFile.is_open()) {
    return true;
  }
  return false;
}

/**********************************************************************
Function: SetNextIncrementalOutputFile()
Category: Modifiers
Scope: public
In: string, filename
Out: bool, succes or not
Description: Sets the next incremental outputfile
**********************************************************************/
bool IOExplore::SetNextIncrementalResultFile() {
  return SetResultFile(ProjectSettings.OutputFile);
}



/**********************************************************************
Function: GetDataFilename()
Category: Selectors
Scope: public
In: -
Out: string, filename
Description: Returns the data-filename.
**********************************************************************/
string IOExplore::GetDataFilename() {
  return DataFilename;
}

/**********************************************************************
Function: GetLoadOutputType()
Category: Modifiers
Scope: public
In: string, LoadOutputType
Out:
Description: Returns the Loadoutputtype.
**********************************************************************/
string IOExplore::GetLoadOutputType() {
  return ProjectLoadOutputType;
}

/**********************************************************************
Function: GetResultFilename()
Category: Selectors
Scope: public
In: -
Out: string, filename
Description: Returns the result-filename.
**********************************************************************/
string IOExplore::GetResultFilename() {
  return ResultFilename;
}

/**********************************************************************
Function: ExistsResultFile()
Category: Modifiers
Scope: public
In: string, filename
Out: bool, succes or not
Description: Determines if the result file already exist
**********************************************************************/
bool IOExplore::ExistsResultFile(string IOFilename) {
  ResultFilename = IOFilename;
  CloseResultFile();
  ResultFile.open(ResultFilename.c_str(),ios::in);
  if (ResultFile.is_open()) {
    return true;
  }
  return false;
}


/**********************************************************************
Function: GetIncrementalOutputFile()
Category: Selectors
Scope: public
In: -
Out: string, project description
Description: Returns the project description.
**********************************************************************/
bool IOExplore::GetIncrementalOutputFile() {
  return IncrementalOutputFile;
}

/**********************************************************************
Function: GetProjectFilename()
Category: Selectors
Scope: public
In: -
Out: string, filename
Description: Returns the project-filename.
**********************************************************************/
string IOExplore::GetProjectFilename() {
  return ProjectFilename;
}

/**********************************************************************
Function: GetCutoffFilename()
Category: Selectors
Scope: public
In: -
Out: string, filename
Description: Returns the cutoff-filename.
**********************************************************************/
string IOExplore::GetCutoffFilename() {
  return CutoffFilename;
}


/**********************************************************************
Function: PrintProjectLoadErros()
Category: Selectors
Scope: public
In: -
Out: string, LoadErrors
Description: Returns the cutoff-filename.
**********************************************************************/
string IOExplore::PrintProjectLoadErrors() {

  ostringstream Result;

  unsigned int CurrentLoadError=0;

  for (; CurrentLoadError < ProjectLoadErrors.size(); CurrentLoadError++) {
	  Result << ProjectLoadErrors[CurrentLoadError];
  }
  return Result.str();
}




