#ifndef CORE_H_
#define CORE_H_
#include "Server.h"

namespace eda {

class Core: public Observer {
public:
  Core(Bank* bank, Server* server);
  virtual ~Core();
  virtual void update();
  virtual void test();

private:
  Bank* mBank;
  Server* mServer;

};

}

#endif /* CORE_H_ */
