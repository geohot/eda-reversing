// Bank.cc -- Apr 14, 2009
//    by posixninja, geohot
//  released under GPLv3, see http://gplv3.fsf.org/

#include "Bank.h"
#include "macros.h"

namespace eda {

Bank::Bank() {
  RegisterFiles.push_back(RegisterFile(32));    //32 is size;
  currentRegisterFile=RegisterFiles.begin();
  mutexInit(&mBankMutex);
  mLocker=LOCKED_NONE;
  mLockCount=0;
}

Bank::~Bank() {


}

Memory* Bank::mem()
{
  //should check to make sure it's locked
  return &mMem;
}

void Bank::lock(int locker)
{
  if(locker==mLocker) { mLockCount++; return; }   //already locked by this person
  mutexLock(&mBankMutex);       //should wait
  mLocker=locker;
  mLockCount++;
}

void Bank::unlock(int locker)
{
  if(locker!=mLocker) { debug << "error, you don't own this" << std::endl; }
  mLockCount--;
  if(mLockCount==0)
  {
    mLocker=LOCKED_NONE;
    mutexUnlock(&mBankMutex);
  }
  else if(mLockCount<0) { debug << "you cheated with the mutex" << std::endl; }
}

}
