// Instruction.h -- Apr 13, 2009
//    by geohot
//  released under GPLv3, see http://gplv3.fsf.org/

#ifndef EDA_INSTRUCTION_H_
#define EDA_INSTRUCTION_H_

//not sure whether this belongs in the controller or the model
//model works without it but will store it, controller needs

//forward declares here instead?
#include "ParsedInstruction.h"
#include "StatelessChangelist.h"

namespace eda {

class Instruction
//a lot here is extended or overloaded by the individual arch
{
public:
  //important disassembly constructor in the arch
  ParsedInstruction mString;    //this is the String the instruction looks like
//protected:
  StatelessChangelist mAction;  //this is the Action the instruction does
};

}

#endif /* EDA_INSTRUCTION_H_ */
