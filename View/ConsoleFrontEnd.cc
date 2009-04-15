// ConsoleFrontEnd.cc -- Apr 14, 2009
//    by geohot
//  released under GPLv3, see http://gplv3.fsf.org/

//this gets passed a pointer to the model, read only
//and a pointer to the core mailbox and the model mailbox

#include "Mailbox.h"
#include "ConsoleFrontEnd.h"
#include "macros.h"
#include "bank.h"

#include <iostream>
using namespace std;

namespace eda
{

ConsoleFrontEnd::ConsoleFrontEnd(Bank *bank): mBank(bank)
{
  info << "creating thread" << endl;
  threadCreate(&mThread, &eda::ConsoleFrontEnd::entryPoint, this);
}

//attach to the Core's mailbox
void ConsoleFrontEnd::attach(Core *a)
{
  mCore=&a->mMail;
}

ConsoleFrontEnd::~ConsoleFrontEnd()
{
  info << "destroying thread" << endl;
  threadDestroy(&mThread);
}

void ConsoleFrontEnd::entryPoint(void *param)
{
  ConsoleFrontEnd *ptr=(ConsoleFrontEnd *)param;
  ptr->runLoop();
}

void ConsoleFrontEnd::runLoop()
{
  cout << "CFE is running" << endl;
  int a;
  while(1)      //tons of mail
  {
    cin >> a;
    mCore->sendMail(Mail(a,0));
  }
}

}
