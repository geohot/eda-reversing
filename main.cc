// Embedded Disassembler
//    by geohot, posixninja, westbaer, pumpkin, and planetbeing
//  released under GPLv3, see http://gplv3.fsf.org/

#include "Core.h"
using namespace eda;

int main(int argc, char* argv[])
{
  // create MVC objects
  Edb* edb = new Edb();
  Server* server = new Server(edb);
  Core* core = new Core(edb, server);

  // make indirect attachments
  edb->attach(server);
  server->attach(core);

  // run application
  core->test();

  // clean up
  delete core;
  delete server;
  delete edb;
}
