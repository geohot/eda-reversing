// Changelist.h -- Apr 13, 2009
//    by geohot

#ifndef EDA_CHANGELIST_H_
#define EDA_CHANGELIST_H_

#include "../Model/RegisterFile.h"
#include "../Model/Memory.h"

#include "../edaMacros.h"
#include "StatelessChangelist.h"

namespace eda {

class Location;

class Changelist
{
public:
  Changelist(int changelistNumber);
  void commit(RegisterFile *r, Memory *m);
  void addChange(Location target, Data source);
private:
  int mChangelistNumber;
  std::vector<std::pair<Location, Data> > mInternalChangelist;
};

}

#endif /* EDA_CHANGELIST_H_ */
