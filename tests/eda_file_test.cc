#include <iostream>
#include <cassert>
#include "../eda_file.h"

using namespace std;

int main()
{
  cout << "testing the File class" << endl;
  eda::File test;

  test.set(0, 0x20);
  test.set(4, 0x50);
  test.set(8, 0x104);

  assert(test[4]==0x50);
  assert(test[7]==0x50);
  assert(test[10]==0x104);
  assert(test[1]==0x20);

  cout << "all tests pass" << endl;
  
}
