//----------------------------------------------------------------------------//
// $Header: //depot/Explore/Explore/outbuf.h#7 $
// $Author: Pepe $
// $DateTime: 2004/10/11 08:11:59 $
// $Change: 100 $
//----------------------------------------------------------------------------//

#include <iostream>
#include <streambuf>
#include <locale>
#include <cstdio>

class outbuf : public streambuf {
  protected:
  	virtual int_type overflow (int_type c) {
	    if (c != EOF) {
    		if (putchar(c) == EOF) {
    		    return EOF;
    		}
	    }
	    return c;
  	}
};

