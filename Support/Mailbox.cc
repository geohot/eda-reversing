// Mailbox.cc -- Apr 15, 2009
//    by geohot
//  released under GPLv3, see http://gplv3.fsf.org/

#include "Mailbox.h"
#include <iostream>
#include "macros.h"

using namespace eda;

Mailbox::Mailbox()
{
  mutexInit(&mMutex);
  eventInit(&mOMGMail);
}

Mailbox::~Mailbox()
{
  mutexDestroy(&mMutex);
  eventDestroy(&mOMGMail);
}

//call from anywhere
void Mailbox::sendMail(Mail themail)
{
  mutexLock(&mMutex);
  mPostOffice.push(themail);
  eventSet(&mOMGMail);   //you've got mail
  mutexUnlock(&mMutex);
}

Mail Mailbox::checkMailbox()
{
  Mail ret;
  mutexLock(&mMutex);
  if(mPostOffice.size()==0) ret=Mail(0,0);
  else
  {
    ret=mPostOffice.front();
    mPostOffice.pop();
    if(mPostOffice.size()==0) eventReset(&mOMGMail);
  }
  mutexUnlock(&mMutex);
  return ret;
}

Mail Mailbox::waitForMail()
{
  eventWaitFor(&mOMGMail);
  return checkMailbox();
}
