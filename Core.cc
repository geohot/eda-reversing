#include <iostream>
#include "Core.h"
using namespace std;

namespace eda {

Core::Core(Edb* edb, Server* server): mEdb(edb), mServer(server) {
}

Core::~Core() {
}

void Core::update() {
}

void Core::test() {
  // From testSuite.cc
  cout << "running tests.          " << endl;
  cout << "ParsedInstruction test: "; //ParsedInstruction_test();
  cout << "File test:              "; //File_test();
  cout << "debug macro test:       "; //debug << "test" << endl;
  cout << "InstructionARM test:    "; //InstructionARM_test();
  return;// 0;
}

}
