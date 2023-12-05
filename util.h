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

using namespace std;

enum class PieceType
{
    King,
    Queen,
    Bishop,
    Rook,
    Knight,
    Pawn,
};

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
    bool captured = false;
    PieceType capturedPt;
    bool enPassentCapture = false;
    Position epCaptureLoc;
    bool upgradePiece = false;
    PieceType upgradeTo;

    bool isCastleMove = false;

    Move();
    Move(const Position &startPos, const Position &endPos);
    Move(const string &startPos, const string &endPos);
    Move(const Position &startPos, const Position &endPos, bool isCastleMove);
    Move(const Position &startPos, const Position &endPos, bool captured, PieceType pt);
    Move(const Position &startPos, const Position &endPos, bool captured, PieceType pt, const Position &epc);
};

bool operator==(const Move &m1, const Move &m2);

bool operator!=(const Move &m1, const Move &m2);

// Constants for sentinel Position values
extern const Position ILLEGAL_MOVE;
extern const Position INVALID_INPUT;

// Constants to store the size of the grid
extern const int NUM_COLS;
extern const int NUM_ROWS;


#endif
