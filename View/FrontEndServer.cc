// FrontEndServer.cc -- Apr 15, 2009
//    by geohot
//  released under GPLv3, see http://gplv3.fsf.org/

#include "FrontEndServer.h"


#ifdef WIN32            //Windows sockets
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#define SHUT_RDWR SD_SEND
#define socklen_t int

#else                   //Linux sockets
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#endif

#include "macros.h"
#include <string>
#include <vector>
#include <sstream>

namespace eda {

int FrontEndServer::hexstrtoint(std::string in)
{
  int ret;
  std::stringstream ss(in);
  ss >> std::hex >> ret;
  return ret;
}

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

bool FrontEndServer::serveFile(int fd, const char *name, const char *contentType)
{
  //assume it's in the data folder
  char buf[1024];
  strcpy(buf, DATA_PATH);
  strncat(buf, name, 1024-strlen(buf));
  FILE *f=fopen(buf, "rb");
  if(f==0) {
    debug << "file not found: " << buf << std::endl;
    return false;
  }
  serveHeaders(fd,contentType);
  int l;
  while( (l=fread(buf, 1, 1024, f)) > 0)
  {
    send(fd, buf, l, 0);
  }
  //info << "sent file " << name << std::endl;
  return true;
}

void FrontEndServer::serveHeaders(int fd,const char *contentType)
{
  char buf[100];
  strcpy(buf, "HTTP/1.1 200 OK\n");
  strcat(buf, "Content-Type: ");
  strcat(buf, contentType);
  strcat(buf,"\n\n");

  send(fd, buf, strlen(buf), 0);
}

void FrontEndServer::serve(int fd)
//pass in file descriptor of the connected client
//sorry my server is so Cish
{
  char buf[1024];
  int l=recv(fd, buf, 1024, 0);
//eventually we'll have to deal with POST
  buf[l]='\0';
  *strchr(buf, '\n')=0;

//point to the request URL
  char *requestURL=strchr(buf, ' ')+1;
  *strchr(requestURL, ' ')=0;

  info << "request: " << requestURL << std::endl;

  if(strcmp(requestURL,"/test")==0)
  {
    serveHeaders(fd,"text/html");
    strcpy(buf, "<pre>hello world</pre>");
    send(fd, buf, strlen(buf), 0);
  }
  else if(strcmp(requestURL,"/test.xml")==0)
  {
    serveHeaders(fd, "application/xml");
    send(fd, XML_HEADER, strlen(XML_HEADER), 0);
    send(fd, "<top><test>hello</test></top>", strlen("<top><test>hello</test></top>"), 0);
  }
  else if(strcmp(requestURL,"/")==0) serveFile(fd,"eda_template.html","text/html");
  else if(strcmp(requestURL,"/favicon.ico")==0) serveFile(fd,"favicon.ico","image/x-icon");
  else if(strcmp(requestURL,"/eda.css")==0) serveFile(fd,"eda.css","text/css");
  else if(strcmp(requestURL,"/eda.js")==0) serveFile(fd,"eda.js","application/x-javascript");
  else if(!lexer(fd,requestURL)) info << "unhandled URL" << std::endl;
}

#define sendString(x,y) send(x, y, strlen(y), 0)

bool FrontEndServer::lexer(int fd,std::string cmd)
{
  std::vector<std::string> argv;
  size_t start=cmd.find_first_not_of("/",0);
  size_t end=cmd.find_first_of("/", start);
  while(end!=std::string::npos || start!=std::string::npos)
  {
    argv.push_back(cmd.substr(start,end-start));
    start=cmd.find_first_not_of("/",end);
    end=cmd.find_first_of("/", start);
  }
  /*info << "lexed " << argv.size() << std::endl;
  for(int a=0;a<argv.size();a++) info << "[" << a << "] " << argv[a] << std::endl;*/
  if(argv[0]=="Bank" && argv.size()>=2) {
    mBank->lock(LOCKED_SERVER);
    if(argv[1]=="getFunctionList") {
      serveHeaders(fd, "application/xml");
      std::stringstream response;
      response << XML_HEADER << "<top>" << std::endl;

      FunctionIterator walk=mBank->mem()->mFunctionStore.begin();
      while(walk!=mBank->mem()->mFunctionStore.end()) {
        response << std::hex << "<function address=\""
          << walk->first << "\">" << mBank->mem()->getName(walk->first) << "</function>" << std::endl;
        ++walk;
      }

      response << "</top>" << std::endl;
      sendString(fd, response.str().c_str());
    }
    else if(argv[1]=="getFunction" && argv.size()>=3)
    {
      serveHeaders(fd, "application/xml");

      std::stringstream response;
      //response << XML_HEADER << "<top><instructiondata>" << std::endl;

      Data addr;
      if(!mBank->mem()->lookupName(argv[2], &addr))
        goto fail;

      Function *f=mBank->mem()->inFunction(addr);
      std::map<Data,Instruction *>::iterator walk=f->mInstructions.begin();
      response << "<html><div class=\"codebox\" id=\"" <<
        mBank->mem()->getName(walk->first) << "\">" << std::endl;

      //next line should really get name
      response << std::hex << "<div class=\"addr\">" <<
        mBank->mem()->getName(walk->first) << "</div>" << std::endl;

      while(walk!=f->mInstructions.end()) {
        if(walk->second->mLandingPad==true) {
          response << "</div>" << std::endl << "<div class=\"codebox\" id=\"" <<
            mBank->mem()->getName(walk->first) << "\">" << std::endl;
          response << std::hex << "<div class=\"addr\">" <<
            mBank->mem()->getName(walk->first) << "</div>" << std::endl;
        }
        response << walk->second->mString.webPrint(walk->first, mBank->mem());
        ++walk;
      }
      response << "</div></html>";
      /*response << "</instructiondata>" << std::endl;
      response << "</top>" << std::endl;*/
      sendString(fd, response.str().c_str());
    }
    else if(argv[1]=="getFunctionBranchData" && argv.size()>=3) {
      serveHeaders(fd, "application/xml");
      std::stringstream response;
      response << XML_HEADER << "<top>" << std::endl;

      Data addr;
      if(!mBank->mem()->lookupName(argv[2], &addr))
        goto fail;

      Function *f=mBank->mem()->inFunction(addr);
      std::vector<Branch>::iterator walk=f->mBranchData.begin();
      while(walk!=f->mBranchData.end()) {
        response << (*walk).getXML() << std::endl;
        ++walk;
      }
      response << "</top>";
      sendString(fd, response.str().c_str());
    }
    else if(argv[1]=="rename" && argv.size()>=4) {
      Data addr;
      if(mBank->mem()->lookupName(argv[2], &addr)) {
        mBank->mem()->setName(addr, argv[3]);
      }
    }
fail:
    mBank->unlock(LOCKED_SERVER);
  }

  return true;
}

void FrontEndServer::runLoop()
{
  mBank->lock(LOCKED_SERVER);
  mBank->mem()->loadFile("bootrom",0x400000);
  mBank->mem()->importIDC("bootrom.idc");
  mBank->mem()->loadFile("eda_test.bin",0);

  mBank->unlock(LOCKED_SERVER);
  mCore->sendMail(Mail(CORE_ANALYSE,0));
  mCore->sendMail(Mail(CORE_ANALYSE,0x400054));


  if(!serverListen()) { delete this; return; }  //works?
  else info << "server started" << std::endl;

  struct sockaddr_in cli_addr;
//this server will be blocking for easiness
  while(1)
  {
    int clientSocket;
    socklen_t sin_size=sizeof(cli_addr);

    if ((clientSocket = accept(mSocket, (struct sockaddr *)&cli_addr, &sin_size)) == -1) {
      debug << "accept failed" << std::endl;
    }


    //if this were threaded, a new thread would go here
    serve(clientSocket); //do shit

    //shutdown(clientSocket, SD_SEND);
    shutdown(clientSocket, SHUT_RDWR);
  }
}

}
