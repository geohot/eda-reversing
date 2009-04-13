// Changelist.h -- Apr 13, 2009
//    by geohot

#ifndef CHANGELIST_H_
#define CHANGELIST_H_

#include "../edaMacros.h"
#include "StatelessChangelist.h"

namespace eda {

class Changelist
{
public:
  void commit(int changelistNumber, RegisterFile *r, Memory *m);
  void addChange(Location target, Data source);
private:
  std::vector<std::pair<Location, Data> > mInternalChangelist;
};

}

#endif /* CHANGELIST_H_ */
