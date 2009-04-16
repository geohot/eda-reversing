// FunctionCache.h -- Apr 15, 2009
//    by geohot
//  released under GPLv3, see http://gplv3.fsf.org/

#ifndef FUNCTIONCACHE_H_
#define FUNCTIONCACHE_H_

#include "Function.h"
#include <map>
#include <iostream>
#include <sstream>

namespace eda {

#define FunctionIterator std::map<Data,Function>::iterator

class FunctionCache
{
public:
  Function *add(int start);
  //add rename
  Function *inFunction(Data addr);
  void debugPrint();
  std::map<Data,Function> mStore;
};

}

#endif /* FUNCTIONCACHE_H_ */
