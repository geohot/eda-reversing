// Changelist.cc -- Apr 13, 2009
//    by geohot

#include "Changelist.h"

namespace eda {

Changelist::Changelist(int changelistNumber)
{
  mChangelistNumber=changelistNumber;
}

void Changelist::commit(RegisterFile *r, Memory *m)
{
  std::vector<std::pair<Location, Data> >::iterator walk=mInternalChangelist.begin();
  while(walk!=mInternalChangelist.end())
  {
    //assign the Data to the Location
    walk->first.assign(mChangelistNumber, walk->second, r, m);
    ++walk;
  }
}

void Changelist::addChange(Location target, Data source)
{
  mInternalChangelist.push_back(std::make_pair(target,source));
}

}
