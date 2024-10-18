//----------------------------------------------------------------------------//
// $Header: //depot/Explore/Explore/candidate.cpp#20 $
// $Author: Rijnbeek $
// $DateTime: 2008/09/09 14:16:02 $
// $Change: 87 $
//----------------------------------------------------------------------------//

#include "candidate.h"
#include "../common.h"
#include <tbb/concurrent_vector.h>

/**********************************************************************
Function: Size()
Category: Selectors
Scope: public
In: -
Out: -
Description: Returns the size of the candidate (number of features).
**********************************************************************/
unsigned int CANDIDATE::Size() {
  return Features.size();
}

/**********************************************************************
Function: Clear()
Category: Modifiers
Scope: public
In: -
Out: -
Description: Clears the candidate.
**********************************************************************/
void CANDIDATE::Clear() {
  Conjunctions.clear();
  Features.clear();
  Operators.clear();
  Cutoffs.clear();

  Performance.Sensitivity.Value = 0;
  Performance.Specificity.Value = 0;
  Performance.Accuracy.Value = 0;
  Performance.NPV.Value = 0;
  Performance.PPV.Value = 0;
  Performance.BalancedAccuracy.Value = 0;
  Performance.F1score.Value = 0;
}

/**********************************************************************
Function: Print()
Category: Selectors
Scope: public
In: -
Out: -
Description: Prints the candidate to screen.
**********************************************************************/
string CANDIDATE::Print() {
  ostringstream Result;
  unsigned int Index=0;

  for (unsigned int i=0; i < Conjunctions.size(); i++) {
    if (i>0) {
      Result << " OR ";
    }
    for (unsigned int j=0; j < Conjunctions[i]; j++) {
      if (j>0) {
        Result << " AND ";
      }
      if (Features.size()>0) {
        Result << Features[Index];
      } else {
        Result << "(" << Conjunctions[i] << ")";
        break;
      }
      if (Operators[Index]!=NONE) {
        if (Operators[Index]==GREATER) {
          Result << " >";
        }
        if (Operators[Index]==LESS) {
          Result << " <=";
        }
        if (Operators[Index]==GREATERLESS) {
          Result << " <>";
        }
		if (Operators[Index]==EQUAL) {
		  Result << " =";
		}
	  }
      if (Cutoffs.size()>Index) {
		Result << " " << Cutoffs[Index];
      }
      Index++;
    }
  }
  return Result.str();
}

/**********************************************************************
Function: PrintRule()
Category: Selectors
Scope: public
In: -
Out: string, the rule represented as a string
Description: Prints the rule of this candidate.
**********************************************************************/
string CANDIDATE::PrintRule() {
  ostringstream Result;

  unsigned int CurrentCondition=0;
  unsigned int CurrentConjunction=0;

  for (; CurrentConjunction < Conjunctions.size(); CurrentConjunction++) {
    if (CurrentConjunction!=0) {
      Result << " OR ";
    }
    for (unsigned int i=0; i < Conjunctions[CurrentConjunction]; i++) {
      if (i!=0) {
        Result << " AND ";
	  }
 	  Result << FeatureNames[CurrentCondition].c_str() << " " << PrintOperator(Operators[CurrentCondition]).c_str() << " " << Cutoffs[CurrentCondition];
	  CurrentCondition++;
	}
  }

  return Result.str();
}

/**********************************************************************
Function: PrintRuleIndexed()
Category: Selectors
Scope: public
In: -
Out: string, the rule represented as a string
Description: Prints the rule of this candidate (using indexed FeatureNames).
**********************************************************************/
string CANDIDATE::PrintRuleIndexed() {
  ostringstream Result;

  unsigned int CurrentCondition=0;
  unsigned int CurrentConjunction=0;

  for (; CurrentConjunction < Conjunctions.size(); CurrentConjunction++) {
    if (CurrentConjunction!=0) {
      Result << " OR ";
    }
    for (unsigned int i=0; i < Conjunctions[CurrentConjunction]; i++) {
      if (i!=0) {
        Result << " AND ";
      }
      Result << FeatureNames[Features[CurrentCondition]].c_str() << " " << PrintOperator(Operators[CurrentCondition]).c_str() << " " << Cutoffs[CurrentCondition];
      CurrentCondition++;
    }
  }

  return Result.str();
}

