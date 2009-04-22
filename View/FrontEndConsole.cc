// FrontEndConsole.cc -- Apr 14, 2009
//    by geohot
//  released under GPLv3, see http://gplv3.fsf.org/

//this gets passed a pointer to the model, read only
//and a pointer to the core mailbox and the model mailbox

#include "Mailbox.h"
#include "FrontEndConsole.h"
#include "macros.h"
#include "Bank.h"

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
using namespace std;

namespace eda {

int FrontEndConsole::strtoint(string in) {
  int ret;
  if (in.substr(0, 2) == "0x") {
    stringstream ss(in.substr(2));
    ss >> std::hex >> ret;
  } else {
    stringstream ss(in);
    ss >> std::dec >> ret;
  }
  return ret;
}

bool FrontEndConsole::lexer(string cmd) {
  vector<string> argv;
  size_t start = cmd.find_first_not_of(" ", 0);
  size_t end = cmd.find_first_of(" ", start);
  while (end != string::npos || start != string::npos) {
    argv.push_back(cmd.substr(start, end - start));
    start = cmd.find_first_not_of(" ", end);
    end = cmd.find_first_of(" ", start);
  }
  //info << "lexed " << argv.size() << endl;
  //parsing commands here
  //should be min nonambiguous, w/e
  if (argv[0] == "loadfile" && argv.size() == 3) {
    mBank->lock(LOCKED_SERVER);
    mBank->mem()->loadFile(argv[1].c_str(), strtoint(argv[2]));
    mBank->unlock(LOCKED_SERVER);
  } else if (argv[0] == "md" && argv.size() == 4) {
    mBank->lock(LOCKED_SERVER);
    mBank->mem()->consoleDump(strtoint(argv[1]), strtoint(argv[2]), strtoint(
        argv[3]));
    mBank->unlock(LOCKED_SERVER);
  } else if (argv[0] == "mw" && argv.size() == 4) {
    //mw <addr> <data> <clnum>
    mBank->lock(LOCKED_SERVER);
    (*(mBank->mem()))[strtoint(argv[1])].set(strtoint(argv[3]), strtoint(
        argv[2]));
    mBank->unlock(LOCKED_SERVER);
  } else if (argv[0] == "segments") {
    mBank->lock(LOCKED_SERVER);
    mBank->mem()->debugPrint();
    mBank->unlock(LOCKED_SERVER);
  } else if ((argv[0] == "d" || argv[0] == "disassemble") && argv.size() == 2) {
    mCore->sendMail(Mail(CORE_DISASSEMBLE, strtoint(argv[1])));
  } else if (argv[0] == "p" && argv.size() == 2) { //print instruction
    mBank->lock(LOCKED_SERVER);
    std::map<Data, Instruction>::iterator i = mBank->mInstructionCache.find(
        strtoint(argv[1]));
    if (i != mBank->mInstructionCache.end())
      i->second.debugPrint();
    else
      cout << "Not in instruction cache" << endl;
    mBank->unlock(LOCKED_SERVER);
  } else if (argv[0] == "analyse" && argv.size() == 2) {
    mCore->sendMail(Mail(CORE_ANALYSE, strtoint(argv[1])));
  } else if (argv[0] == "dumpicache") {
    mBank->lock(LOCKED_SERVER);
    InstructionIterator iter = mBank->mInstructionCache.begin();
    while (iter != mBank->mInstructionCache.end()) {
      cout << "entry(0x" << std::hex << iter->first << "): ";
      iter->second.mString.consolePrint();
      ++iter;
    }
    mBank->unlock(LOCKED_SERVER);
  } else if (argv[0] == "dumpfcache") {
    mBank->lock(LOCKED_SERVER);
    //mBank->mFunctionCache.debugPrint();
    cout << "broken" << endl;
    mBank->unlock(LOCKED_SERVER);
  } else {
    cout << "syntax error" << endl;
    return false;
  }

  return true;
}

void FrontEndConsole::runLoop() {
  info << "CFE is running" << endl;
  string cmd;
  //script some crap
  lexer("loadfile bootrom 0x400000");
  lexer("loadfile Memory_data 0x0");
  lexer("analyse 0x400054");
  while (1) //tons of mail
  {
    //cout << "EDA> ";  //stupid threads ruin my prompt
    getline(cin, cmd);
    lexer(cmd); //couldn't be handled locally, send to core
  }
}

}
