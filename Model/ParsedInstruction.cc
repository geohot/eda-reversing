// ParsedInstruction.cc -- Apr 13, 2009
//    by geohot
//  released under GPLv3, see http://gplv3.fsf.org/

#include "ParsedInstruction.h"
#include <vector>
#include <string>

#include <iostream>
#include <sstream>

using namespace eda;

void ParsedInstruction::consolePrint()
{
  std::vector<std::pair<std::string, int> >::iterator walk=mString.begin();
  while(walk!=mString.end())
  {
    if(walk->second==DT_IMMED) std::cout << "#";
    std::cout << walk->first;
    ++walk;
  }
  std::cout << std::endl;
}

void ParsedInstruction::debugPrint()
{
  std::vector<std::pair<std::string, int> >::iterator walk=mString.begin();
  while(walk!=mString.end())
  {
    switch(walk->second)
    {
      case DT_OPCODE:     std::cout << "OPCODE    "; break;
      case DT_CONDITION:  std::cout << "CONDITION "; break;
      case DT_REGISTER:   std::cout << "REGISTER  "; break;
      case DT_IMMED:      std::cout << "IMMEDIATE "; break;
      case DT_FORMATTING: std::cout << "FORMATTING"; break;
      case DT_FLAG:       std::cout << "FLAG      "; break;
      case DT_SYMBOL:     std::cout << "SYMBOL    "; break;
      case DT_DECIMAL:    std::cout << "DECIMAL   "; break;
      case DT_SUBOPCODE:  std::cout << "SUBOPCODE "; break;
      case DT_SIGNED:     std::cout << "SIGNED    "; break;
    }
    std::cout << ": '" << walk->first << "'" << std::endl;
    ++walk;
  }
}

void ParsedInstruction::add(std::pair<std::string,int> a)
{
  mString.push_back(a);
}

void ParsedInstruction::add(std::string s, int dt)
{
  mString.push_back(std::make_pair(s,dt));
}

void ParsedInstruction::add(Data a)
{
  std::stringstream out;
  if(a>=10) out << "0x";
  out << std::hex << a;
  mString.push_back(std::make_pair(out.str(),DT_IMMED));
}

void ParsedInstruction::add(Data a, int dt)
{
  std::stringstream out;
  out << std::hex << (int)a;
  mString.push_back(std::make_pair(out.str(),dt));
}

ParsedInstruction& ParsedInstruction::operator<<(std::pair<std::string,int> a)
{
  this->add(a);
  return *this;
}

ParsedInstruction& ParsedInstruction::operator<<(const char *a)
{
  this->add((std::string)a, DT_FORMATTING);
  return *this;
}

ParsedInstruction& ParsedInstruction::operator<<(Data a)
{
  this->add(a);
  return *this;
}

