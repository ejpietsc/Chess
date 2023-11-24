#ifndef OBSERVER_H
#define OBSERVER_H
#include "util.h"

class Observer {
 public:
  virtual void notify(Move& move, GameState state) = 0;
  virtual ~Observer() = default;
};

#endif
