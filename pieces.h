// provides piece classes

#ifndef PIECES
#define PIECES

#include <string>

using namespace std;

// provides the piece ABC
class Piece {
    char icon; // char icon (i.e P for white pawn)
    bool isWhite; // true if piece is white

    public:
    // ctor
    Piece(const char icon, const bool isWhite);

    // dtor
    virtual ~Piece() = 0;

    // return isWhite bool
    bool pieceIsWhite() const;

    // set piece's icon to c
    void setPieceIcon(const char c);

    // return piece's icon
    char getPieceIcon() const;

    // attempt to move piece to pos (eg. "e4"), return true if success
    //  (UNIMPLEMENTED)
    virtual bool move(string pos);

    // attempt to capture piece at pos (eg. "e4"), return true if success
    //  (UNIMPLEMENTED)
    virtual bool capture(string pos);

};

class Pawn final : public Piece {
    public:
    Pawn(const bool isWhite);
};

class Rook final : public Piece {
    public:
    Rook(const bool isWhite);
};

// ... more piece classes

ostream &operator<<(ostream &out, const Piece &p); // output p's char icon

#endif
