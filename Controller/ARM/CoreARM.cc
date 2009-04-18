// CoreARM.cc -- Apr 15, 2009
//    by geohot
//  released under GPLv3, see http://gplv3.fsf.org/

#include "CoreARM.h"
#include "InstructionARM.h"     //only allowed to be included because this is ARM

//several things should be defined
//like the PC register

namespace eda {

CoreARM::CoreARM(Bank *mBank): Core(mBank)
{
  //clear registers or something?
}

InstructionIterator CoreARM::disassemble(Data addr)
{
  mBank->lock(LOCKED_CORE);
  InstructionARM a=(*(mBank->mem()))[addr][0];      //always the 0 changelist
  InstructionIterator ret=mBank->mInstructionCache.insert(std::make_pair(addr, a)).first;
  mBank->unlock(LOCKED_CORE);
  return ret;
  //a.debugPrint();
}

//can probably eventually migrate to core
void CoreARM::fastAnalyse(Data addr, Function *currentFunction, bool first)
//recursive fast analyser, be careful here
{
  Data a;
  mBank->lock(LOCKED_CORE);

  //info << "analysing " << std::hex << addr << std::endl;
  InstructionIterator working;
  do
  {
    //info << "  disassembling" << addr << std::endl;
    if(!(mBank->mem()->exists(addr)))
    {
      //info << "memory isn't real: " << addr << std::endl;
      goto done;
    }
    working=disassemble(addr);
    if(first==true) {    //mark as landingPad
      working->second.mLandingPad=true;
      first=false;
    }
    currentFunction->mInstructions.insert(std::make_pair(addr, &(working->second)));
    if(working->second.mReturn==true)
    {
      /*s << std::hex << "<code type=\"end\">" << addr << "</code>";
      currentFunction->mBranchData.push_back(s.str()); s.str("");*/

      //info << "function return at: " << addr << std::endl;
      goto done;
    }
    addr+=4;    //needs to be param
  } while(working->second.mBranch==false);
  addr-=4;
  a=working->second.mAction.resolveToRegisterWithRegister(15, addr+8);
  //info << std::hex << "found next branch at " << addr << " to " << a << std::endl;
  if(working->second.mConditional)      //if conditional, continue here as well
  {
    if(mBank->mInstructionCache.find(addr+4)==mBank->mInstructionCache.end()) {
      //not in icache already
      currentFunction->mBranchData.push_back(Branch(BRANCH_CONDITIONFAIL, addr, addr+4));
      fastAnalyse(addr+4, currentFunction, true);       //recurse..weeeee
    }
  }
  //for normal branches + conditionals
  if(mBank->mInstructionCache.find(a)==mBank->mInstructionCache.end()) {
    //not in icache already
    //if linked, add it to the function cache
    if(working->second.mLinkedBranch==true) {
      fastAnalyse(addr+4, currentFunction, false);     //continue here, since it'll be back
      //info << "function found at: " << a << std::endl;
      fastAnalyse(a, mBank->mFunctionCache.add(a), false);     //and do the new function
    }
    else
    {
      if(working->second.mConditional)
        currentFunction->mBranchData.push_back(Branch(BRANCH_CONDITIONPASS, addr, a));
      else
        currentFunction->mBranchData.push_back(Branch(BRANCH_FOLLOW, addr, a));
      fastAnalyse(a, currentFunction, true);
    }
  }
done:
  mBank->unlock(LOCKED_CORE);
}

}
