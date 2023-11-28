#ifndef OBSERVER_H
#define OBSERVER_H

#include "util.h"
#include "board.h"

enum class SquareColor { Dark, Light };

// forward declarations, observers includes board, board includes observer
class Board;
class Move;
enum class GameState;

class Observer {
  Board* subject; // ! NEWLY ADDED
  virtual void doNotify(Move& move, GameState state) = 0;
 public:
  // NVI - call doNotify
  void notify(Move& move, GameState state);
  virtual ~Observer() = default;
};

#endif

