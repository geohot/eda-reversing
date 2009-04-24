// Commands.cc -- Apr 22, 2009
//    by geohot
//  released under GPLv3, see http://gplv3.fsf.org/

#include "Bank.h"
#include "Commands.h"
#include "macros.h"

namespace eda {

int Commands::hexstrtoint(std::string in) {
  int ret;
  std::stringstream ss(in);
  ss >> std::hex >> ret;
  return ret;
}

//get a list of functions and return it to the server
//returns xml list of all functions
bool Commands::getFunctionList(COMMAND_PARAMS) {
  mBank->lock(LOCKED_SERVER);
  response << XML_HEADER << "<top>" << std::endl;

  FunctionIterator walk = mBank->mem()->mFunctionStore.begin();
  while (walk != mBank->mem()->mFunctionStore.end()) {
    response << std::hex << "<function address=\"" << walk->first << "\">"
        << mBank->mem()->getName(walk->first) << "</function>" << std::endl;
    ++walk;
  }
  response << "</top>" << std::endl;
}

//get all instructions in function
//returns HTML data in the stringstream
bool Commands::getFunction(COMMAND_PARAMS) {
  if(argv.size()<3) return false;
  mBank->lock(LOCKED_SERVER);

  Data addr;

  if (mBank->mem()->lookupName(argv[2], &addr))
  {
    Function *f = mBank->mem()->inFunction(addr);
    std::map<Data, Instruction *>::iterator walk = f->mInstructions.begin();
    response << "<html><div class=\"codebox\" id=\"" << mBank->mem()->getName(
        walk->first) << "\">" << std::endl;

    //next line should really get name
    response << std::hex << "<div class=\"addr\">" << mBank->mem()->getName(
        walk->first) << "</div>" << std::endl;

    while (walk != f->mInstructions.end()) {
      if (walk->second->mLandingPad == true) {
        response << "</div>" << std::endl << "<div class=\"codebox\" id=\""
            << mBank->mem()->getName(walk->first) << "\">" << std::endl;
        response << std::hex << "<div class=\"addr\">" << mBank->mem()->getName(
            walk->first) << "</div>" << std::endl;
      }

      response << walk->second->mString.webPrint(walk->first, mBank->mem());
      ++walk;
    }
    response << "</div></html>";
  }

  mBank->unlock(LOCKED_SERVER);
}

//get the branch data
//returns XML file with branch data
bool Commands::getFunctionBranchData(COMMAND_PARAMS) {
  if(argv.size()<3) return false;
  mBank->lock(LOCKED_SERVER);

  Data addr;
  if (mBank->mem()->lookupName(argv[2], &addr)) {
    response << XML_HEADER << "<top>" << std::endl;
    Function *f = mBank->mem()->inFunction(addr);
    std::vector<Branch>::iterator walk = f->mBranchData.begin();
    while (walk != f->mBranchData.end()) {
      response << (*walk).getXML() << std::endl;
      ++walk;
    }
    response << "</top>";
  }
  mBank->unlock(LOCKED_SERVER);
}

//rename specified function
bool Commands::rename(COMMAND_PARAMS) {
  if(argv.size()<4) return false;
  mBank->lock(LOCKED_SERVER);
  Data addr;
  if (mBank->mem()->lookupName(argv[2], &addr)) {
    mBank->mem()->setName(addr, argv[3]);
  }
  else if(argv[2].compare(0,4,"loc_")==0) {  //unnamed
    mBank->mem()->setName(hexstrtoint(argv[2].substr(4))
        , argv[3]);
  }
  mBank->unlock(LOCKED_SERVER);
}

}
