// StatelessChangelist.cc -- Apr 12, 2009
//    by geohot

#include "StatelessChangelist.h"

using namespace eda;

StatelessChangelist::StatelessChangelist addChange(StatelessData lhs, StatelessData rhs)
{
  mInternalChangelist.push_back(std::make_pair(lhs,rhs));

}

//StatelessData constructors below this line

//3
StatelessData::StatelessData(u32 data)
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
  mDataType=DATATYPE_REGOPER;
}

//[R3+4]
StatelessData::StatelessData(int oper, StatelessData d)
{
  mOperation=oper;
  mOperand=d;
  mDataType=DATATYPE_OPER;
}

