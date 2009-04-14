#ifndef CORE_H_
#define CORE_H_
#include "Server.h"

namespace eda {

class Core: public Observer {
public:
  Core(Edb* edb, Server* server);
  virtual ~Core();
  virtual void update();
  virtual void test();

private:
  Edb* mEdb;
  Server* mServer;

};

}

#endif /* CORE_H_ */
