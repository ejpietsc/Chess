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

enum class Colour { White, Black };

pair<int, int> strToCoords(const string &s)
{
    const int len = s.length();
    if (len != 2)
    {
        return make_pair(-1, -1);
    }

    const int col = s[0] - 'a';
    const int row = s[1] - '1';
    return make_pair(row, col);
}

// 0 based coordinates ( eg. "a1" becomes (0, 0) )
struct Position {
    int row, col;

    Position() = default;

    // ctor with coord ints
    Position(const int row, const int col)
    : row{row}, col{col} {}

    // ctor with a pos string (eg. "e4")
    Position(const string &pos) {
        pair<int, int> p = strToCoords(pos);
        this->row = p.first;
        this->col = p.second;
    }

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

#endif
