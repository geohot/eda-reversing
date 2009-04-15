// Embedded Disassembler
//    by geohot, posixninja, westbaer, pumpkin, and planetbeing
//  released under GPLv3, see http://gplv3.fsf.org/

#include "Core.h"
using namespace eda;

int main(int argc, char* argv[])
{
  // create MVC objects
  Bank* bank = new Bank();
  Server* server = new Server(bank);
  Core* core = new Core(bank, server);

  // make indirect attachments
  bank->attach(server);
  server->attach(core);

  // run application
  core->test();

  // clean up
  delete core;
  delete server;
  delete bank;
}
