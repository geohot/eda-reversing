// File.h -- Apr 12, 2009
//    by geohot

#ifndef FILE_H_
#define FILE_H_

#include "../edaMacros.h"
#include <map>

namespace eda {

class File
{
public:
  u32& operator[](int changelistNumber);
  void set(int changelistNumber, Data data);
private:
  std::map<int, Data> mStore;
};

}

#endif /* FILE_H_ */
