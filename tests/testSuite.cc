// testSuite.cc -- Apr 13, 2009
//    by geohot

#include <iostream>
#include <iomanip>
using namespace std;

int ParsedInstruction_test();
int File_test();

int main()
{
  cout << "running tests.          " << endl;
  cout << "ParsedInstruction test: "; ParsedInstruction_test();
  cout << "File test:              "; File_test();
  return 0;
}
