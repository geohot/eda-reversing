#ifndef OBSERVER_H_
#define OBSERVER_H_

namespace eda {

class Observer {
public:
  Observer() {}
  virtual ~Observer() {}

  virtual void update() = 0;
};

}

#endif /* OBSERVER_H_ */
