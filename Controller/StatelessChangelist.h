// StatelessChangelist.h -- Apr 12, 2009
//    by geohot

#ifndef EDA_STATELESSCHANGELIST_H_
#define EDA_STATELESSCHANGELIST_H_
//prototypes for:
//	StatelessChangelist
//	StatelessData

#include "../edaMacros.h"
#include "Changelist.h"
#include <vector>
namespace eda {

//this is a stateless changelist
//contains entries like R3<-[PC+8]
//hmm this also needs a very non-specific ALU

//every entry has finite characteristics

//every ARM assembly instruction is of the following form
  //[register+-Data]

//target is memory[register+-register], memory[register+-immed]

class StatelessData;
class Changelist;

class StatelessChangelist
{
public:
  Changelist resolve(int changelistNumber, RegisterFile *r, Memory *m);
  //generates a changelist with state using the register file and memory
  //add both memory moving things and ALU things
  //should be const
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
#define OPERATION_LSL 7
#define OPERATION_LSR 8

//high operations, evaluateOperation doesn't do these
#define OPERATION_DEREF 256

#define DATATYPE_CONST 1
#define DATATYPE_REG 2
#define DATATYPE_OPER 4


//Location is either a register or memory for now

class Location
{
public:
  Location(Data);       //Data is a memory address
  Location(int);        //int is a register
  void assign(int changelistNumber, Data value, RegisterFile *r, Memory *m);
private:
  Data mStore;
  int mDataType;
};

class StatelessData;

//StatelessData is just a storage class, actually not, have it walk
class StatelessData
{
public:
  StatelessData();
  StatelessData(Data);  //immediate data
  StatelessData(int);   //immediate register
  StatelessData(int, int, StatelessData *);
  StatelessData(int, StatelessData *); //like dereference

  Data resolve(int changelistNumber, RegisterFile *r, Memory *m);
  Location resolveLocation(int changelistNumber, RegisterFile *r, Memory *m);
private:
  Data evaluateOperation(Data lhs, Data rhs, int operation);
  int mDataType;
  int mRegister;
  int mOperation;
  Data mData;
  StatelessData *mOperand;
};

}

#endif /* EDA_STATELESSCHANGELIST_H_ */
