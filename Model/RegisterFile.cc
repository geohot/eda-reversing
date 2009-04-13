// RegisterFile.cc -- Apr 13, 2009
//    by geohot

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
