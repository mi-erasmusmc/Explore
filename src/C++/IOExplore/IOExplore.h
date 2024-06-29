//----------------------------------------------------------------------------//
// $Header: //depot/Explore/IOExplore/IOExplore.h#18 $
// $Author: Rijnbeek $
// $DateTime: 2008/09/09 14:16:02 $
// $Change: 87 $
//----------------------------------------------------------------------------//

#ifndef ExploreIO
#define ExploreIO

#include "../Explore/explore.h"
#include "../Explore/values.h"

struct FeatureRestriction {
  string    Name;
  bool      Mandatory;
  double    Range1Start;
  double    Range1End;
  double    Range2Start;
  double    Range2End;
  OPERATOR  Operator;
};

struct ExploreSettings {
  string                      Name;                                             // General
  string                      Remarks;
  string                      DataFile;
  string                      OutputType;
  string                      OutputFile;
  string					  ProjectFile;
  bool                        IncrementalOutputFile;
  PARTITION_METHOD            PartitionMethod;                                  // Setup
  bool                        Randomize;
  long int					  Seed;
  unsigned int                StartRulelength;
  unsigned int                EndRulelength;
  unsigned int				  Runs;
  float                       LearnRatio;
  unsigned int                NumberofPartitions;
  string                      ClassFeature;
  string                      PositiveClass;
  OPERATOR_METHOD             OperatorMethod;
  CUTOFF_METHOD               CutoffMethod;
  string                      CutoffFile;
  string                      Rule;                                             // Restrictions
  string                      FeatureInclude;
  vector<FeatureRestriction>  FeatureRestrictions;
  PERFORMANCE_MEASURE         Maximize;
  double                      Sensitivity;
  double                      Specificity;
  double                      NPV;
  double                      PPV;
  double                      Accuracy;
  double                      BalancedAccuracy;
  double                      F1score;
  RULE_OUTPUT_METHOD          OutputMethod;                                     // Output
  bool                        PrintSettings;
  bool                        PrintPartitions;
  bool                        PrintFeatureOperators;
  bool                        PrintCutoffMethod;
  bool                        PrintCutoffValues;
  bool                        PrintOperatorMethod;
  bool                        PrintOperatorValues;
  bool                        PrintCombinations;
  bool                        PrintFeatureSets;
  bool                        PrintCutoffSets;
  bool                        PrintCutoffSetsBestLength;
  bool                        PrintPerformance;
  bool                        PrintSets;
  bool                        SavePartitions;
  bool                        BranchBound;
  bool						  SubSumption;
  bool                        Parallel;
  PARALLEL_METHOD             ParallelMethod;
  bool                        BinaryReduction;
};

class IOExplore {
  private:
    Explore *Project;

    fstream DataFile;
    fstream ResultFile;
    fstream ProjectFile;
    fstream CutoffFile;
    fstream PartitionFile;

    string DataFilename;
    string ResultFilename;
    string ProjectFilename;
    string CutoffFilename;
    string PartitionFilename;

    string ProjectLoadOutputType;                                               // Output-type selected in project file (indirect pass-through to UIExplore)

    bool IncrementalOutputFile;

    streambuf *OutputBuffer;

    // Needed for setting up Explore and ExploreGUI
    list<string> ClassNames;
    list<string> FeatureNames;

    // Lists sections and parameters of project-file
    vector< vector<string> > Sections;

    void ClearSettings();                                                       // Clear struct with current project settings
    bool SaveSettingsToFile(string IOFilename);                                 // Saves settings in struct to file

  public:

	// Settings of current project-file as a struct
	ExploreSettings ProjectSettings;

	// Public settings
	vector<string> ProjectLoadErrors;                                           // Errors occurred during loading of a project
	vector<string> ProjectSaveErrors;                                           // Errors occurred during saving of project

	IOExplore();
	~IOExplore();
    bool SetupExploreFromStruct();                                              // Initialises Explore from struct

	void SetProject(Explore* NewProject);

    bool SetDataFile(string IOFilename);
    void SetLoadOutputType(string LoadOutputType);
	bool SetResultFile(string IOFilename);
    bool SetPartitionFile(string IOFilename);
    void SetIncrementalOutputFile(bool Setting);                                // Set Incremental OutputFile
	bool SetCutoffFile(string IOFilename);
    bool SetNextIncrementalResultFile();

    void CloseDataFile();
    void CloseResultFile();
    void CloseCutoffFile();
    void ClosePartitionFile();

	bool ReadDataFile();
	bool ReadArffDataFile();
    bool ReadCutoffFile();

    void SavePartitionFile(string Output, PARTITION_TYPE Type, unsigned int PartNr);
	bool SaveExploreToProject(string IOFilename);
	bool SetupExploreFromProject(string IOFilename);

    string GetDataFilename();
    string GetLoadOutputType();
    string GetResultFilename();
    bool ExistsResultFile(string IOFilename);
    bool GetIncrementalOutputFile();                                            // Returns the incremental output selection
    string GetProjectFilename();
    string GetCutoffFilename();

    streambuf *GetResultBuffer();

	void Reset();

	string PrintProjectLoadErrors();

    // Classes and Features interface
	list<string>* GetClassNames();
    list<string>* GetFeatureNames();
};

#endif
