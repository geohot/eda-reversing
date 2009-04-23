// FrontEndServer.h -- Apr 15, 2009
//    by geohot
//  released under GPLv3, see http://gplv3.fsf.org/

#ifndef FRONTENDSERVER_H_
#define FRONTENDSERVER_H_

#include "FrontEnd.h"
#include "macros.h"

#include <string>

namespace eda {

class FrontEndServer: public FrontEnd {
public:
  FrontEndServer(Bank *bank) :
    FrontEnd(bank) {
  }
private:
  int hexstrtoint(std::string);
  bool lexer(int, std::string);
  bool serverListen();
  void runLoop();
  void serve(int);
  void serveHeaders(int, const char *);
  bool serveFile(int, const char*, const char *);
  int mSocket;
};

}

#endif /* FRONTENDSERVER_H_ */
