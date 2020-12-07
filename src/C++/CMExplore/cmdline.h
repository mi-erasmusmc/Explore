class TCmdline {
  public:
																				// Constructor
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
}
