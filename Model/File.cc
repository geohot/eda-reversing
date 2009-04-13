// File.cc -- Apr 12, 2009
//    by geohot

#include "File.h"

using namespace eda;

void File::set(int changelistNumber, Data data)
{
  mStore.insert(std::make_pair(changelistNumber, data));
}

//return biggest cl_num entry <= passed cl_num
u32& File::operator[] (int cl_num)
{
  return (--mStore.upper_bound(cl_num))->second;
}
