#include "pieces.h"

Piece::Piece(const bool isWhite): isWhite{isWhite} {}

Piece::~Piece() {}

bool Piece::pieceIsWhite() const {
    return isWhite;
}

PieceType Piece::getType() const {
    return type;
}

void Piece::setType(PieceType pt) {
    type = pt;
}

// *** unimplemented
bool Piece::move(string pos) {
    return true;
}

// *** unimplemented
bool Piece::capture(string pos) {
    return true;
}

EmptyPiece::EmptyPiece(): Piece(false) {
    setType(PieceType::EMPTY);
}

King::King(const bool isWhite): Piece(isWhite) {
    setType(PieceType::King);
}

Queen::Queen(const bool isWhite): Piece(isWhite) {
    setType(PieceType::Queen);
}

Bishop::Bishop(const bool isWhite): Piece(isWhite) {
    setType(PieceType::Bishop);
}

Rook::Rook(const bool isWhite): Piece(isWhite) {
    setType(PieceType::Rook);
}

Knight::Knight(const bool isWhite): Piece(isWhite) {
    setType(PieceType::Knight);
}

Pawn::Pawn(const bool isWhite): Piece(isWhite) {
    setType(PieceType::Pawn);
}

ostream &operator<<(ostream &out, const Piece &p) {
    // string icon{p.getPieceIcon()};
    string icon = "X";
    out << icon;
    
    return out;
}
