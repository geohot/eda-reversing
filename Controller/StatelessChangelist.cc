// StatelessChangelist.cc -- Apr 12, 2009
//    by geohot
//  released under GPLv3, see http://gplv3.fsf.org/

#include "../Model/RegisterFile.h"
#include "../Model/Memory.h"

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

//*******************************StatelessData Class Methods*******************************

StatelessData::StatelessData()
{
  //blank constructor
}

//3
StatelessData::StatelessData(Data data)
{
  mData=data;
  mDataType=DATATYPE_CONST;
}

//R2
StatelessData::StatelessData(int reg)
{
  mRegister=reg;
  mDataType=DATATYPE_REG;
}

void StatelessData::debugPrint()
{
  if(mDataType==DATATYPE_CONST)
  {
    std::cout << "#" << std::hex << mData;
  }
  else if(mDataType==DATATYPE_REG)
  {
    std::cout << "R(" << mRegister << ")";
  }
  else if(mDataType==DATATYPE_OPER && mOperation==OPERATION_DEREF)
  {
    std::cout << "[";
    mOperand->debugPrint();
    std::cout << "]";
  }
  else if(mDataType==(DATATYPE_REG|DATATYPE_OPER))
  {
    std::cout << "R(" << mRegister << ") " << operations_s << " ";
    mOperand->debugPrint();
  }
}

//R3+R2 LSL 3
StatelessData::StatelessData(int reg, int oper, StatelessData *d)
{
  mOperand=d;
  mOperation=oper;
  mRegister=reg;
  mDataType=DATATYPE_REG|DATATYPE_OPER;
}

//[R3+4]
StatelessData::StatelessData(int oper, StatelessData *d)
{
  mOperation=oper;
  mOperand=d;
  mDataType=DATATYPE_OPER;
}

Data StatelessData::resolve(int changelistNumber, RegisterFile *r, Memory *m)
//add state
//should really be const
{
  if(mDataType==DATATYPE_CONST) return mData;
  else if(mDataType==DATATYPE_REG) return (*r)[mRegister][changelistNumber];
  else if(mDataType==(DATATYPE_REG|DATATYPE_OPER))
  {
    return evaluateOperation( (*r)[mRegister][changelistNumber],
        mOperand->resolve(changelistNumber, r,m),
        mOperation);
  }
  else if(mDataType==DATATYPE_OPER)
  {
    if(mOperation==OPERATION_NONE)
      return mOperand->resolve(changelistNumber, r,m);
    else if(mOperation==OPERATION_DEREF)
      return (*m)[mOperand->resolve(changelistNumber, r,m)][changelistNumber];

    debug << "can't eval high operation" << std::endl;
    return 0;
  }
  return 0;
}

Location StatelessData::resolveLocation(int changelistNumber, RegisterFile *r, Memory *m)
//add state
{
  if(mDataType==DATATYPE_CONST) return Location(mData);  //memory address
  else if(mDataType==DATATYPE_REG) return Location(mRegister);     //register
  else if(mDataType==(DATATYPE_REG|DATATYPE_OPER))       //must be inside deref, like [R3+4]
  {
    return evaluateOperation( (*r)[mRegister][changelistNumber],
        mOperand->resolve(changelistNumber, r,m),
        mOperation);
  }
  else if(mDataType==DATATYPE_OPER)
  {
    if(mOperation==OPERATION_NONE) { debug << "i'm confused" << std::endl; return 0; }
    else if(mOperation==OPERATION_DEREF)
      return Location(mOperand->resolve(changelistNumber, r,m));  //resolved memory address

    debug << "can't eval high operation" << std::endl;
    return 0;
  }
  return 0;
}

Data StatelessData::evaluateOperation(Data lhs, Data rhs, int operation)
{
  switch(operation)
  {
    case OPERATION_AND: return lhs&rhs;
    case OPERATION_XOR: return lhs^rhs;
    case OPERATION_SUB: return lhs-rhs;
    case OPERATION_RSB: return rhs-lhs;
    case OPERATION_ADD: return lhs+rhs;

    case OPERATION_ORR: return lhs|rhs;
    case OPERATION_BIC: return lhs&(~rhs);
    case OPERATION_LSL: return lhs<<rhs;
    case OPERATION_LSR: return lhs>>rhs;
  }
  debug << "can't eval operation" << std::endl;
  return 0;
}

//*******************************Location Class Methods*******************************

Location::Location(int reg)
{
  mDataType=DATATYPE_REG;
  mStore=(Data)reg;
}

Location::Location(Data memloc)
{
  mDataType=DATATYPE_CONST;
  mStore=memloc;
}

void Location::assign(int changelistNumber, Data value, RegisterFile *r, Memory *m)
{
  switch(mDataType)
  {
    case DATATYPE_CONST:        //memory
      (*m)[mStore].set(changelistNumber, value);
      break;
    case DATATYPE_REG:
      (*r)[(int)mStore].set(changelistNumber, value);
      break;
  }
}
