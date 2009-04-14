// testSuite.cc -- Apr 13, 2009
//    by geohot

#include <iostream>
#include "../include/macros.h"
using namespace std;

int ParsedInstruction_test();
int File_test();
int InstructionARM_test();

int main()
{
  /*cout << "running tests.          " << endl;
  cout << "ParsedInstruction test: "; ParsedInstruction_test();
  cout << "File test:              "; File_test();
  cout << "debug macro test:       "; debug << "test" << endl;*/
  cout << "InstructionARM test:    "; InstructionARM_test();
  return 0;
}
