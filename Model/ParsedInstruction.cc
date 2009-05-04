// ParsedInstruction.cc -- Apr 13, 2009
//    by geohot
//  released under GPLv3, see http://gplv3.fsf.org/

#include "ParsedInstruction.h"
#include "Memory.h"
#include <vector>
#include <string>

#include <iostream>
#include <sstream>

using namespace eda;

void ParsedInstruction::consolePrint() {
  std::vector<std::pair<std::string, int> >::iterator walk = mString.begin();
  while (walk != mString.end()) {
    if (walk->second == DT_IMMED)
      std::cout << "#";
    std::cout << walk->first;
    ++walk;
  }
  std::cout << std::endl;
}

/*
 #define DT_OPCODE 0
 #define DT_CONDITION 1
 #define DT_REGISTER 2
 #define DT_IMMED 3
 #define DT_FORMATTING 4
 #define DT_FLAG 5
 #define DT_SYMBOL 6
 #define DT_DECIMAL 7
 #define DT_SUBOPCODE 8
 #define DT_SIGNED 9
 #define DT_OFFSETADDRESS 10 //isn't
 #define DT_OFFSETDATA 11
 */

const char divLookup[12][30] = { "opcode", "flag", "register", "immed",
    "formatting", "flag", "symbol", "immed", "subopcode", "immed", "location",
    "immed" };

Data ParsedInstruction::stoi(std::string s)
//convert string to Data
{
  Data ret;
  std::istringstream ss(s);
  ss >> std::hex >> ret;
  return ret;
}

std::string ParsedInstruction::webPrint(Data address, Memory *mem)
//needs memory for things like pc dereffing and names
{
  std::stringstream ss;
  ss << std::hex << "<div class=\"instruction\" id=\"" << address << "\">";
  ss << "<span class=\"opblock\">";
  std::vector<std::pair<std::string, int> >::iterator walk = mString.begin();
  bool endedopblock = false;
  while (walk != mString.end()) {
    ss << "<span class=\"" << divLookup[walk->second] << "\">";
    if (walk->second == DT_OFFSETADDRESS) { //address is pc
      /*ss << "<span class=\"" << divLookup[walk->second] << "\" ondblclick=\"refreshFunction('" <<
       mem->getName(address+stoi(walk->first)) <<"')\">";*/
      ss << mem->getName(address + stoi(walk->first)) << "</span>"; //really should be namelookup
      break;
    }

    if (walk->second == DT_OFFSETDATA) {
      ss << "=0x";
      ss << (*mem)[(address + stoi(walk->first))][0] << "</span>"; //really should be namelookup
      break;
    }
    ss << walk->first;
    ss << "</span>";
    if (walk->second == DT_CONDITION) {
      ss << "</span>"; //end the opblock
      endedopblock = true;
    }
    ++walk;
  }
  if (endedopblock == false)
    ss << "</span>";
  ss << "</div>";
  return ss.str();
}

void ParsedInstruction::debugPrint() {
  std::vector<std::pair<std::string, int> >::iterator walk = mString.begin();
  while (walk != mString.end()) {
    switch (walk->second) {
    case DT_OPCODE:
      std::cout << "OPCODE    ";
      break;
    case DT_CONDITION:
      std::cout << "CONDITION ";
      break;
    case DT_REGISTER:
      std::cout << "REGISTER  ";
      break;
    case DT_IMMED:
      std::cout << "IMMEDIATE ";
      break;
    case DT_FORMATTING:
      std::cout << "FORMATTING";
      break;
    case DT_FLAG:
      std::cout << "FLAG      ";
      break;
    case DT_SYMBOL:
      std::cout << "SYMBOL    ";
      break;
    case DT_DECIMAL:
      std::cout << "DECIMAL   ";
      break;
    case DT_SUBOPCODE:
      std::cout << "SUBOPCODE ";
      break;
    case DT_SIGNED:
      std::cout << "SIGNED    ";
      break;
    }
    std::cout << ": '" << walk->first << "'" << std::endl;
    ++walk;
  }
}

void ParsedInstruction::add(std::pair<std::string, int> a) {
  mString.push_back(a);
}

void ParsedInstruction::add(std::string s, int dt) {
  mString.push_back(std::make_pair(s, dt));
}

void ParsedInstruction::add(Data a) {
  std::stringstream out;
  if (a >= 10)
    out << "0x";
  out << std::hex << a;
  mString.push_back(std::make_pair(out.str(), DT_IMMED));
}

void ParsedInstruction::add(Data a, int dt) {
  std::stringstream out;
  out << std::hex << (int) a;
  mString.push_back(std::make_pair(out.str(), dt));
}

ParsedInstruction& ParsedInstruction::operator<<(std::pair<std::string, int> a) {
  this->add(a);
  return *this;
}

ParsedInstruction& ParsedInstruction::operator<<(const char *a) {
  this->add((std::string) a, DT_FORMATTING);
  return *this;
}

ParsedInstruction& ParsedInstruction::operator<<(Data a) {
  this->add(a);
  return *this;
}

