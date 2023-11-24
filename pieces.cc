#include "pieces.h"

using namespace std;

// TODO


void Piece::setPosition(int r, int c) {
    pos = Position{r, c};
}

int Piece::getRow() const {
    return pos.row;
}

int Piece::getCol() const {
    return pos.col;
}

