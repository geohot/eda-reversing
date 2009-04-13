// File.h -- Apr 12, 2009
//    by geohot

#ifndef FILE_H_
#define FILE_H_

#include <map>

#define u32 unsigned int

namespace eda {

class File
{
public:
  u32& operator[](int changelistNumber);
  void set(int changelistNumber, u32 data);
private:
  std::map<int, u32> mStore;
};

}

#endif /* FILE_H_ */
