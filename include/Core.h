// Core.h -- Apr 14, 2009
//    by posixninja
//  released under GPLv3, see http://gplv3.fsf.org/

#ifndef EDA_CORE_H_
#define EDA_CORE_H_
#include "Mailbox.h"
#include "Bank.h"
#include "Function.h"

namespace eda {

#define CORE_NONE 0
#define CORE_DISASSEMBLE 1
#define CORE_ANALYSE 2

class Core: public Observer {
public:
  Core();
  Core(Bank* bank);     //, Server* server(Server is in a different thread)
  virtual ~Core();
  virtual void update();
  virtual void test();
  virtual InstructionIterator disassemble(Data);
  virtual void fastAnalyse(Data, Function *, bool);
  void runLoop();
  Mailbox mMail;
protected:
  Bank* mBank;

};

}

#endif /* EDA_CORE_H_ */
