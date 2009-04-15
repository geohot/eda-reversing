// FrontEndServer.h -- Apr 15, 2009
//    by geohot
//  released under GPLv3, see http://gplv3.fsf.org/

#ifndef FRONTENDSERVER_H_
#define FRONTENDSERVER_H_

#include "FrontEnd.h"
#include "macros.h"

namespace eda
{

class FrontEndServer : public FrontEnd
{
public:
  FrontEndServer(Bank *bank) : FrontEnd(bank) {}
private:
  bool serverListen();
  void runLoop();
  void serve(int);
  void serveHeaders(int);
  bool serveFile(int, const char*);
  int mSocket;
};

}

#endif /* FRONTENDSERVER_H_ */
