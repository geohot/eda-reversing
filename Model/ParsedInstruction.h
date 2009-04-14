// ParsedInstruction.h -- Apr 13, 2009
//    by geohot

#ifndef EDA_PARSEDINSTRUCTION_H_
#define EDA_PARSEDINSTRUCTION_H_

//used to be eda_string

#include <vector>
#include <string>

#include "../include/macros.h"

namespace eda {

//DT is datatype
#define DT_OPCODE 0
#define DT_CONDITION 1
#define DT_REGISTER 2
#define DT_IMMED 3
#define DT_FORMATTING 4
#define DT_FLAGS 5
#define DT_SYMBOL 6
#define DT_DECIMAL 7
#define DT_SUBOPCODE 8

class ParsedInstruction
{
public:
  void consolePrint();
  void debugPrint();
  void add(std::pair<std::string,int>);
  void add(std::string, int);
  void add(Data);
  void add(Data, int);
//let me try it, alternate mutators for more readibility
  ParsedInstruction& operator<<(std::pair<std::string,int>);
  ParsedInstruction& operator<<(const char *);
  ParsedInstruction& operator<<(Data a);
private:
  std::vector<std::pair<std::string,int> > mString;
};

}


#endif /* EDA_PARSEDINSTRUCTION_H_ */
