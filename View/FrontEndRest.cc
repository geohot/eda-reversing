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
    cout << "got " << words.size() << " words" << endl;
  }
}

}
