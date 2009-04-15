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
#include <string>
#include <vector>
#include <sstream>
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

int ConsoleFrontEnd::strtoint(string in)
{
  int ret;
  if(in.substr(0,2)=="0x")
  {
    stringstream ss(in.substr(2));
    ss >> std::hex >> ret;
  }
  else
  {
    stringstream ss(in);
    ss >> std::dec >> ret;
  }
  return ret;
}

bool ConsoleFrontEnd::lexer(string cmd)
{
  vector<string> argv;
  size_t start=cmd.find_first_not_of(" ",0);
  size_t end=cmd.find_first_of(" ", start);
  while(end!=string::npos || start!=string::npos)
  {
    argv.push_back(cmd.substr(start,end-start));
    start=cmd.find_first_not_of(" ",end);
    end=cmd.find_first_of(" ", start);
  }
  //info << "lexed " << argv.size() << endl;
//parsing commands here
  if(argv[0]=="loadfile" && argv.size()==3)
  {
    mBank->lock();
    mBank->mMem.loadFile(argv[1].c_str(),strtoint(argv[2]));
    mBank->unlock();
  }
  else if(argv[0]=="md" && argv.size()==4)
  {
    mBank->lock();
    mBank->mMem.consoleDump(strtoint(argv[1]),strtoint(argv[2]), strtoint(argv[3]));
    mBank->unlock();
  }
  else if(argv[0]=="mw" && argv.size()==4) {
    //mw <addr> <data> <clnum>
    mBank->lock();
    mBank->mMem[strtoint(argv[1])].set(strtoint(argv[3]), strtoint(argv[2]));
    mBank->unlock();
  }
  else if(argv[0]=="segments")
  {
    mBank->lock();
    mBank->mMem.debugPrint();
    mBank->unlock();
  }
  else
  {
    //cout << "syntax error" << endl;
    return false;
  }

  return true;
}

void ConsoleFrontEnd::runLoop()
{
  info << "CFE is running" << endl;
  string cmd;
  lexer("loadfile bootrom 0x400000");
  lexer("loadfile Memory_data 0x0");
  while(1)      //tons of mail
  {
    //cout << "EDA> ";  //stupid threads ruin my prompt
    getline(cin,cmd);
    if(!lexer(cmd))     //couldn't be handled locally, send to core
      mCore->sendMail(Mail(MAIL_SERVER,cmd));
  }
}

}
