// FrontEndServer.cc -- Apr 15, 2009
//    by geohot
//  released under GPLv3, see http://gplv3.fsf.org/

#include "FrontEndServer.h"

//Windows only for now
#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#endif

#include "macros.h"

namespace eda {

bool FrontEndServer::serverListen()
//open the server on port 80
{
#ifdef WIN32                //winsock sucks
  WSADATA wsaData;
  WSAStartup(0x0101, &wsaData);
#endif
  mSocket=socket(AF_INET, SOCK_STREAM, 0);

  struct sockaddr_in serv_addr;
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family      = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port        = htons(80);

  if(bind(mSocket, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
  {
    debug << "bind failed" << std::endl;
    return false;
  }

  if (listen(mSocket, SOMAXCONN) == -1) {
    debug << "listen failed" << std::endl;
    return false;
  }

  return true;

}

bool FrontEndServer::serveFile(int fd, const char *name)
{
  FILE *f=fopen(name, "rb");
  if(f==0) {
    debug << "file not found: " << name << std::endl;
    return false;
  }
  char buf[1024];
  serveHeaders(fd);
  int l;
  while( (l=fread(buf, 1, 1024, f)) > 0)
  {
    send(fd, buf, l, 0);
  }
  info << "sent file " << name << std::endl;
  return true;
}

void FrontEndServer::serveHeaders(int fd)
{
  char buf[100];
  strcpy(buf, "HTTP/1.1 200 OK\n\n");
  send(fd, buf, strlen(buf), 0);
}

void FrontEndServer::serve(int fd)
//pass in file descriptor of the connected client
//sorry my server is so Cish
{
  char buf[1024];
  int l=recv(fd, buf, 1024, 0);
  buf[l]='\0';
  info << "got: " << std::endl << buf;

  serveHeaders(fd);

  strcpy(buf, "<pre>hello world</pre>");
  send(fd, buf, strlen(buf), 0);

}

void FrontEndServer::runLoop()
{
  if(!serverListen()) { delete this; return; }  //works?
  else info << "server started" << std::endl;

  struct sockaddr_in cli_addr;
//this server will be blocking for easiness
  while(1)
  {
    int clientSocket, sin_size=sizeof(cli_addr);

    if ((clientSocket = accept(mSocket, (struct sockaddr *)&cli_addr, &sin_size)) == -1) {
      debug << "accept failed" << std::endl;
    }

    //if this were threaded, a new thread would go here
    serve(clientSocket); //do shit

    shutdown(clientSocket, SD_SEND);
  }
}

}
