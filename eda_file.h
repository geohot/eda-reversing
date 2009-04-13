#ifndef __EDA_FILE_H
#define __EDA_FILE_H

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

#endif
