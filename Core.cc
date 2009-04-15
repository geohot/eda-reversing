#include "ConsoleFrontEnd.h"

#include <iostream>
#include "Core.h"
#include "macros.h"
using namespace std;

int ParsedInstruction_test();
int File_test();
int InstructionARM_test();
int Memory_test();

namespace eda {

Core::Core(Edb* edb, Server* server): mEdb(edb), mServer(server) {
}

Core::~Core() {
}

void Core::update() {
}

void Core::test() {
  // From testSuite.cc
  /*cout << "running tests.          " << endl;
  cout << "ParsedInstruction test: "; ParsedInstruction_test();
  cout << "File test:              "; File_test();
  cout << "debug macro test:       "; info << "test" << endl;
  cout << "InstructionARM test:    "; InstructionARM_test();
  cout << "Memory test:            "; Memory_test();*/
  Mailbox test;
  ConsoleFrontEnd a(&test,0);
  while(1)
  {
    Mail yay=test.waitForMail();
    cout << "i've got mail: " << yay.mCommand << endl;
  }
  return;// 0;
}

}
