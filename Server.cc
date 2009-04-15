// Server.cc -- Apr 14, 2009
//    by posixninja, geohot
//  released under GPLv3, see http://gplv3.fsf.org/

#include "Server.h"
#include "Core.h"

//i think this is what FrontEnd is now

namespace eda {

Server::Server(Bank* bank): mBank(bank) {
}

Server::~Server() {
}

void Server::update() {
}

}

/*void Server::attach(Core *a)
{
  mCore=a->mMail;
}*/
