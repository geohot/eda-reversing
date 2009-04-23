// Commands.h -- Apr 22, 2009
//    by geohot
//  released under GPLv3, see http://gplv3.fsf.org/

#ifndef EDA_COMMANDS_H_
#define EDA_COMMANDS_H_

#include <vector>
#include <string>
#include <sstream>

namespace eda {

#define COMMAND_PARAMS std::stringstream &response, std::vector<std::string> argv

class Bank;
class Mailbox;

class Commands {
public:
  Commands(Bank *bank, Mailbox *mailbox) :
    mBank(bank), mCore(mailbox) {
  }
  bool getFunctionList(COMMAND_PARAMS);
  bool getFunction(COMMAND_PARAMS);
  bool getFunctionBranchData(COMMAND_PARAMS);
  bool rename(COMMAND_PARAMS);
private:
  Mailbox *mCore;
  Bank *mBank;
};

}

#endif /* EDA_COMMANDS_H_ */
