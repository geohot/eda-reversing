// FrontEndRest.cc -- May 5, 2009
//    by geohot
//  released under GPLv3, see http://gplv3.fsf.org/

#include "FrontEndRest.h"

#include <string>
#include <vector>

#include <iostream>

using namespace std;

namespace eda {

void FrontEndRest::runLoop() {
  info << "REST is running" << endl;
  string cmd;
  while (1) { //tons of mail
    getline(cin, cmd);
    std::vector<string> words;
    Lexer::splitSpaces(cmd, &words);
    if(words.size() >= 2) {
      cout << "action: " << words[0] << endl;
      std::vector<string> address;
      Lexer::splitAddress(cmd, &address);
      cout << "got " << address.size() << " part address" << endl;
    }
  }
}

}
