// StatelessChangelist.h -- Apr 12, 2009
//    by geohot

#ifndef STATELESSCHANGELIST_H_
#define STATELESSCHANGELIST_H_
//prototypes for:
//	StatelessChangelist
//	StatelessData

#include <vector>
namespace eda {

//this is a stateless changelist
//contains entries like R3<-[PC+8]
//hmm this also needs a very non-specific ALU

//every entry has finite characteristics

//target is memory[register+-register], memory[register+-immed]

class StatelessData;

class StatelessChangelist
{
public:
  Changelist apply(const RegisterFile *r, const Memory *m);
  //generates a changelist with state using the register file and memory
  //add both memory moving things and ALU things
  void addChange(StatelessData target, StatelessData source);
private:
  std::vector<std::pair<StatelessData, StatelessData> > mInternalChangelist;
//some rep
};

#define OPERATION_NONE 0
#define OPERATION_AND 1
#define OPERATION_XOR 2
#define OPERATION_SUB 3
#define OPERATION_ADD 4
#define OPERATION_ORR 5
#define OPERATION_BIC 6

#define OPERATION_DEREF 10

#define DATATYPE_CONST 1
#define DATATYPE_REG 2
#define DATATYPE_OPER 3
#define DATATYPE_REGOPER 4


//StatelessData is just a storage class
class StatelessData
{
public:
  StatelessData(u32);  //immediate data
  StatelessData(int);   //immediate register
  StatelessData(int, int, StatelessData);
  StatelessData(int, StatelessData); //like dereference
private:
  u32 mDataType;
  u32 mRegister;
  u32 mOperation;
  u32 mData;
  StatelessData mOperand;
}

}

#endif /*S TATELESSCHANGELIST_H_ */
