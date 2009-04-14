// File_test.cc -- Apr 12, 2009
//    by geohot
//  released under GPLv3, see http://gplv3.fsf.org/

#include <iostream>
#include <cassert>
#include "../Controller/ARM/InstructionARM.h"

void InstructionARM_debugPrint(eda::InstructionARM *test)
{
  test->mString.consolePrint();
  test->mString.debugPrint();
  test->mAction.debugPrint();
}

int InstructionARM_test()
{
  eda::InstructionARM test(0xE3A00000);
  InstructionARM_debugPrint(&test);

  eda::InstructionARM test2(0xE8BD8010);
  InstructionARM_debugPrint(&test2);

  return 0;
}
