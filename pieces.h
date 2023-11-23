// provides piece classes

#ifndef PIECES_H
#define PIECES_H

#include <string>

using namespace std;

enum class PieceType { EMPTY, King, Queen, Bishop, Rook, Knight, Pawn };

// provides the piece ABC
class Piece {
    PieceType type = PieceType::EMPTY;
    bool isWhite; // true if piece is white

    public:
    // ctor
    Piece(const bool isWhite);

    // dtor
    virtual ~Piece() = 0;

    // return isWhite bool
    bool pieceIsWhite() const;

    PieceType getType() const;

    void setType(PieceType pt);

    // attempt to move piece to pos (eg. "e4"), return true if success
    //  (UNIMPLEMENTED)
    virtual bool move(string pos);

    // attempt to capture piece at pos (eg. "e4"), return true if success
    //  (UNIMPLEMENTED)
    virtual bool capture(string pos);

};

class EmptyPiece : public Piece {
    public:
    EmptyPiece();
};

class King final : public Piece {
    public:
    King(const bool isWhite);
};

class Queen final : public Piece {
    public:
    Queen(const bool isWhite);
};

class Bishop final : public Piece {
    public:
    Bishop(const bool isWhite);
};

class Rook final : public Piece {
    public:
    Rook(const bool isWhite);
};

class Knight final : public Piece {
    public:
    Knight(const bool isWhite);
};

class Pawn final : public Piece {
    public:
    Pawn(const bool isWhite);
};

// ... more piece classes

ostream &operator<<(ostream &out, const Piece &p); // output p's char icon

#endif
