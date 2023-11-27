#ifndef OBSERVER_H
#define OBSERVER_H

#include "util.h"

enum class SquareColor { Dark, Light };

class Observer {
  Board* subject; // ! NEWLY ADDED
  virtual void doNotify(Move& move, GameState state) = 0;
 public:
  // NVI - call doNotify
  void notify(Move& move, GameState state);
  virtual ~Observer() = default;
};

#endif

