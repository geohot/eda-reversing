#ifndef SERVER_H_
#define SERVER_H_
#include "Edb.h"

namespace eda {

class Server: public Observable, public Observer {
public:
  Server(Edb* edb);
  virtual ~Server();
  virtual void update();

private:
  Edb* mEdb;
};

}

#endif /* SERVER_H_ */
