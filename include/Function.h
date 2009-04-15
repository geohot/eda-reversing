// Function.h -- Apr 15, 2009
//    by geohot
//  released under GPLv3, see http://gplv3.fsf.org/

#ifndef EDA_FUNCTION_H_
#define EDA_FUNCTION_H_

#include "macros.h"
#include <string>
#include <vector>

namespace eda {

class Function {
public:
  Function(Data start, std::string name): mName(name), mStart(start) {}
  std::string mName;
  Data mStart;
  std::vector<std::pair<Data, Data> > mSegments;
};

}


#endif /* EDA_FUNCTION_H_ */
