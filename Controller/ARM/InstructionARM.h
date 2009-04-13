// InstructionARM.h -- Apr 13, 2009
//    by geohot

#ifndef EDA_INSTRUCTIONARM_H_
#define EDA_INSTRUCTIONARM_H_

#include "../Instruction.h"
#include "../../edaMacros.h"

namespace eda {

class InstructionARM : public Instruction
{
public:
  InstructionARM(Data opcode);  //disassembler
private:
  //will probably have private functions
};

}

#endif /* EDA_INSTRUCTIONARM_H_ */
