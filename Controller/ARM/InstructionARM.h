// InstructionARM.h -- Apr 13, 2009
//    by geohot
//  released under GPLv3, see http://gplv3.fsf.org/

//not moved into include on purpose

#ifndef EDA_INSTRUCTIONARM_H_
#define EDA_INSTRUCTIONARM_H_

#include "Instruction.h"
#include "macros.h"

#include "InstructionARM.h"

namespace eda {

class InstructionARM : public Instruction
{
public:
  InstructionARM(Data opcode);  //disassembler
private:
  bool init();
  bool initDataProcessing();
  bool initLoadStore();
  bool initMiscellaneous();
  bool initBranches();
  int getEncodingARM(Data);
  int mEncodingARM;
  Data mOpcode;
  bool mValid;
  //will probably have private functions
};

}

#endif /* EDA_INSTRUCTIONARM_H_ */
