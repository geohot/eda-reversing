// Bank.h -- Apr 13, 2009
//    by posixninja, geohot
//  released under GPLv3, see http://gplv3.fsf.org/

#ifndef EDA_BANK_H_
#define EDA_BANK_H_
#include "threading.h"

#include <vector>
#include "Memory.h"
#include "RegisterFile.h"
#include "Instruction.h"

#define LOCKED_NONE 0
#define LOCKED_CORE 1
#define LOCKED_SERVER 2
#define LOCKED_WEBSERVER 3

namespace eda {

#define InstructionIterator std::map<Data,Instruction>::iterator

class Bank {
public:
  Bank();
  virtual ~Bank();
  void lock(int locker);
  void unlock(int locker);
  void waitForAction(); //wait for lock and unlock
  Memory* mem();
  std::vector<RegisterFile>::iterator currentRegisterFile;
  std::map<Data, Instruction> mInstructionCache;
private:
  std::vector<RegisterFile> RegisterFiles;
  mutexContainer mBankMutex;
  int mLocker, mLockCount;
  Memory mMem;
};

}

#endif /* EDA_BANK_H_ */
