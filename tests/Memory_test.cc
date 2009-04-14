// Memory_test.cc -- Apr 14, 2009
//    by geohot
//  released under GPLv3, see http://gplv3.fsf.org/

#include "../Model/Memory.h"
#include <iostream>
#include <cassert>

using namespace std;

//something has got to be done about this
const char filename[]="z:\\EDA\\eda-reversing\\tests\\data\\Memory_data";

int Memory_test()
{
  cout << endl;
  eda::Memory test;
  assert(test.loadFile(filename, 0x80000));
  test.debugPrint();
  test.consoleDump(0x80000,0x40,1);
  return 0;
}
