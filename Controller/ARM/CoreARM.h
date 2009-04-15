// CoreARM.h -- Apr 15, 2009
//    by geohot
//  released under GPLv3, see http://gplv3.fsf.org/

#ifndef EDA_COREARM_H_
#define EDA_COREARM_H_

//intentionally not with the other header files

#include "Core.h"

namespace eda{

class CoreARM : public Core
{
public:
  void disassemble(int addr);   //should add to instruction cache


};

}

#endif /* EDA_COREARM_H_ */
