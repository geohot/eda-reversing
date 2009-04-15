// Bank.cc -- Apr 14, 2009
//    by posixninja, geohot
//  released under GPLv3, see http://gplv3.fsf.org/

#include "Bank.h"

namespace eda {

Bank::Bank() {
  RegisterFiles.push_back(RegisterFile(32));    //32 is size;
  currentRegisterFile=RegisterFiles.begin();
  mutexInit(&mBankMutex);
}

Bank::~Bank() {


}

void Bank::lock()
{
  mutexLock(&mBankMutex);
}

void Bank::unlock()
{
  mutexUnlock(&mBankMutex);
}

}
