#ifndef OBSERVER_H
#define OBSERVER_H

#include "util.h"

class Observer {
  virtual void doNotify(Move& move, GameState state) = 0;
 public:
  // NVI - call doNotify
  void notify(Move& move, GameState state);
  virtual ~Observer() = default;
};

#endif
