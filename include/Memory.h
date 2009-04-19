// Memory.h -- Apr 13, 2009
//    by geohot
//  released under GPLv3, see http://gplv3.fsf.org/

#ifndef EDA_MEMORY_H_
#define EDA_MEMORY_H_

#include "macros.h"
#include <vector>
#include <map>
#include <string>
#include "File.h"
#include "Function.h"

//not actually implementing this now, just the interface, so I can do ChangeList
//actually I'll implement really dumbly in this header for now, really needs pagetableish things

namespace eda {

#define FunctionIterator std::map<Data,Function>::iterator

class Memory
{
public:
  //default constructor
  File& operator[](Data address);
  bool allocate(Data, int);
  void debugPrint();
  bool loadFile(const char *, Data);        //allocate the memory first
  void consoleDump(Data address, int len, int);
  bool exists(Data);
  std::string getName(Data address);
  bool isNameSet(Data address);
  void setName(Data address, std::string name);
  bool lookupName(std::string name, Data *address);
  bool importIDC(const char *);
  std::vector<File>* getChunk(Data);
  Function* inFunction(Data addr);
  Function* addFunction(int start);
  std::map<Data,Function> mFunctionStore;
private:
//allocated memory goes here, since memory is only on one interval, bsearch=interval tree
  std::map<int, std::vector<File> > mChunks;
//memory that isn't specifically allocated ends up here
  std::map<Data,File> mMemoryUndefined;
  std::map<Data,std::string> mNames;
  std::map<std::string, Data> mReverseNames;            //memory/speed tradeoff
  bool checkRegion(Data address, int len);
  static int fileSize(FILE *f);
};

}

#endif /* EDA_MEMORY_H_ */
