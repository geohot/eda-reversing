// StatelessData.h -- Apr 15, 2009
//    by geohot
//  released under GPLv3, see http://gplv3.fsf.org/

#ifndef STATELESSDATA_H_
#define STATELESSDATA_H_

#include "macros.h"

namespace eda {

//forward declarations :-) google
class RegisterFile;
class Memory;

//block 1
#define OPERATION_AND 0
#define OPERATION_XOR 1
#define OPERATION_RSB 2
#define OPERATION_SUB 3
#define OPERATION_ADD 4

//odd
#define OPERATION_ORR 5
#define OPERATION_BIC 6

//block 2
#define OPERATION_LSL 8
#define OPERATION_LSR 9
#define OPERATION_ASR 10
#define OPERATION_ROR 11

const char operations_s[][4] = { "&", "^", "`-", "-", "+", "|", "~&", "", "<<",
    ">>", ">>>", ">/>" };
//high operations, evaluateOperation doesn't do these
#define OPERATION_DEREF 256
#define OPERATION_NONE 257

#define DATATYPE_CONST 1
#define DATATYPE_REG 2
#define DATATYPE_OPER 4

//Location is either a register or memory for now
// it resolves one away from the end

class Location {
public:
  Location(Data); //Data is a memory address
  Location(int); //int is a register
  void assign(int changelistNumber, Data value, RegisterFile *r, Memory *m);
private:
  Data mStore;
  int mDataType;
};

//StatelessData is just a storage class, actually not, have it walk
class StatelessData {
public:
  StatelessData();
  StatelessData(Data); //immediate data
  StatelessData(int); //immediate register
  StatelessData(int, int, StatelessData *);
  StatelessData(int, StatelessData *); //like dereference
  void debugPrint();
  Data resolve(int changelistNumber, RegisterFile *r, Memory *m);
  Location resolveLocation(int changelistNumber, RegisterFile *r, Memory *m);
  int mDataType;
  int mRegister;
private:
  Data evaluateOperation(Data lhs, Data rhs, int operation);
  int mOperation;
  Data mData;
  StatelessData *mOperand;
};

}

#endif /* STATELESSDATA_H_ */
