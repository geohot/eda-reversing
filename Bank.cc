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
