// provides various structs, classes, and helper functions

#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <sstream>
#include <map>
#include <vector>
#include <string>
#include <memory>
# include <cstdlib> // ! added for abs()
/*
#include "board.h"
#include "observer.h"
#include "pieces.h"
#include "player.h"
#include "TextDisplay.h"
*/

using namespace std;


// Forward declarations
static pair<int, int> strToCoords(const string &s);


// Structures and custom objects:

// 0 based coordinates ( eg. "a1" becomes (0, 0) )
//! (column, row) not the opposite
struct Position {
    int col, row;

    Position() = default;

    // ctor with coord ints
    Position(const int col, const int row)
    : col{col}, row{row} {}

    // ctor with a pos string (eg. "e4")
    Position(const string &pos) {
        pair<int, int> p = strToCoords(pos);
        this->col = p.first;
        this->row = p.second;
    }

    bool operator==(const Position &other) {
        return (this->row == other.row && this->col == other.col);
    }
};

struct Move {
    Position startPos, endPos;

    bool operator==(const Move &other) {
        return (this->startPos == other.startPos && this->endPos == other.endPos);
    }
};


// Helper functions

static pair<int, int> strToCoords(const string &s)
{
    const int len = s.length();
    if (len != 2) return make_pair(-1, -1);
    return make_pair(s[0] - 'a', s[1] - '1');
}

#endif
