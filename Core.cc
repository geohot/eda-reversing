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

Core::Core(Bank* bank): mBank(bank) {

}

Core::~Core() {
}

void Core::update() {
}

void Core::runLoop()
//this is where the Core actually works
//it is event driven completely
{
  while(1)
  {
    Mail yay=mMail.waitForMail();
    info << "i've got mail: " << yay.mCommand << endl;
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
