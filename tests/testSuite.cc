// testSuite.cc -- Apr 13, 2009
//    by geohot
//  released under GPLv3, see http://gplv3.fsf.org/

//ideally this should be a very complete,
//  nice testsuite so people don't break things
//  come on people, get coding

#include <iostream>
#include "../include/macros.h"
using namespace std;

int ParsedInstruction_test();
int File_test();
int InstructionARM_test();
int Memory_test();

int main()
{
/*cout << "File test:              "; File_test();
  cout << "ParsedInstruction test: "; ParsedInstruction_test();
  cout << "debug macro test:       "; debug << "test" << endl;
  cout << "InstructionARM test:    "; InstructionARM_test();*/
  cout << "Memory test:            "; Memory_test();
  return 0;
}
