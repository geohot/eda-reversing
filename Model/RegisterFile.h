// RegisterFile.h -- Apr 13, 2009
//    by geohot

#ifndef REGISTERFILE_H_
#define REGISTERFILE_H_

#include "File.h"

namespace eda {

class RegisterFile
{
public:
  RegisterFile(int size);
  File& operator[](int RegisterNumber);
private:
  std::vector<File> mRegisterStore;
};

}

#endif /* REGISTERFILE_H_ */
