// ConsoleFrontEnd.cc -- Apr 14, 2009
//    by geohot
//  released under GPLv3, see http://gplv3.fsf.org/

//this gets passed a pointer to the model, read only
//and a pointer to the core mailbox and the model mailbox

#include "Mailbox.h"
#include "ConsoleFrontEnd.h"
#include "macros.h"

#include <iostream>
using namespace std;

namespace eda
{

ConsoleFrontEnd::ConsoleFrontEnd(Mailbox *core, Mailbox *edb)
{
  info << "creating thread" << endl;
  threadCreate(&mThread, &eda::ConsoleFrontEnd::entryPoint, this);
  mCore=core;   //mail to the core
  mEdb=edb;     //mail to the model
}

ConsoleFrontEnd::~ConsoleFrontEnd()
{
  info << "destroying thread" << endl;
  threadDestroy(&mThread);
}

void ConsoleFrontEnd::chill()
{
  threadWait(&mThread);
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
