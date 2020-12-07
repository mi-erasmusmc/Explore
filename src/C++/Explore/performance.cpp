//----------------------------------------------------------------------------//
// $Header: //depot/Explore/Explore/performance.cpp#11 $
// $Author: Pepe $
// $DateTime: 2004/10/11 08:11:59 $
// $Change: 100 $
//----------------------------------------------------------------------------//

#include "performance.h"

void PERFORMANCE::Print() {
  cout << "SE:" << (Sensitivity.Value*100) << " ";
  cout << "SP:" << (Specificity.Value*100) << " ";
  cout << "ACC:" << (Accuracy.Value*100) << " ";
  cout << "PPV:" << (PPV.Value*100) << " ";
  cout << "NPV:" << (NPV.Value*100) << " ";
  cout << endl;
}

string PERFORMANCE::PrintString(bool headers) {
  ostringstream Result;
  
  Result.precision(4);
  Result.width(4);
  if (headers) {
    Result << "SE: " << (Sensitivity.Value*100) << " ";
    Result << "SP: " << (Specificity.Value*100) << " ";
    Result << "ACC:" << (Accuracy.Value*100) << " ";
    Result << "PPV:" << (PPV.Value*100) << " ";
    Result << "NPV:" << (NPV.Value*100) << " ";
  }
  else {
    Result << (Sensitivity.Value*100) << "\t";
    Result << (Specificity.Value*100) << "\t ";
    Result << (Accuracy.Value*100) << "\t ";
    Result << (PPV.Value*100) << "\t ";
    Result << (NPV.Value*100);

  }
  return Result.str();
}

