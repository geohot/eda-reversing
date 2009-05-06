// FrontEndRest.h -- May 5, 2009
//    by geohot
//  released under GPLv3, see http://gplv3.fsf.org/

#include "FrontEnd.h"
#include "Lexer.h"

namespace eda {

class FrontEndRest : public FrontEnd {
public:
  FrontEndRest(Bank *bank) :
    FrontEnd(bank) {
  }
private:
  void runLoop();
};

}
