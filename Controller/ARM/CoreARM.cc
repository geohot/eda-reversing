// CoreARM.cc -- Apr 15, 2009
//    by geohot
//  released under GPLv3, see http://gplv3.fsf.org/

#include "CoreARM.h"
#include "InstructionARM.h"     //only allowed to be included because this is ARM
//several things should be defined
//like the PC register

namespace eda {

CoreARM::CoreARM(Bank *mBank) :
  Core(mBank) {
  //clear registers or something?
}

InstructionIterator CoreARM::disassemble(Data addr) {
  mBank->lock(LOCKED_CORE);
  InstructionARM a = (*(mBank->mem()))[addr][0]; //always the 0 changelist
  InstructionIterator ret = mBank->mInstructionCache.insert(std::make_pair(
      addr, a)).first;
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

  InstructionIterator working;
  std::map<Data, Instruction *>::iterator impblue;
#ifdef VERBOSE
  info << "analysing " << std::hex << addr << std::endl;
#endif
  do {

    //see if we've been here before, if so, die
    //also possibly set landing pad
    //also possibly add a blue implicit
    impblue = currentFunction->mInstructions.find(addr);
    if (impblue != currentFunction->mInstructions.end()) {
      if (first == true) { //mark as landingPad
        impblue->second->mLandingPad = true;
      }
      //instruction already here
      //adding implicit
      impblue = currentFunction->mInstructions.find(addr - 4);
      //if instruction is found in function and it isn't a branch, add the implicit blue line
      if (impblue != currentFunction->mInstructions.end()
          && impblue->second->mBranch == false)
        currentFunction->mBranchData.push_back(Branch(BRANCH_FOLLOW, addr - 4,
            addr));
#ifdef VERBOSE
      info << "repeat at " << std::hex << addr << std::endl;
#endif
      goto done;
    }

    //info << "  disassembling" << addr << std::endl;
    if (!(mBank->mem()->exists(addr))) {
#ifdef VERBOSE
      info << "memory isn't real: " << addr << std::endl;
#endif
      goto done;
    }

    working = disassemble(addr);
    if (first == true) { //mark as landingPad
      working->second.mLandingPad = true;
      first = false;
    }
    currentFunction->mInstructions.insert(std::make_pair(addr,
        &(working->second)));
    if (working->second.mReturn == true) {
      /*s << std::hex << "<code type=\"end\">" << addr << "</code>";
       currentFunction->mBranchData.push_back(s.str()); s.str("");*/

      //info << "function return at: " << addr << std::endl;
      goto done;
    }
    addr += 4; //needs to be param
  } while (working->second.mBranch == false);
  addr -= 4;
  a = working->second.mAction.resolveToRegisterWithRegister(15, addr + 8);
#ifdef VERBOSE
  info << std::hex << "found next branch at " << addr << " to " << a << std::endl;
#endif
  if (working->second.mConditional) //if conditional, continue here as well
  {
    currentFunction->mBranchData.push_back(Branch(BRANCH_CONDITIONFAIL, addr,
        addr + 4));
    if (currentFunction->mInstructions.find(addr + 4)
        == currentFunction->mInstructions.end()) {
      //not in icache already
      fastAnalyse(addr + 4, currentFunction, true); //recurse..weeeee
    }
  }
  //for normal branches + conditionals

  //not in icache already
  //if linked, add it to the function cache
  if (working->second.mLinkedBranch == true) {
    fastAnalyse(addr + 4, currentFunction, false); //continue here, since it'll be back
    if (mBank->mem()->inFunction(a) == 0) //is function not done
    {
#ifdef VERBOSE
      info << "function found at: " << a << std::endl;
#endif
      fastAnalyse(a, mBank->mem()->addFunction(a), false); //and do the new function
    }
  } else {
    if (working->second.mConditional)
      currentFunction->mBranchData.push_back(Branch(BRANCH_CONDITIONPASS, addr,
          a));
    else {
      //branching from current addr to branch location
      currentFunction->mBranchData.push_back(Branch(BRANCH_FOLLOW, addr, a));
      //branching from branch location-4 to branch location, possibly
      //conditionals should have been done already and pushed
      /*impblue=currentFunction->mInstructions.find(a-4);
       //if instruction is found in function and it isn't a branch, add the implicit blue line
       if(impblue!=currentFunction->mInstructions.end() && impblue->second->mBranch==false)
       currentFunction->mBranchData.push_back(Branch(BRANCH_FOLLOW, a-4, a));*/
    }
    fastAnalyse(a, currentFunction, true);
  }
  done: mBank->unlock(LOCKED_CORE);
}

}