/**********************************************************************
Function: PrintRuleAsNumber()
Category: Selectors
Scope: public
In: -
Out: string, the rule represented as a string
Description: Prints the rule of this candidate with Features as their order.
**********************************************************************/
string CANDIDATE::PrintRuleAsNumber() {
  ostringstream Result;

  unsigned int CurrentCondition=0;
  unsigned int CurrentConjunction=0;

  for (; CurrentConjunction < Conjunctions.size(); CurrentConjunction++) {
    if (CurrentConjunction!=0) {
      Result << " OR ";
    }
    for (unsigned int i=0; i < Conjunctions[CurrentConjunction]; i++) {
      if (i!=0) {
        Result << " AND ";
      }
      Result << Features[CurrentCondition]+1 << " " << PrintOperator(Operators[CurrentCondition]) << " " << Cutoffs[CurrentCondition];
      CurrentCondition++;
    }
  }
  return Result.str();
}

/**********************************************************************
Function: PrintFeatureSetAsNumber()
Category: Selectors
Scope: public
In: -
Out: string, the FeatureSet of the rule represented as a string
Description: Prints the FeatureSet of the candidate with Features as their order.
**********************************************************************/
string CANDIDATE::PrintFeatureSetAsNumber() {
  ostringstream Result;

  unsigned int CurrentCondition=0;
  unsigned int CurrentConjunction=0;

  for (; CurrentConjunction < Conjunctions.size(); CurrentConjunction++) {
    if (CurrentConjunction!=0) {
      Result << " OR ";
    }
    for (unsigned int i=0; i < Conjunctions[CurrentConjunction]; i++) {
      if (i!=0) {
        Result << " AND ";
      }
      Result << Features[CurrentCondition]+1 << " " << PrintOperator(Operators[CurrentCondition]) << " ";
      CurrentCondition++;
    }
  }
  return Result.str();
}

/**********************************************************************
Function: PrintCombination()
Category: Selectors
Scope: public
In: -
Out: string, the combination of the rule
Description: Prints the combination of the rule.
**********************************************************************/
string CANDIDATE::PrintCombination() {
  ostringstream Result;

  vector<unsigned int>::iterator CurrentConjunction(Conjunctions.begin());
  vector<unsigned int>::iterator LastConjunction(Conjunctions.end());

  for (; CurrentConjunction != LastConjunction; CurrentConjunction++) {
    Result << "(" << (*CurrentConjunction) << ") ";
  }
  
  return Result.str();
}

/**********************************************************************
Function: PrintPerformance()
Category: Selectors
Scope: public
In: -
Out: string, the performance represented as a string
Description: Prints the performance of this candidate.
**********************************************************************/
string  CANDIDATE::PrintPerformance(bool headers) {
  return Performance.PrintString(headers);
}

/**********************************************************************
Function: IsValid()
Category: Selectors
Scope: public
In: -
Out: bool, candidate is valid/not valide
Description: Returns whether the candidate is valid.
**********************************************************************/
bool CANDIDATE::IsValid() {
  if (Features.size()>0) {
    unsigned int RuleLength=0;

    for (unsigned int i=0; i<Conjunctions.size(); i++) {
      RuleLength += Conjunctions[i];
    }

    if (RuleLength!=Features.size()) {
      return false;
    }
  }
  return true;
}

/**********************************************************************
Function: operator=()
Category: Modifiers
Scope: public
In: CANDIDATE, a candidate
Out: CANDIDATE, the modified candidate
Description: Operator overloading for candidates.
**********************************************************************/
CANDIDATE* CANDIDATE::operator=(CANDIDATE* SourceCandidate) {
  Conjunctions = (*SourceCandidate).Conjunctions;
  Features     = (*SourceCandidate).Features;
  Operators    = (*SourceCandidate).Operators;
  Cutoffs      = (*SourceCandidate).Cutoffs;
  Performance  = (*SourceCandidate).Performance;

  return this;
}

