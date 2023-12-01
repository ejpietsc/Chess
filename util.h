// provides various structs, classes, and helper functions

#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <sstream>
#include <map>
#include <vector>
#include <string>
#include <memory>
#include <cstdlib> // ! added for abs()
/*
#include "board.h"
#include "observer.h"
#include "pieces.h"
#include "player.h"
#include "TextDisplay.h"
*/

using namespace std;

// Position
//? improve to class that returns move errors and have makemove() in board return that class
// ? instead of a Position
const Position illegal_move = Position{-2, -2};
const Position invalid_input = Position{-1, -1};

// Forward declarations
static pair<int, int> strToCoords(const string &s);

// 0 based coordinates ( eg. "a1" becomes (0, 0) )
//! (column, row) not the opposite
struct Position
{
    int col, row;

    Position() = default;

    // ctor with coord ints
    Position(const int col, const int row)
        : col{col}, row{row} {}

    // ctor with a pos string (eg. "e4")
    Position(const string &pos)
    {
        pair<int, int> p = strToCoords(pos);
        this->col = p.first;
        this->row = p.second;
    }
};

bool operator==(const Position &p1, const Position &p2)
{
    return (p1.col == p2.col && p1.row == p2.row);
}

bool operator!=(const Position &p1, const Position &p2)
{
    return (p1.col != p2.col || p1.row != p2.row);
}

// Helper functions
static pair<int, int> strToCoords(const string &s)
{
    const int len = s.length();
    if (len != 2 && !isValidSyntax(s[0], s[1]))
    {
        return make_pair(-1, -1);
    }
    else
    {
        return make_pair(s[0] - 'a', s[1] - '1');
    }
}

// MOVE
struct Move
{
    Position startPos, endPos;
    //! [added] two fields for capture info
    bool captured = false;
    PieceType capturedPt = PieceType::NULL_PIECE;
    //! [added] all following move ctors
    Move() : startPos{Position{0, 0}}, endPos{Position{0, 0}} {} // ? good default value
    Move(const Position &startPos, const Position &endPos) : startPos{startPos}, endPos{endPos} {}
    Move(const Position &startPos, const Position &endPos, bool captured, PieceType pt) : 
        startPos{startPos}, endPos{endPos}, captured{captured}, capturedPt{pt} {}
    Move(const string &startPos, const string &endPos) : startPos{Position(startPos)}, endPos{Position(endPos)} {}
};

bool operator==(const Move &m1, const Move &m2)
{
    return (m1.startPos == m2.startPos && m1.endPos == m2.endPos);
}

//! ADDED
bool operator!=(const Move &m1, const Move &m2)
{
    return (m1.startPos != m2.startPos || m1.endPos != m2.endPos);
}

bool isValidSyntax(char c1, char c2)
{
    return ('a' <= c1 && c1 <= 'h') && ('1' <= c2 && c2 <= '8');
}

#endif

