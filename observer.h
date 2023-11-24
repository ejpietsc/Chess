#ifndef OBSERVER_H
#define OBSERVER_H


class Observer {
 public:
  virtual void notify() = 0; //! todo
  virtual ~Observer() = default;
};

#endif
