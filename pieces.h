// provides piece classes

#ifndef PIECES_H
#define PIECES_H

#include "util.h"

using namespace std;

enum class PieceType { King, Queen, Bishop, Rook, Knight, Pawn };

// Abstract Subject
class Piece
{
protected:
    vector<Observer *> observers; // textDisplay & GraphicsDisplay
    PieceType type;               //! may need to be in concretePieces
    Colour colour;
    Position pos;

    virtual vector<Position> doGetMoves(Piece &p) = 0;

public:
    // Ctor
    Piece(PieceType type, Colour colour, Position pos);

    /* checks:
    1. conform to piece move  2. vacant or has enemy */
    vector<Position> getMoves(Piece &p); // NVI - doGetMoves

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
public:
    King(Colour colour, int x, int y) : Piece{PieceType::King, colour, Position{x, y}} {}
    vector<Position> doGetMoves(Piece &p) override;
};

class Queen final : public Piece
{
public:
    Queen(Colour colour, int x, int y) : Piece{PieceType::Queen, colour, Position{x, y}} {}
    vector<Position> doGetMoves(Piece &p) override;
};

class Bishop final : public Piece
{
public:
    Bishop(Colour colour, int x, int y) : Piece{PieceType::Bishop, colour, Position{x, y}} {}
    vector<Position> doGetMoves(Piece &p) override;
};

class Rook final : public Piece
{
public:
    Rook(Colour colour, int x, int y) : Piece{PieceType::Rook, colour, Position{x, y}} {}
    vector<Position> doGetMoves(Piece &p) override;
};

class Knight final : public Piece
{
public:
    Knight(Colour colour, int x, int y) : Piece{PieceType::Knight, colour, Position{x, y}} {}
    vector<Position> doGetMoves(Piece &p) override;
};

class Pawn final : public Piece
{
public:
    Pawn(Colour colour, int x, int y) : Piece{PieceType::Pawn, colour, Position{x, y}} {}
    vector<Position> doGetMoves(Piece &p) override;
};

#endif
