// File.cc -- Apr 12, 2009
//    by geohot
//  released under GPLv3, see http://gplv3.fsf.org/

#include "File.h"

using namespace eda;

File::File()
//initial value of 0 sounds good
{
  mStore.insert(std::make_pair(0, (Data) 0));
}

void File::set(int changelistNumber, Data data) {
  //mStore.insert(std::make_pair(changelistNumber, data));
  mStore[changelistNumber] = data; //may not be the most efficent
}

//return biggest cl_num entry <= passed cl_num
Data& File::operator[](int cl_num) {
  return (--mStore.upper_bound(cl_num))->second;
}
