// ParsedInstruction.h -- Apr 13, 2009
//    by geohot
//  released under GPLv3, see http://gplv3.fsf.org/

#ifndef EDA_PARSEDINSTRUCTION_H_
#define EDA_PARSEDINSTRUCTION_H_

//used to be eda_string

#include <vector>
#include <string>

#include "macros.h"

namespace eda {

class Memory;

//DT is datatype
#define DT_OPCODE 0
#define DT_CONDITION 1
#define DT_REGISTER 2
#define DT_IMMED 3
#define DT_FORMATTING 4
#define DT_FLAG 5
#define DT_SYMBOL 6
#define DT_DECIMAL 7
#define DT_SUBOPCODE 8
#define DT_SIGNED 9
#define DT_OFFSETADDRESS 10    //deferenced
#define DT_OFFSETDATA 11 //isn't
class ParsedInstruction {
public:
  void consolePrint();
  std::string webPrint(Data, Memory *);
  void debugPrint();
  void add(std::pair<std::string, int>);
  void add(std::string, int);
  void add(Data);
  void add(Data, int);
  //let me try it, alternate mutators for more readibility
  ParsedInstruction& operator<<(std::pair<std::string, int>);
  ParsedInstruction& operator<<(const char *);
  ParsedInstruction& operator<<(Data a);
private:
  std::vector<std::pair<std::string, int> > mString;
  Data stoi(std::string);
};

}

#endif /* EDA_PARSEDINSTRUCTION_H_ */
