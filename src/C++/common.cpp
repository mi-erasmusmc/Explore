//----------------------------------------------------------------------------//
// $Header: //depot/Explore/common.cpp#6 $
// $Author: Rijnbeek $
// $DateTime: 2008/09/09 14:16:02 $
// $Change: 87 $
//----------------------------------------------------------------------------//

#include "common.h"

/**********************************************************************
Function: str_parse()
Category: Selectors
Scope: public
In: char**, string to be tokenized
    char*, delimiters
Out: -
Description: Universal tokenizer function for importing data files.
**********************************************************************/
char *str_parse(char **str, char *delimiters) {
  char *head, *tail, *rv;

  //remove heading spaces or tabs
  while ((**str!='\0')&& ((**str==' ') || (**str=='\t'))){
   (*str)++;
  }

  if (str==NULL || *str==NULL || **str=='\0') {
    rv = NULL;
  } else if (delimiters==NULL || *delimiters=='\0' ) {
    rv = NULL;
  } else {
    rv = head = *str;
    if ((tail = strpbrk(head,delimiters)) == NULL) {
      *str = head + strlen( head );
    } else {
      *tail = '\0';
      *str = tail + 1;
    }
  }
  return rv;
}

/**********************************************************************
Function: PrintOperator()
Category: Selectors
Scope: public
In: OPERATOR, operator to be printed
Out: string, text output
Description: Prints the enumerator Operator as a string.
**********************************************************************/
string PrintOperator(OPERATOR Operator) {
  ostringstream Result;

  switch (Operator) {
    case (GREATER):
      Result << ">";
      break;
    case (LESS):
      Result << "<=";
      break;
    case (GREATERLESS):
      Result << "<=>";
      break;
	case (EQUAL):
	  Result << "=";
	  break;
  }
  
  return Result.str();
}
