// RegisterFile.cc -- Apr 13, 2009
//    by geohot
//  released under GPLv3, see http://gplv3.fsf.org/

#include "RegisterFile.h"

using namespace eda;

RegisterFile::RegisterFile(int size)
{
  mRegisterStore.resize(size);
}

File& RegisterFile::operator[](int registerNumber)
{
  return mRegisterStore[registerNumber];
}
