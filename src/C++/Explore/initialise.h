//----------------------------------------------------------------------------//
// $Header: //depot/Explore/Explore/initialise.h#16 $
// $Author: Rijnbeek $
// $DateTime: 2008/09/09 14:16:02 $
// $Change: 87 $
//----------------------------------------------------------------------------//

#ifndef EXPLORE_COMMON
#define EXPLORE_COMMON

#include "../stl.h"
#include "values.h"

#include <iostream>
#include <sstream>
#include <math.h>
#include <time.h>
#include "timing.h"
 using namespace std;
#define MINIMUM_OBSERVATIONS 2
#define MINIMUM_FEATURES 1

#define MAX_LINESIZE 2000                                                    // Maximum number of characters per line read
#define ENDL_SEPARATOR     '\n'                                                 // Needed for last observation to make sure class on next line is read successfully
#ifdef HYPER
  #include <jibu4c++.h>
#endif
//#define DEBUG_TIMING



// What method should be used for sorting observations in a feature
enum FEATURE_SORT { CLASS, ORDER, VALUE };

// Use simple or RVA cutoff detection
enum CUTOFF_METHOD { USER, ALL, RVAC };     // RVA -> RVAC because of double declaration

// Use median or roc heuristic for finding operators
enum OPERATOR_METHOD { MANUAL, EXHAUSTIVE, MEDIAN, ROCAREA, ROCDIAGONAL };

// Operators for each feature
enum OPERATOR { NONE, GREATER, LESS, GREATERLESS, EQUAL };

// Which method should be used for partitioning the observations
enum PARTITION_METHOD { RESUBSTITUTION, HOLDOUT, LEAVE_ONE_OUT, CROSS_VALIDATION };

// Function of this partition (for learning, for testing or both)
enum PARTITION_TYPE { LEARN, VALIDATION, TRAIN };

// Different ways in which rules can be displayed in Explore
enum RULE_OUTPUT_METHOD { EVERY, INCREMENT, BEST };

// Performance variable that has to be maximised
enum PERFORMANCE_MEASURE { SENSITIVITY, SPECIFICITY, PPV, NPV, ACCURACY, BALANCEDACCURACY, F1SCORE};

// Cutoff-ranges
struct Range {
  float Min;
  float Max;
 
  Range() {
     Min = MINFLOAT;
     Max = MAXFLOAT;
  }

  bool InRange(float Value) {
    if (Value>=Min && Value<=Max) {
      return true;
    }
    return false;
  }
};

// Class ratios
struct ClassRatio {
  unsigned int Class;
  float Ratio;
  vector<unsigned int> Observations;
};

#endif


