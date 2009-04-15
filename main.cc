// Embedded Disassembler
//    by geohot, posixninja, westbaer, pumpkin, and planetbeing
//  released under GPLv3, see http://gplv3.fsf.org/

#include "ConsoleFrontEnd.h"
#include "Core.h"
using namespace eda;

int main(int argc, char* argv[])
{
  // create MVC objects
  Bank* bank = new Bank();
  Core* core = new Core(bank);
  //Server* server = new Server(bank);
  ConsoleFrontEnd* server = new ConsoleFrontEnd(bank);
  //ConsoleFrontEnd has the same interface as the server,
  //  except it serves to one user on the console


  // make indirect attachments
  //bank->attach(server);
  server->attach(core);

  // run application
  core->runLoop();      //this thread hangs out in the Core

  // clean up
  delete core;
  delete server;
  delete bank;
}
