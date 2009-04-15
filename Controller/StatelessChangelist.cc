// StatelessChangelist.cc -- Apr 12, 2009
//    by geohot
//  released under GPLv3, see http://gplv3.fsf.org/

#include "RegisterFile.h"
#include "Memory.h"

#include "StatelessChangelist.h"

#include <vector>
#include <iostream>

using namespace eda;

//*******************************StatelessChangelist Class Methods*******************************

void StatelessChangelist::addChange(StatelessData lhs, StatelessData rhs)
{
  mInternalChangelist.push_back(std::make_pair(lhs,rhs));
}

Changelist StatelessChangelist::resolve(int changelistNumber, RegisterFile *r, Memory *m)
//walk over the StatelessData and add state
{
  Changelist ret(changelistNumber);
  std::vector<std::pair<StatelessData,StatelessData> >::iterator walk=mInternalChangelist.begin();
  while(walk!=mInternalChangelist.end())
  {
    ret.addChange(walk->first.resolveLocation(changelistNumber,r,m), walk->second.resolve(changelistNumber,r,m));
    ++walk;
  }
  return ret;
}

//not really used yet, I thought I'd need it
bool StatelessChangelist::isRegisterAffected(int reg) {
  std::vector<std::pair<StatelessData,StatelessData> >::iterator walk=mInternalChangelist.begin();
  while(walk!=mInternalChangelist.end())
  {
    if(walk->first.mDataType==DATATYPE_REG && walk->first.mRegister==reg) return true;
    ++walk;
  }
  return false;
}

Data StatelessChangelist::resolveToRegisterWithRegister(int reg, Data Value)
//theres much more efficent ways to write this, but this is cooler
{
  RegisterFile temp_rf(16);    //allocate a temp RegisterFile
  Memory temp_m;           //and a temp memory

  temp_rf[reg].set(0,Value);    //set the register, rest are w/e

  Changelist temp_c=resolve(1,&temp_rf,&temp_m);
  temp_c.commit(&temp_rf,&temp_m);

  return temp_rf[reg][1];
}

void StatelessChangelist::debugPrint()
{
  std::vector<std::pair<StatelessData,StatelessData> >::iterator walk=mInternalChangelist.begin();
  while(walk!=mInternalChangelist.end())
  {
    walk->first.debugPrint();
    std::cout << " <- ";
    walk->second.debugPrint();
    std::cout << std::endl;
    ++walk;
  }
}
