// CoreARM.h -- Apr 15, 2009
//    by geohot
//  released under GPLv3, see http://gplv3.fsf.org/

#ifndef EDA_COREARM_H_
#define EDA_COREARM_H_

//intentionally not with the other header files

#include "Core.h"
#include "Bank.h"
#include "Function.h"

namespace eda {


class CoreARM : public Core
{
public:
  CoreARM(Bank* bank);
  InstructionIterator disassemble(Data);   //should add to instruction cache
  void fastAnalyse(Data, Function *,bool);
};

}

#endif /* EDA_COREARM_H_ */
