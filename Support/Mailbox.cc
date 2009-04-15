// Mailbox.cc -- Apr 15, 2009
//    by geohot
//  released under GPLv3, see http://gplv3.fsf.org/

#include "Mailbox.h"
#include <iostream>
#include "macros.h"

using namespace eda;

Mailbox::Mailbox()
{
  InitializeCriticalSection(&mMutex);
  mOMGMail=CreateEvent(0,true,false,0);
}

Mailbox::~Mailbox()
{
  DeleteCriticalSection(&mMutex);
  CloseHandle(mOMGMail);
}

//call from anywhere
void Mailbox::sendMail(Mail themail)
{
  EnterCriticalSection(&mMutex);
  mPostOffice.push(themail);
  SetEvent(mOMGMail);   //you've got mail
  LeaveCriticalSection(&mMutex);
}

Mail Mailbox::checkMailbox()
{
  if(WaitForSingleObject(mOMGMail, 0)==0)       //OMGMail is yes
  {
    EnterCriticalSection(&mMutex);
    Mail ret=mPostOffice.front();
    //info << "got mail: " << ret.mCommand << std::endl;
    mPostOffice.pop();
    if(mPostOffice.size()==0) ResetEvent(mOMGMail);
    LeaveCriticalSection(&mMutex);
    return ret;
  }
  return Mail(0,0);        //ugliness
}

Mail Mailbox::waitForMail()
{
  WaitForSingleObject(mOMGMail, INFINITE);
  return checkMailbox();
}
