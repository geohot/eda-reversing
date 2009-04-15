// ConsoleFrontEnd.h -- Apr 14, 2009
//    by geohot
//  released under GPLv3, see http://gplv3.fsf.org/

#ifndef EDA_CONSOLEFRONTEND_H_
#define EDA_CONSOLEFRONTEND_H_

#include "Mailbox.h"
#include "threading.h"
#include "macros.h"

namespace eda
{

class ConsoleFrontEnd
{
public:
  ConsoleFrontEnd();
  ConsoleFrontEnd(Mailbox *core, Mailbox *edb);
  ~ConsoleFrontEnd();
  void chill();         //hang out here until the thread finishes
private:
  static void entryPoint(void *);
  void runLoop();
  threadContainer mThread;
  Mailbox *mCore;
  Mailbox *mEdb;
};

}

#endif /* EDA_CONSOLEFRONTEND_H_ */
