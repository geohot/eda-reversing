// ConsoleFrontEnd.h -- Apr 14, 2009
//    by geohot
//  released under GPLv3, see http://gplv3.fsf.org/

#ifndef EDA_CONSOLEFRONTEND_H_
#define EDA_CONSOLEFRONTEND_H_

#include "threading.h"
#include "Mailbox.h"
#include "macros.h"
#include "Core.h"

namespace eda
{

class ConsoleFrontEnd
{
public:
  ConsoleFrontEnd();
  ConsoleFrontEnd(Bank *bank);
  ~ConsoleFrontEnd();
  void attach(Core *);
private:
  static void entryPoint(void *);
  void runLoop();
  threadContainer mThread;
  Mailbox *mCore;
  Bank *mBank;
};

}

#endif /* EDA_CONSOLEFRONTEND_H_ */
