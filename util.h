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

//! moved here since used in new Move fields
enum class PieceType
{
    King,
    Queen,
    Bishop,
    Rook,
    Knight,
    Pawn,
};
// Position
//? improve to class that returns move errors and have makemove() in board return that class
// ? instead of a Position

// 0 based coordinates ( eg. "a1" becomes (0, 0) )
//! (column, row) not the opposite
struct Position
{
    int col, row;

    Position() = default;
    Position(const int col, const int row);
    // ctor with a pos string (eg. "e4")
    Position(const string &pos);
};

bool operator==(const Position &p1, const Position &p2);

bool operator!=(const Position &p1, const Position &p2);

string posToStr(const Position &p);

// MOVE
struct Move
{
    Position startPos, endPos;
    //! [added] two fields for capture info
    bool captured = false;
    PieceType capturedPt;
    Position enPassentCapture;
    Move(); // ? good default value
    Move(const Position &startPos, const Position &endPos);
    Move(const Position &startPos, const Position &endPos, bool captured, PieceType pt);
    Move(const Position &startPos, const Position &endPos, bool captured, PieceType pt, const Position &epc);
    Move(const string &startPos, const string &endPos);
};

bool operator==(const Move &m1, const Move &m2);

//! ADDED
bool operator!=(const Move &m1, const Move &m2);

// Constants for sentinel Position values
extern const Position ILLEGAL_MOVE;
extern const Position INVALID_INPUT;

// Constants to store the size of the grid
extern const int NUM_COLS;
extern const int NUM_ROWS;


#endif
