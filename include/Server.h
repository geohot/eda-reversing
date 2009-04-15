#ifndef SERVER_H_
#define SERVER_H_
#include "Bank.h"

namespace eda {

class Server: public Observable, public Observer {
public:
  Server(Bank* bank);
  virtual ~Server();
  virtual void update();

private:
  Bank* mBank;
};

}

#endif /* SERVER_H_ */
