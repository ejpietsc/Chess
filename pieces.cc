#include "pieces.h"

const int CAPS_DIFF = 'a' - 'A'; // difference between a chars capital and lowercase versions

Piece::Piece(const char icon, const bool isWhite)
: icon{icon}, isWhite{isWhite} {}

Piece::~Piece() {}

bool Piece::pieceIsWhite() const {
    return this->isWhite;
}

void Piece::setPieceIcon(const char c) {
    this->icon = c;
}

char Piece::getPieceIcon() const {
    return this->icon;
}

// *** unimplemented
bool Piece::move(string pos) {
    return true;
}

// *** unimplemented
bool Piece::capture(string pos) {
    return true;
}

// update a piece icon to caps if it is a white piece, otherwise do nothing
static void modifyPieceIconForColor(Piece *p) {
    if (p->pieceIsWhite()) { // make white icons capital letters
        p->setPieceIcon(p->getPieceIcon() - CAPS_DIFF);
    }
}

Pawn::Pawn(const bool isWhite)
: Piece{'p', isWhite} 
{
    modifyPieceIconForColor(this);
}

Rook::Rook(const bool isWhite)
: Piece{'r', isWhite} 
{
    modifyPieceIconForColor(this);
}

ostream &operator<<(ostream &out, const Piece &p) {
    string icon{p.getPieceIcon()};
    out << icon;
    
    return out;
}
