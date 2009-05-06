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

#define FunctionIterator std::map<Address,Function>::iterator

class Memory {
public:
  //default constructor
  File& operator[](Address address);
  bool allocate(Address, int);
  void debugPrint();
  bool loadFile(const char *, Address); //allocate the memory first
  void consoleDump(Address address, int len, int);
  bool exists(Address);
  std::string getName(Address address);
  bool isNameSet(Address address);
  void setName(Address address, std::string name);
  bool lookupName(std::string name, Address *address);
  bool importIDC(const char *);
  std::vector<File>* getChunk(Address);
  Function* inFunction(Address addr);
  Function* addFunction(int start);
  std::map<Address, Function> mFunctionStore;
  std::map<Address, std::string> mNames;
private:
  //allocated memory goes here, since memory is only on one interval, bsearch=interval tree
  std::map<int, std::vector<File> > mChunks;
  //memory that isn't specifically allocated ends up here
  std::map<Data, File> mMemoryUndefined;
  std::map<std::string, Address> mReverseNames; //memory/speed tradeoff
  bool checkRegion(Data address, int len);
  static int fileSize(FILE *f);
};

}

#endif /* EDA_MEMORY_H_ */