/**********************************************************************
Function: Interpret()
Category: Modifiers
Scope: public
In: string, rule or list of combinations represented as a string
Out: bool, rule/list of combinations could/could not be interpreted
Description: Interprets a string representing a rule or list of
combinations. Cleans it up first and identifies it as a rule or list
of combinations, then tries to interpret.
**********************************************************************/
bool CANDIDATE::Interpret(string StringRule) {
  unsigned int FindPosition=0;
  bool IsCombinations = false;

  Clear();

  FindPosition = StringRule.find("(",0);
  if (FindPosition != string::npos) {                                           // Interpret a list of combinations
    IsCombinations = true;
    while (FindPosition != string::npos) {
      StringRule.erase(FindPosition,1);
      FindPosition = StringRule.find("(");
    }
    FindPosition = StringRule.find(")",0);
    while (FindPosition != string::npos) {
      StringRule.erase(FindPosition,1);
      FindPosition = StringRule.find(")");
    }
  } else {                                                                      // Interpret a rule
    // Replace AND with &
    FindPosition = StringRule.find("AND",0);
    while (FindPosition != string::npos) {
      StringRule.erase(FindPosition,3);
      StringRule.insert(FindPosition,"&");
      FindPosition = StringRule.find("AND",0);
    }

    // Replace OR with |
    FindPosition = StringRule.find("OR",0);
    while (FindPosition != string::npos) {
      StringRule.erase(FindPosition,2);
      StringRule.insert(FindPosition,"|");
      FindPosition = StringRule.find("OR",0);
    }

    // Replace = with nothing (turn <= into <)
    FindPosition = StringRule.find("=");
    while (FindPosition != string::npos) {
      StringRule.erase(FindPosition,1);
      FindPosition = StringRule.find("=");
    }
  }

  if (IsCombinations) {
    return InterpretCombinations(StringRule);
  } else {
    return InterpretRule(StringRule);                        
  }

}

/**********************************************************************
Function: InterpretRule()
Category: Modifiers
Scope: public
In: string, rule represented as a string
Out: bool, rule could/could not be interpreted
Description: Interprets a string representing a rule.
**********************************************************************/
bool CANDIDATE::InterpretRule(string StringRule) {
/*  list<string> StringConjunctions;
  list<string> StringConditions;

  unsigned int NoConditions=0, LastNoConditions=0;
  unsigned int CurrentMatch=0;

  char* MyToken;

  if (StringRule.length()==0) {
	return false;
  }

  // Cut rule up into conjunctions represented as strings
  char* StringDummy = (char*)StringRule.c_str();
  MyToken = str_parse(&StringDummy,"|");
  while (MyToken) {
	StringConjunctions.push_back(MyToken);
	MyToken = str_parse(&StringDummy,"|");
  }

  // Iterate through conjunctions and check their sizes plus cut them up into conditions represented as strings
  list<string>::iterator CurrentStringConjunction(StringConjunctions.begin());
  list<string>::iterator LastStringConjunction(StringConjunctions.end());

  while (CurrentStringConjunction != LastStringConjunction) {
	char* StringDummy = (char*)(*CurrentStringConjunction).c_str();

	// Cut up current conjunction into conditions
	MyToken = str_parse(&StringDummy,"&");
	for (NoConditions=0; MyToken; NoConditions++) {
	  StringConditions.push_back(MyToken);
	  MyToken = str_parse(&StringDummy,"&");
	}

	// Make sure CurrentCombination is equal or less than LastCombination if LastCombination does not equal 0
	if (NoConditions != 0 && (NoConditions <= LastNoConditions || LastNoConditions == 0)) {
	  Conjunctions.push_back(NoConditions);
	  LastNoConditions = NoConditions;
	  CurrentStringConjunction++;
	} else {
	  return false;
	}
  }

  // Iterate through conditions and set operators/cutoffs where necessary
  list<string>::iterator CurrentStringCondition(StringConditions.begin());
  list<string>::iterator LastStringCondition(StringConditions.end());

  unsigned int Position, Part;
  bool Found;
  string Matches = "<>";

  for (; CurrentStringCondition != LastStringCondition; CurrentStringCondition++) {
	Position = 0;                                                               // Position for verification of operator
	Found = false;                                                              // Operator found in this conditionset?
	Part = 0;                                                                   // Indicates feature or cutoff

	// As long as separator (<,>) is not found and we haven't processed all matches
	for (CurrentMatch=0; !Found && CurrentMatch < Matches.size(); ) {
	  Position = (*CurrentStringCondition).find(Matches[CurrentMatch],0);
	  if (Position <= (*CurrentStringCondition).size()) {
		Found = true;
	  } else {
		CurrentMatch++;
	  }
	}

	// Did we find an operator?
	char* StringDummy = (char*)(*CurrentStringCondition).c_str();
	if (Found) {

	  if (CurrentMatch==0) {
		Operators.push_back(LESS);
	  } else {
		Operators.push_back(GREATER);
	  }

	  // Cut up feature and cutoff
	  MyToken = str_parse(&StringDummy,&Matches[CurrentMatch]);
	  for (; MyToken; Part++) {
		switch (Part) {
		  case 0:                                                               // Feature
			if (MyToken) {
			  Features.push_back(atoi(MyToken)-1);
			} else {
			  return false;
			}
			break;
		  case 1:                                                               // Cutoff
			if (MyToken) {
			  Cutoffs.push_back(atof(MyToken));
			} else {
			  Cutoffs.push_back(NULL);
			}
			break;
		  default:
			return false;                                                       // Too many operators in this condition
		}
		MyToken = str_parse(&StringDummy,&Matches[CurrentMatch]);
	  }
	} else {                                                                    // No operator found in condition
	  if (atoi(StringDummy)==0) {                                               // Valid feature?
		return false;
	  } else {
		Features.push_back(atoi(StringDummy)-1);                                // Save feature
		Cutoffs.push_back(NULL);                                                // Empty cutoff
		Operators.push_back(NONE);                                              // Empty operator
	  }
	}
  }
  */
  return true;
}

