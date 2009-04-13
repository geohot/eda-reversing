// ParsedInstruction.h -- Apr 13, 2009
//    by geohot

#ifndef EDA_PARSEDINSTRUCTION_H_
#define EDA_PARSEDINSTRUCTION_H_

//used to be eda_string

#include <vector>
#include <string>

namespace eda {

//DT is datatype
#define DT_INSTRUCTION 0
#define DT_CONDITION 1
#define DT_REGISTER 2
#define DT_IMMED 3
#define DT_FORMATTING 4

class ParsedInstruction
{
public:
  void consolePrint();
  void add(std::string, int);
private:
  std::vector<std::pair<std::string,int> > mString;
};

}


#endif /* EDA_PARSEDINSTRUCTION_H_ */
