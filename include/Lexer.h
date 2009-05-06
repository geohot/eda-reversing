// Lexer.h -- May 5, 2009
//    by geohot
//  released under GPLv3, see http://gplv3.fsf.org/

// These are some parsing functions for input

#include <string>
#include <vector>

using namespace std;

namespace eda {

class Lexer {
public:
  static void splitSpaces(std::string cmd, std::vector<string>* argv);
  static void splitAddress(std::string cmd, std::vector<string>* argv);
};

}
