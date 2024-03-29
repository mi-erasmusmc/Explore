//----------------------------------------------------------------------------//
// $Header: //depot/Explore/Explore/roc.h#7 $
// $Author: Rijnbeek $
// $DateTime: 2008/06/02 13:52:45 $
// $Change: 81 $
//----------------------------------------------------------------------------//

#ifndef ROC_H
#define ROC_H

#include <vector>

class ROC {
  private:
	std::vector<double> MaxSensitivity;
    unsigned int RuleLength;

	int Round(double d);
    
  public:
    ROC(unsigned int RRuleLength);
    ~ROC();

    // Modifiers
    void AddEntry(double Specificity, double Sensitivity);

    // Selectors
    double GetEntry(double FPF);
    unsigned int GetRuleLength();
	std::vector<double>* GetROC();
};

#endif
