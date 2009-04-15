// Server.h -- Apr 14, 2009
//    by posixninja
//  released under GPLv3, see http://gplv3.fsf.org/

#ifndef EDA_SERVER_H_
#define EDA_SERVER_H_
#include "Mailbox.h"
#include "Bank.h"

class Core;     //forward declaration here

namespace eda {

class Server: public Observable, public Observer {
public:
  Server(Bank* bank);
  virtual ~Server();
  virtual void update();
  void attach(Core *);
private:
  Bank* mBank;
  Mailbox* mCore;
};

}

#endif /* EDA_SERVER_H_ */
