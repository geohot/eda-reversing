// Changelist.cc -- Apr 13, 2009
//    by geohot

#include "Changelist.h"

namespace eda {

void Changelist::commit(int changelistNumber, RegisterFile *r, Memory *m)
{
  std::vector<std::pair<Location, Data> >::iterator walk=mInternalChangeList.begin();
  while(walk!=mInternalChangeList.end())
  {
    walk->first.assign(changelistNumber, walk->second, r, m);
  }
}

void Changelist::addChange(Location target, Data source)
{
  mInternalChangelist.push_back(std::make_pair(target,source));
}

}
