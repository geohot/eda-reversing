// ParsedInstruction.cc -- Apr 13, 2009
//    by geohot

#include "ParsedInstruction.h"
#include <vector>
#include <string>

#include <iostream>

using namespace eda;

void ParsedInstruction::consolePrint()
{
  std::vector<std::pair<std::string, int> >::iterator walk=mString.begin();
  while(walk!=mString.end())
  {
    std::cout << walk->first;
    ++walk;
  }
  std::cout << std::endl;
}

void ParsedInstruction::add(std::string s, int dt)
{
  mString.push_back(std::make_pair(s,dt));
}
