// FunctionCache.cc -- Apr 15, 2009
//    by geohot
//  released under GPLv3, see http://gplv3.fsf.org/

#include "FunctionCache.h"

using namespace eda;


void FunctionCache::add(int start)
{
  std::stringstream name;
  name << std::hex << "sub_" << start;
  mStore.insert(std::make_pair(start,Function(start, name.str())));
}

Function* FunctionCache::inFunction(Data addr)
{
  //return function with this instruction in it
  //doesn't work yet
  FunctionIterator a=mStore.find(addr);
  if(a!=mStore.end())
    return &(a->second);
  else
    return 0;
}

void FunctionCache::debugPrint()
{
  FunctionIterator a=mStore.begin();
  while(a!=mStore.end()) {
    std::cout << a->first << ": " << a->second.mName << std::endl;
    ++a;
  }
}
