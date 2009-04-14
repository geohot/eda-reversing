// Memory.h -- Apr 13, 2009
//    by geohot
//  released under GPLv3, see http://gplv3.fsf.org/

#ifndef EDA_MEMORY_H_
#define EDA_MEMORY_H_

#include "../include/macros.h"

//not actually implementing this now, just the interface, so I can do ChangeList
//actually I'll implement really dumbly in this header for now, really needs pagetableish things

namespace eda {

class Memory
{
public:
  File& operator[](Data address)
  {
    return mMemorySpace[address];
  }
private:
  std::map<Data,File> mMemorySpace;
};

}

#endif /* EDA_MEMORY_H_ */
