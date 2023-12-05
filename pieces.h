// provides piece classes

#ifndef PIECES_H
#define PIECES_H

#include "util.h"

using namespace std;

enum class Colour
{
    White,
    Black
};

// Abstract Subject
class Piece
{
    virtual vector<Position> doGetMoves() const = 0;

protected:
    PieceType type;
    Colour colour;
    Position pos;

public:
    // ctor
    Piece(PieceType type, Colour colour, Position pos);

    /* checks: conform to piece move*/
    vector<Position> getMoves() const; // NVI - call doGetMoves

    void movePiece(const Position &landingPos);

    // Getters and setters
    PieceType getType() const;    // done
    Colour getColour() const;     // done
    Position getPosition() const; // done
    int getRow() const;           // done
    int getCol() const;           // done

    void setType(PieceType pt);
    void setColour(Colour clr);
    void setPosition(int c, int r);

    // Dtor
    virtual ~Piece();
};

class King final : public Piece
{
    vector<Position> doGetMoves() const override;

public:
    King(Colour colour, Position pos);
};

class Queen final : public Piece
{
    vector<Position> doGetMoves() const override;

public:
    Queen(Colour colour, Position pos);
};

class Bishop final : public Piece
{
    vector<Position> doGetMoves() const override;

public:
    Bishop(Colour colour, Position pos);
};

class Rook final : public Piece
{
    vector<Position> doGetMoves() const override;

public:
    Rook(Colour colour, Position pos);
};

class Knight final : public Piece
{
    vector<Position> doGetMoves() const override;

public:
    Knight(Colour colour, Position pos);
};

class Pawn final : public Piece
{
    bool hasMoved = false; // used to see if double move is allowed
    bool doubleMoved = false; // Used for en passent capture
    vector<Position> doGetMoves() const override;

public:
    Pawn(Colour colour, Position pos);
    bool getHasMoved() const;
    void setHasMoved(const bool b);
    bool getDoubleMoved() const;
    void setDoubleMoved(const bool b);
};

std::unique_ptr<Piece> createPiece(PieceType type, Colour colour, Position pos);

#endif

