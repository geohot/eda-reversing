#ifndef __EDA_FILE_H
#define __EDA_FILE_H

#include <map>

#define u32 unsigned int

class eda::file
{
public:
  u32& operator[](int cl_num);
  void set(int cl_num, u32 data);
private:
  map<int, u32> db;
}

#endif
