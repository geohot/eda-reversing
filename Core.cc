// Core.cc -- Apr 14, 2009
//    by posixninja, geohot
//  released under GPLv3, see http://gplv3.fsf.org/

#include "Mailbox.h"
#include <iostream>
#include "Core.h"
#include "macros.h"
using namespace std;

int ParsedInstruction_test();
int File_test();
int InstructionARM_test();
int Memory_test();

namespace eda {

Core::Core()
{
  info << "where's the Bank at" << endl;
}

Core::Core(Bank* bank): mBank(bank) {

}

Core::~Core() {
}

void Core::disassemble(int addr)
{
  info << "I am a generic Core, I can't disassemble" << endl;
}

void Core::update() {
}

/*bool Core::lexer(Mail event)
{
  vector<string> argv;
  string cmd=event.mParam;
  size_t start=cmd.find_first_not_of(" ",0);
  size_t end=cmd.find_first_of(" ", start);
  while(end!=string::npos || start!=string::npos)
  {
    argv.push_back(cmd.substr(start,end-start));
    start=cmd.find_first_not_of(" ",end);
    end=cmd.find_first_of(" ", start);
  }
  //if(argv[0])

}*/

void Core::runLoop()
//this is where the Core actually works
//it is event driven completely
{
  while(1)
  {
    Mail event=mMail.waitForMail();
    if(event.mType==MAIL_SERVER)
      //lexer(event);
    info << "i've got mail: " << event.mParam << endl;
  }
}

void Core::test() {
  // From testSuite.cc
  /*cout << "running tests.          " << endl;
  cout << "ParsedInstruction test: "; ParsedInstruction_test();
  cout << "File test:              "; File_test();
  cout << "debug macro test:       "; info << "test" << endl;
  cout << "InstructionARM test:    "; InstructionARM_test();
  cout << "Memory test:            "; Memory_test();*/
  /*Mailbox test;
  ConsoleFrontEnd a(&test,0);*/

  return;// 0;
}

}
