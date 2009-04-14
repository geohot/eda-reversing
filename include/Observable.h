#ifndef OBSERVABLE_H_
#define OBSERVABLE_H_
#include <list>
#include "Observer.h"

namespace eda {

class Observable {
public:
  Observable() {}
  virtual ~Observable() {}

  virtual void attach(Observer* obs) {
    mObserverList.push_back(obs);
  }

  virtual void detatch(Observer* obs) {
    mObserverList.remove(obs);
  }

  virtual void notify() {
    std::list<Observer*>::iterator it;
    for(it = mObserverList.begin(); it != mObserverList.end(); ++it) {
      (*it)->update();
    }
  }

protected:
  std::list<Observer*> mObserverList;
};

}

#endif /* OBSERVABLE_H_ */
