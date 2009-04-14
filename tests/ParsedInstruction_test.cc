// ParsedInstruction_test.cc -- Apr 13, 2009
//    by geohot

#include "../Model/ParsedInstruction.h"

using namespace std;

int ParsedInstruction_test()
{
  eda::ParsedInstruction test;
  test.add("LDR",DT_OPCODE);
  test.add("EQ",DT_CONDITION);
  test.add(" ",DT_FORMATTING);
  test.add("R1",DT_REGISTER);
  test.add(", [",DT_FORMATTING);
  test.add("R2",DT_REGISTER);
  test.add("]",DT_FORMATTING);

  test.consolePrint();

  return 0;
}
