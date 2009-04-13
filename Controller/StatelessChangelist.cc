// StatelessChangelist.cc -- Apr 12, 2009
//    by geohot

#include "StatelessChangelist.h"

using namespace eda;

//*******************************StatelessChangelist Class Methods*******************************

StatelessChangelist::StatelessChangelist addChange(StatelessData lhs, StatelessData rhs)
{
  mInternalChangelist.push_back(std::make_pair(lhs,rhs));

}

ChangeList StatelessChangelist::resolve(const RegisterFile *r, const Memory *m)
//walk over the StatelessData and add state
{
  ChangeList ret;
  std::vector<std::pair<StatelessData, StatelessData> >::iterator walk=mInternalChangeList.begin();
  while(walk!=mInternalChangeList.end())
  {
    ret.addChange(walk->first.resolveLocation(), walk->second.resolve());
  }
  return ret;
}

//*******************************StatelessData Class Methods*******************************

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

//R3+R2 LSL 3
StatelessData::StatelessData(int reg, int oper, StatelessData d)
{
  mOperand=d;
  mOperation=oper;
  mRegister=reg;
  mDataType=DATATYPE_REG|DATATYPE_OPER;
}

//[R3+4]
StatelessData::StatelessData(int oper, StatelessData d)
{
  mOperation=oper;
  mOperand=d;
  mDataType=DATATYPE_OPER;
}

Data StatelessData::resolve
  (int changelistNumber, const RegisterFile *r, const Memory *m)
//add state
{
  if(mDataType==DATATYPE_CONST) return mData;
  else if(mDataType==DATATYPE_REG) return (*r)[mRegister][changelistNumber];
  else if(mDataType==(DATATYPE_REG|DATATYPE_OPER))
  {
    return evaluateOperation( (*r)[mRegister][changelistNumber],
        mOperand.resolve(changelistNumber, r,m),
        mOperation);
  }
  else if(mDataType==DATATYPE_OPER)
  {
    if(OPERATION_NONE) return mOperand.resolve;
    else if(OPERATION_DEREF) return (*m)[mOperand.resolve][changelistNumber];

    debug << "can't eval high operation" << endl;
    return 0;
  }
}

Location StatelessData::resolve
  (int changelistNumber, const RegisterFile *r, const Memory *m)
//add state
{
  if(mDataType==DATATYPE_CONST) return Location(mData);  //memory address
  else if(mDataType==DATATYPE_REG) return Location(mRegister);     //register
  else if(mDataType==(DATATYPE_REG|DATATYPE_OPER))       //must be inside deref, like [R3+4]
  {
    return evaluateOperation( (*r)[mRegister][changelistNumber],
        mOperand.resolve(changelistNumber, r,m),
        mOperation);
  }
  else if(mDataType==DATATYPE_OPER)
  {
    if(OPERATION_NONE) { debug << "i'm confused" << endl; return 0; }
    else if(OPERATION_DEREF) return Location(mOperand.resolve);  //resolved memory address

    debug << "can't eval high operation" << endl;
    return 0;
  }
}

Data StatelessData::evaluateOperation(Data lhs, Data rhs, int operation)
{
  switch(operation)
  {
    case OPERATION_AND: return lhs&rhs;
    case OPERATION_XOR: return lhs^rhs;
    case OPERATION_SUB: return lhs-rhs;
    case OPERATION_ADD: return lhs+rhs;
    case OPERATION_ORR: return lhs|rhs;
    case OPERATION_BIC: return lhs&(~rhs);
  }
  debug << "can't eval operation" << endl;
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
