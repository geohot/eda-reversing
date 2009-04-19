// Function.h -- Apr 15, 2009
//    by geohot
//  released under GPLv3, see http://gplv3.fsf.org/

#ifndef EDA_FUNCTION_H_
#define EDA_FUNCTION_H_

#include "macros.h"
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include "Instruction.h"

namespace eda {

#define BRANCH_FOLLOW 0
#define BRANCH_CONDITIONFAIL 1
#define BRANCH_CONDITIONPASS 2

class Branch {
public:
  Branch(int type, Data from, Data to): mType(type), mFrom(from), mTo(to) {}
  std::string getXML()
  {
    std::stringstream ss;
    ss << std::hex << "<line color=\"";
    switch(mType) {
      case BRANCH_FOLLOW: ss << "blue"; break;
      case BRANCH_CONDITIONFAIL: ss << "red"; break;
      case BRANCH_CONDITIONPASS: ss << "green"; break;
    }
    ss << "\" to=\"" << mTo << "\" from=\"" << mFrom << "\"></line>";
    return ss.str();
  }
  int mType;
  Data mFrom;
  Data mTo;
};

class Function {
public:
  Function(Data start): mStart(start) {}
  Data mStart;
  //std::vector<std::pair<Data, Data> > mSegments;
  std::map<Data,Instruction *> mInstructions;
  std::vector<Branch> mBranchData;
};

}


#endif /* EDA_FUNCTION_H_ */
