#include "eda_file.h"

using namespace eda;

void file::set(int cl_num, u32 data)
{
  db.insert(make_pair(cl_num, data));
}

//return biggest cl_num entry <= passed cl_num
u32& file::operator[] (int cl_num)
{
  map<int, u32>
  if(upper_bound

}
