// StatelessChangelist.h -- Apr 12, 2009
//    by geohot
//  released under GPLv3, see http://gplv3.fsf.org/

#ifndef EDA_STATELESSCHANGELIST_H_
#define EDA_STATELESSCHANGELIST_H_
//prototypes for:
//	StatelessChangelist
//	StatelessData

#include "macros.h"
#include "Changelist.h"
#include "StatelessData.h"
#include <vector>
namespace eda {

//this is a stateless changelist
//contains entries like R3<-[PC+8]
//hmm this also needs a very non-specific ALU

//every entry has finite characteristics

//every ARM assembly instruction is of the following form
  //[register+-Data]

//target is memory[register+-register], memory[register+-immed]

class Changelist;

class StatelessChangelist
{
public:
  Changelist resolve(int changelistNumber, RegisterFile *r, Memory *m);
  //generates a changelist with state using the register file and memory
  //add both memory moving things and ALU things
  //should be const
  void addChange(StatelessData target, StatelessData source);
  void debugPrint();
  bool isRegisterAffected(int reg);
  Data resolveToRegisterWithRegister(int reg, Data Value);
private:
  std::vector<std::pair<StatelessData, StatelessData> > mInternalChangelist;
//some rep
};

}

#endif /* EDA_STATELESSCHANGELIST_H_ */
