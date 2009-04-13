// RegisterFile.h -- Apr 13, 2009
//    by geohot

#ifndef EDA_REGISTERFILE_H_
#define EDA_REGISTERFILE_H_

#include <vector>
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

#endif /* EDA_REGISTERFILE_H_ */
