// Lexer.cc -- May 5, 2009
//    by geohot
//  released under GPLv3, see http://gplv3.fsf.org/

// These are some parsing functions for input

#include "Lexer.h"

#include <string>
#include <vector>

using namespace std;

namespace eda {

void Lexer::splitSpaces(string cmd, vector<string>* argv) {
  size_t start = cmd.find_first_not_of(" ", 0);
  size_t end = cmd.find_first_of(" ", start);
  while (end != string::npos || start != string::npos) {
    argv->push_back(cmd.substr(start, end - start));
    start = cmd.find_first_not_of(" ", end);
    end = cmd.find_first_of(" ", start);
  }
}

void Lexer::splitAddress(string cmd, vector<string>* argv) {
  size_t start = cmd.find_first_not_of("/?", 0);
  size_t end = cmd.find_first_of("/?", start);
  while (end != std::string::npos || start != std::string::npos) {
    argv->push_back(cmd.substr(start, end - start));
    start = cmd.find_first_not_of("/?", end);
    end = cmd.find_first_of("/?", start);
  }
}

}
