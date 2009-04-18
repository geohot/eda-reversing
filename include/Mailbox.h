// Mailbox.h -- Apr 15, 2009
//    by geohot
//  released under GPLv3, see http://gplv3.fsf.org/

#ifndef EDA_MAILBOX_H_
#define EDA_MAILBOX_H_

//shoudl be cross platform

//Mailbox is threadsafe

#include <threading.h>

#include <queue>
#include <string>

namespace eda {

#define MAIL_NONE 0
#define MAIL_SERVER 1

class Mail
{
public:
  Mail()
  {

  }
  Mail(int lcmd, int lparam)
  {
    mCommand=lcmd;
    mParam=lparam;
  }
  int mCommand;
  int mParam;
};

class Mailbox
{
public:
  Mailbox();
  ~Mailbox();
  void sendMail(Mail mail);
  Mail waitForMail();    //dude, you're really gonna sit by the mailbox all day?
  Mail checkMailbox();   //NULL is no mail, else mail. i love mail
private:
  std::queue<Mail> mPostOffice;  //we only mail Mails, strings cost more
  mutexContainer mMutex;
  eventContainer mOMGMail;
};

}


#endif /* EDA_MAILBOX_H_ */
