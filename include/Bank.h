// Bank.h -- Apr 13, 2009
//    by posixninja
//  released under GPLv3, see http://gplv3.fsf.org/

#ifndef EDA_BANK_H_
#define EDA_BANK_H_
#include "threading.h"

#include "Observable.h"

#include <vector>
#include "Memory.h"
#include "RegisterFile.h"

namespace eda {

class Bank: public Observable {
public:
  Bank();
  virtual ~Bank();
  void lock();
  void unlock();
private:
  std::vector<RegisterFile> RegisterFiles;
  std::vector<RegisterFile>::iterator currentRegisterFile;
  Memory mMem;
  mutexContainer mBankMutex;
};

}

#endif /* EDA_BANK_H_ */
