// provides various structs, classes, and helper functions

#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <sstream>
#include <map>
#include <vector>
#include <string>
#include <memory>
#include "board.h"
#include "observer.h"
#include "pieces.h"
#include "player.h"
#include "TextDisplay.h"

using namespace std;

struct Move; // forward-declaration

enum class Colour { White, Black };

struct Position {
    int row, col;

    bool operator==(const Position &other) {
        return (this->row == other.row && this->col == other.col);
    }
};

struct Move {
    Position startPos, endPos;
    bool captured;

    bool operator==(const Move &other) {
        return (this->startPos == other.startPos && 
        this->endPos == other.endPos && this->captured == other.captured);
    }
};

// if a player resigns, return this move
const extern Move RESIGNATION_MOVE = Move{{-1, -1}, {-1, -1}, false};

#endif
