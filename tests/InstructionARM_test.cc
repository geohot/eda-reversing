// File_test.cc -- Apr 12, 2009
//    by geohot
//  released under GPLv3, see http://gplv3.fsf.org/

#include <iostream>
#include <cassert>
#include "../Controller/ARM/InstructionARM.h"

int InstructionARM_test()
{
  eda::InstructionARM test(0xE3A00000);
  test.mString.consolePrint();
  test.mString.debugPrint();
  test.mAction.debugPrint();
  return 0;
}
