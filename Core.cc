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

Core::Core() {
  info << "where's the Bank at" << endl;
}

Core::Core(Bank* bank) :
  mBank(bank) {

}

Core::~Core() {
}

InstructionIterator Core::disassemble(Data addr) {
  info << "I am a generic Core, I can't disassemble" << endl;
  return InstructionIterator();
}

void Core::fastAnalyse(Data addr, Function *fxn, bool crap) {
  info << "I am a generic Core, I can't fastAnalyse" << endl;
  return;
}

void Core::update() {
}

void Core::runLoop()
//this is where the Core actually works
//it is event driven completely
{
  while (1) {
    Mail event = mMail.waitForMail();
    if (event.mCommand == CORE_DISASSEMBLE) {
      //cout << "disassembling " << event.mParam << endl;
      disassemble((Data) event.mParam);
      //lexer(event);
    } else if (event.mCommand == CORE_ANALYSE) {
      fastAnalyse((Data) event.mParam, mBank->mem()->addFunction(
          (Data) event.mParam), false);
    }
    //info << "i've got mail: " << event.mParam << endl;
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
