#include "pieces.h"

using namespace std;

Piece::Piece(PieceType type, Colour colour, Position pos)
    : type(type), colour{colour}, pos{pos} {}

// ! added
std::unique_ptr<Piece> createPiece(PieceType type, Colour colour, Position pos)
{
    switch (type)
    {
    case PieceType::King:
        return std::make_unique<King>(colour, pos);
    case PieceType::Queen:
        return std::make_unique<Queen>(colour, pos);
    case PieceType::Bishop:
        return std::make_unique<Bishop>(colour, pos);
    case PieceType::Rook:
        return std::make_unique<Rook>(colour, pos);
    case PieceType::Knight:
        return std::make_unique<Knight>(colour, pos);
    case PieceType::Pawn:
        return std::make_unique<Pawn>(colour, pos);
    default:
        return nullptr; // Handle NULL_PIECE - empty tile
    }
}

vector<Position> Piece::getMoves() const
{
    return this->doGetMoves();
}

void Piece::setPosition(int c, int r)
{
    pos.col = c;
    pos.row = r;
}

PieceType Piece::getType() const
{
    return type;
}

Colour Piece::getColour() const
{
    return colour;
}

int Piece::getRow() const
{
    return pos.row;
}

int Piece::getCol() const
{
    return pos.col;
}

int Piece::getRow() const
{
    return pos.row;
}

int Piece::getCol() const
{
    return pos.col;
}

// === PIECE SUBCLASSES ===

// return true if newP is a valid move (given no other piece info)
static bool isValidPos(const Position &currP, const Position &newP)
{
    if (newP.row == currP.row && newP.col == currP.col)
    { // same position as curr position
        return false;
    }
    else if (newP.row < 0 || newP.row >= 8 || newP.col < 0 || newP.col >= 8)
    {
        return false;
    }

    return true;
}

static void addPosToVec(const Position &currP, const Position &newP, vector<Position> &vec)
{
    if (isValidPos(currP, newP))
    {
        vec.reserve(1);
        vec.emplace_back(newP);
    }
}

vector<Position> King::doGetMoves() const
{
    vector<Position> vec;
    const int col = this->pos.col;
    const int row = this->pos.row;

    // 1 around
    for (int i = -1; i <= 1; ++i)
    {
        for (int j = -1; j <= 1; ++j)
        {
            Position p{col + j, row + i};
            addPosToVec(this->pos, p, vec);
        }
    }

    return vec;
}

vector<Position> Queen::doGetMoves() const
{
    vector<Position> vec;
    const int col = this->pos.col;
    const int row = this->pos.row;

    // entire row
    for (int i = 0 - row; i <= 7 - row; ++i)
    {
        Position p{col, row + i};
        addPosToVec(this->pos, p, vec);
    }

    // entire col
    for (int i = 0 - col; i <= 7 - col; ++i)
    {
        Position p{col + i, row};
        addPosToVec(this->pos, p, vec);
    }

    // diagonals
    for (int i = 0 - col; i <= 7 - col; ++i)
    {
        Position p1{col + i, row + i};
        Position p2{col + i, row - i};
        addPosToVec(this->pos, p1, vec);
        addPosToVec(this->pos, p2, vec);
    }
    return vec;
}

vector<Position> Rook::doGetMoves() const
{
    vector<Position> vec;
    const int col = this->pos.col;
    const int row = this->pos.row;

    // entire row
    for (int i = 0 - row; i <= 7 - row; ++i)
    {
        Position p{col, row + i};
        addPosToVec(this->pos, p, vec);
    }

    // entire col
    for (int i = 0 - col; i <= 7 - col; ++i)
    {
        Position p{col + i, row};
        addPosToVec(this->pos, p, vec);
    }

    return vec;
}

vector<Position> Knight::doGetMoves() const
{
    vector<Position> vec;
    const int col = this->pos.col;
    const int row = this->pos.row;

    // 2 in one direction, 1 in the other
    Position arr[8] = {
        {col + 2, row + 1},
        {col + 2, row - 1},
        {col - 2, row + 1},
        {col - 2, row - 1},

        {col + 1, row + 2},
        {col + 1, row - 2},
        {col - 1, row + 2},
        {col - 1, row - 2},
    };

    for (int i = 0; i < 8; ++i)
    {
        addPosToVec(this->pos, arr[i], vec);
    }

    return vec;
}

vector<Position> Bishop::doGetMoves() const
{
    vector<Position> vec;
    const int col = this->pos.col;
    const int row = this->pos.row;

    // diagonals
    for (int i = 0 - col; i <= 7 - col; ++i)
    {
        Position p1{col + i, row + i};
        Position p2{col + i, row - i};
        addPosToVec(this->pos, p1, vec);
        addPosToVec(this->pos, p2, vec);
    }

    return vec;
}

// ! note: pawn generates moves as if it can capture and/or move 2 spaces
vector<Position> Pawn::doGetMoves() const
{
    vector<Position> vec;
    const int col = this->pos.col;
    const int row = this->pos.row;

    int directionMult = 1;

    if (this->colour == Colour::Black)
    { // change dir for black
        directionMult = -1;
    }

    Position arr[4] = {
        {col, row + (1 * directionMult)},
        {col, row + (2 * directionMult)},     // 2 infront
        {col + 1, row + (1 * directionMult)}, // right capture
        {col - 1, row + (1 * directionMult)}, // left capture
    };

    for (int i = 0; i < 4; ++i)
    {
        addPosToVec(this->pos, arr[i], vec);
    }

    return vec;
}
