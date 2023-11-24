// provides various structs, classes, and helper functions

#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <memory>
#include "board.h"
#include "observer.h"
#include "pieces.h"
#include "player.h"

using namespace std;

enum class Colour { White, Black };

struct Position {
    int row, col;
};

struct Move {
    Position startPos, endPos;
    bool captured;
};

#endif
