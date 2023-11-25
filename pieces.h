// provides piece classes

#ifndef PIECES_H
#define PIECES_H

#include "util.h"

using namespace std;

// ! added NULL_PIECE for return purposes in pieceStringToPieceType() function in main.cc
enum class PieceType { King, Queen, Bishop, Rook, Knight, Pawn, NULL_PIECE };

// Abstract Subject
class Piece
{
    virtual vector<Position> doGetMoves(Piece &p) = 0;

protected:
    vector<Observer *> observers; // textDisplay & GraphicsDisplay
    PieceType type;               //! may need to be in concretePieces
    Colour colour;
    Position pos;

public:
    // Ctor
    Piece(PieceType type, Colour colour, Position pos);

    /* checks:
    1. conform to piece move  2. vacant or has enemy */
    vector<Position> getMoves(Piece &p); // NVI - call doGetMoves

    void makeMove(Position &landingPos);

    // Observer Pattern methods
    void notifyObservers() const;
    void attach(Observer *o);

    // Getters and setters
    PieceType getType() const;
    Colour getColour() const;
    int getRow() const;
    int getCol() const;

    void setType(PieceType pt);
    void setColour(Colour clr);
    void setPosition(int r, int c);
    // void setRow() const;
    // void setCol() const;

    // Dtor
    virtual ~Piece();

    /* BIG 5 - Not needed given current implementation _______________________
    !IF NEEDED LATER MAKE PROTECTED
    Piece(const Piece& other);
    Piece& operator=(const Piece& other);
    Piece(Piece&& other);
    Piece& operator=(Piece&& other); */
};

class King final : public Piece
{
    vector<Position> doGetMoves(Piece &p) override;
public:
    King(Colour colour, int x, int y) : Piece{PieceType::King, colour, Position{x, y}} {}
};

class Queen final : public Piece
{
    vector<Position> doGetMoves(Piece &p) override;
public:
    Queen(Colour colour, int x, int y) : Piece{PieceType::Queen, colour, Position{x, y}} {}
};

class Bishop final : public Piece
{
    vector<Position> doGetMoves(Piece &p) override;
public:
    Bishop(Colour colour, int x, int y) : Piece{PieceType::Bishop, colour, Position{x, y}} {}
};

class Rook final : public Piece
{
    vector<Position> doGetMoves(Piece &p) override;
public:
    Rook(Colour colour, int x, int y) : Piece{PieceType::Rook, colour, Position{x, y}} {}
};

class Knight final : public Piece
{
    vector<Position> doGetMoves(Piece &p) override;
public:
    Knight(Colour colour, int x, int y) : Piece{PieceType::Knight, colour, Position{x, y}} {}
};

class Pawn final : public Piece
{
    vector<Position> doGetMoves(Piece &p) override;
public:
    Pawn(Colour colour, int x, int y) : Piece{PieceType::Pawn, colour, Position{x, y}} {}
};

#endif
