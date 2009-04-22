// FrontEndConsole.h -- Apr 14, 2009
//    by geohot
//  released under GPLv3, see http://gplv3.fsf.org/

#ifndef EDA_FRONTENDCONSOLE_H_
#define EDA_FRONTENDCONSOLE_H_

#include "FrontEnd.h"
#include "macros.h"

#include <string>

namespace eda {

class FrontEndConsole: public FrontEnd {
public:
  FrontEndConsole(Bank *bank) :
    FrontEnd(bank) {
  }
  bool lexer(std::string);
private:
  int strtoint(std::string);
  void runLoop();
};

}

#endif /* EDA_FRONTENDCONSOLE_H_ */
