// provides piece classes

#ifndef PIECES_H
#define PIECES_H

#include "util.h"
#include "observer.h"
#include <string>
#include <vector>

using namespace std;

// provides the piece ABC
class Piece {
    vector<Observer *> observers; // textDisplay & GraphicsDisplay
    PieceType type;
    Colour colour;
    int x, y;
    
    public:
    // Ctor
    Piece(Colour col);

    // Copy ctor and assignment operator
    Piece(const Piece& other);
    Piece& operator=(const Piece& other);

    // Move ctor and assignment operator
    Piece(Piece&& other);
    Piece& operator=(Piece&& other);

    // Dtor
    virtual ~Piece();

    // Getters and setters
    PieceType getType() const;
    void setType(PieceType pt);

    void notifyAllObservers() const;
    virtual vector<Position> getMoves(Position p) = 0;
};

class King final : public Piece {

};

class Queen final : public Piece {

};

class Bishop final : public Piece {

};

class Rook final : public Piece {

};

class Knight final : public Piece {

};

class Pawn final : public Piece {

};

#endif
