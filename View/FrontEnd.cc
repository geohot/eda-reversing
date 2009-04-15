// FrontEnd.cc -- Apr 15, 2009
//    by geohot
//  released under GPLv3, see http://gplv3.fsf.org/

#include "FrontEnd.h"
#include "macros.h"
#include "bank.h"

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
using namespace std;

namespace eda
{

FrontEnd::FrontEnd()
{
  debug << "wheres the bank?" << std::endl;
}

FrontEnd::FrontEnd(Bank *bank): mBank(bank)
{
  info << "creating thread" << endl;
  threadCreate(&mThread, &eda::FrontEnd::entryPoint, this);
}

//attach to the Core's mailbox
void FrontEnd::attach(Core *a)
{
  mCore=&a->mMail;
}

FrontEnd::~FrontEnd()
{
  info << "destroying thread" << endl;
  threadDestroy(&mThread);
}

void FrontEnd::entryPoint(void *param)
{
  FrontEnd *ptr=(FrontEnd *)param;
  ptr->runLoop();
}

void FrontEnd::runLoop()
{
  while(1) debug << "i am a boring FrontEnd" << std::endl;
}

}
