// Memory.h -- Apr 13, 2009
//    by geohot
//  released under GPLv3, see http://gplv3.fsf.org/

#ifndef EDA_MEMORY_H_
#define EDA_MEMORY_H_

#include "macros.h"
#include <vector>
#include <map>
#include "File.h"

//not actually implementing this now, just the interface, so I can do ChangeList
//actually I'll implement really dumbly in this header for now, really needs pagetableish things

namespace eda {

class Memory
{
public:
  File& operator[](Data address);
  bool allocate(Data, int);
  void debugPrint();
  bool loadFile(const char *, Data);        //allocate the memory first
  void consoleDump(Data address, int len, int);
  std::vector<File>* getChunk(Data);
private:
//allocated memory goes here, since memory is only on one interval, bsearch=interval tree
  std::map<int, std::vector<File> > mChunks;
//memory that isn't specifically allocated ends up here
  std::map<Data,File> mMemoryUndefined;
  bool checkRegion(Data address, int len);
  static int fileSize(FILE *f);
};

}

#endif /* EDA_MEMORY_H_ */
