// provides various structs, classes, and helper functions
#include "util.h"

using namespace std;

const Position illegal_move = Position{-2, -2};
const Position invalid_input = Position{-1, -1};

// Constants to store the size of the grid
const int NUM_COLS = 8;
const int NUM_ROWS = 8;


// Helper functions
bool isValidSyntax(char c1, char c2)
{
    return ('a' <= c1 && c1 <= 'h') && ('1' <= c2 && c2 <= '8');
}

static pair<int, int> strToCoords(const string &s)
{
    const int len = s.length();
    if (len != 2 && !isValidSyntax(s[0], s[1]))
    {
        return make_pair(-1, -1);
    }
    else
    {
        //! fix
        return make_pair(s[0] - 'a', s[1] - '1');
    }
}

Position::Position(const int col, const int row) : col{col}, row{row} {}

// ctor with a pos string (eg. "e4")
Position::Position(const string &pos)
{
    pair<int, int> p = strToCoords(pos);
    this->col = p.first;
    this->row = p.second;
}

bool operator==(const Position &p1, const Position &p2)
{
    return (p1.col == p2.col && p1.row == p2.row);
}

bool operator!=(const Position &p1, const Position &p2)
{
    return (p1.col != p2.col || p1.row != p2.row);
}

Move::Move() : startPos{Position{0, 0}}, endPos{Position{0, 0}} {} // ? good default value
Move::Move(const Position &startPos, const Position &endPos) : startPos{startPos}, endPos{endPos} {}
Move::Move(const Position &startPos, const Position &endPos, bool captured, PieceType pt) : startPos{startPos}, endPos{endPos}, captured{captured}, capturedPt{pt} {}
Move::Move(const string &startPos, const string &endPos) : startPos{Position(startPos)}, endPos{Position(endPos)} {}

bool operator==(const Move &m1, const Move &m2)
{
    return (m1.startPos == m2.startPos && m1.endPos == m2.endPos);
}

//! ADDED
bool operator!=(const Move &m1, const Move &m2)
{
    return (m1.startPos != m2.startPos || m1.endPos != m2.endPos);
}
