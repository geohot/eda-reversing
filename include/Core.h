// Core.h -- Apr 14, 2009
//    by posixninja
//  released under GPLv3, see http://gplv3.fsf.org/

#ifndef EDA_CORE_H_
#define EDA_CORE_H_
#include "Mailbox.h"
#include "Bank.h"

namespace eda {

class Core: public Observer {
public:
  Core();
  Core(Bank* bank);     //, Server* server(Server is in a different thread)
  virtual ~Core();
  virtual void update();
  virtual void test();
  virtual void disassemble(int);
  void runLoop();
  Mailbox mMail;
private:
  Bank* mBank;

};

}

#endif /* EDA_CORE_H_ */