/**********************************************************************
Function: InterpretCombinations()
Category: Modifiers
Scope: public
In: string, String representing a rule
Out: bool, rule could/could not be interpreted
Description: Interprets a string representing a (number of) combinations.
**********************************************************************/
bool CANDIDATE::InterpretCombinations(string StringCombinations) {
  char Separators[] = " ";
  char* MyToken;
  char Buffer[MAX_LINESIZE];

  int CurrentCombination=0, LastCombination=0;
  char* StringDummy = (char*)StringCombinations.c_str();

  MyToken = str_parse(&StringDummy,Separators);
  while (MyToken) {
	CurrentCombination = atoi(MyToken);                                         // Make sure CurrentCombination is equal or less than LastCombination if LastCombination does not equal 0
	if (CurrentCombination != 0 && (CurrentCombination <= LastCombination || LastCombination==0)) {
	  Conjunctions.push_back(CurrentCombination);
	} else {
	  return false;
	}
	MyToken = str_parse(&StringDummy,Separators);
  }

  return true;
}

bool CANDIDATE::operator<(const CANDIDATE& SourceCandidate) {
  if (Features.size() < SourceCandidate.Features.size()) {
	return true;
  } else if (Features.size() > SourceCandidate.Features.size()) {
    cout << "Rulelength: (" << Features.size() << ") > (" << SourceCandidate.Features.size() << ")" << endl;
    return false;
  } else {
    // Compare Conjunction-sizes
    unsigned int NoConjunctions = Conjunctions.size();
    for (unsigned int i=0; i < NoConjunctions; i++) {
      if (Conjunctions[i] > SourceCandidate.Conjunctions[i]) {
        return true;
      } else if (Conjunctions[i] < SourceCandidate.Conjunctions[i]) {
        cout << "Conjunction-size: (" << Conjunctions[i] << ") > (" << SourceCandidate.Conjunctions[i] << ")" << endl;
        return false;
      } else {
        // Conjunction-sizes are the same -> compare Features
        unsigned int StartFeature = 0;                                          // Starting Feature for current Conjunction
        unsigned int EndFeature = 0;                                            // Ending Feature for current Conjunction
        for (unsigned int j=0; j < i; j++) {
          StartFeature += Conjunctions[j];
        }
        EndFeature = StartFeature + Conjunctions[i];
        for (unsigned int k=StartFeature; k < EndFeature; k++) {                // Iterate through Features of current Conjunction
          if (Features[k] < SourceCandidate.Features[k]) {
            return true;
          } else if (Features[k] > SourceCandidate.Features[k]) {
            cout << "Feature: (" << Features[k]+1 << ") > (" << SourceCandidate.Features[k]+1 << ")" << endl;
            return false;
          } else {
            // Features are the same -> compare Operators
            if (Operators[k] < SourceCandidate.Operators[k]) {
              return true;
            } else if (Operators[k] > SourceCandidate.Operators[k]) {
              cout << "Operator: (" << Operators[k] << ") > (" << SourceCandidate.Operators[k] << ")" << endl;
              return false;
            }
            // Feature and Operator are the same, go to next Feature
          }
        }
        // Features, Operators, Conjunction-size are the same, go to next Conjunction
      }
    }
  }
  return false;
}
