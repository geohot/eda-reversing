// Memory.h -- Apr 13, 2009
//    by geohot

#ifndef MEMORY_H_
#define MEMORY_H_

#include "edaMacros.h"

//not actually implementing this now, just the interface, so I can do ChangeList

class Memory
{
public:
  File& operator[](Data address);
};

#endif /* MEMORY_H_ */
