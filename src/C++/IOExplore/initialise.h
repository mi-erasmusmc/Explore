//----------------------------------------------------------------------------//
// $Header: //depot/Explore/IOExplore/initialise.h#6 $
// $Author: Pepe $
// $DateTime: 2004/10/09 23:17:14 $
// $Change: 99 $
//----------------------------------------------------------------------------//

#ifdef EXPLORE_IO_INIT
#define EXPLORE_IO_INIT

enum EXPLORETYPES { DATA, PROJECT, RESULT };

struct ExploreFormat {
  enum TYPES ( DATA, PROJECT, RESULT);
  enum EXTENSIONS ( '.data', '.project', '.result');

  GetType(string Filename);
}

#endif EXPLORE_IO_INIT
 