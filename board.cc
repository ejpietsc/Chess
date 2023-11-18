#include "board.h"

using namespace std;

const char START_CHAR = 'a'; // starting row character (abcd...)
const int NUM_ROWS = 8; // # of rows on board
const int NUM_COLUMNS = 8; // # of columns on board
const int NUM_TILES = NUM_ROWS * NUM_COLUMNS; // 8x8 board

const char WHITE_TILE = ' '; // white tile display icon
const char BLACK_TILE = '_'; // black tile display icon

Tile::Tile(bool isWhite)
: isWhite{isWhite} {}

Tile::~Tile() {
    if (this->occupant != nullptr) {
        delete this->occupant;
    }
}

bool Tile::tileIsWhite() const {
    return this->isWhite;
}

Piece *Tile::getPiece() const {
    return this->occupant;
}

void Tile::setPiece(Piece *newOccupant) {
    this->occupant = newOccupant;
}

void Tile::printTile(ostream &out) const {
    if (this->occupant == nullptr) {
        if (this->isWhite) {
            out << WHITE_TILE;
        } else {
            out << BLACK_TILE;
        }
    } else {
        out << *(this->getPiece());
    }
}

// add piece, eg. white/black pawns, rooks, knights, etc. to Tile
//  *if* that piece should be there at the start of a standard chess game
static void addPieceToBoardOnInit(Tile &t, const int rowNum, const char colChar) {
     // nothing to add
    if (rowNum != 1 && rowNum != 2 && rowNum != 7 && rowNum != 8) {
        return;
    }

    Piece *p = nullptr;

    // pawns
    if (rowNum == 2) { // white row 2 of pawns
        p = new Pawn{true};
    } else if (rowNum == 7) { // black row 7 of pawns
        p = new Pawn{false};
    }

    // at this point, rowNum == 1 or rowNum == 8

    // rooks
    else if (colChar == 'a' || colChar == 'h') {
        if (rowNum == 1) { // white rooks
            p = new Rook{true};
        } else { // black rooks
            p = new Rook{false};
        }
    }

    // knights
    // else if ...

    if (p != nullptr) {
        t.setPiece(p);
    }
}

Board::Board() {
    map<string, Tile *> tiles;
    bool isWhite = true;
    const char endChar = START_CHAR + NUM_COLUMNS; // final char to end at

    // create a tile for each position on the board
    // start from a8, end at h1
    for (int rowNum = NUM_ROWS; rowNum >= 1; --rowNum) { // iterate rows, starting at row 8

        for (char colChar = START_CHAR; colChar < endChar; ++colChar) { // iterate columns, starting at column 'a'
            string pos{colChar};
            pos = pos + to_string(rowNum);
            Tile *t = new Tile{isWhite};
            addPieceToBoardOnInit(*t, rowNum, colChar);
            this->tiles[pos] = t;

            isWhite ? isWhite = false : isWhite = true;
        }

        isWhite ? isWhite = false : isWhite = true;
    }
}

Board::~Board() {
    // delete all tiles, which will in turn delete all pieces
    const char endChar = START_CHAR + NUM_COLUMNS;
    for (int rowNum = 1; rowNum <= NUM_ROWS; ++rowNum) {
        for (char colChar = START_CHAR; colChar < endChar; ++colChar) {
            string pos{colChar};
            pos = pos + to_string(rowNum);
            delete (this->tiles).at(pos);
        }
    }

    (this->tiles).clear();
    

}

Tile &Board::getTile(string pos) const {
    return *((this->tiles).at(pos));
}

void Board::printBoard(ostream &out) const {
    char currChar;

    for (int i = 0; i < NUM_ROWS; ++i) { // print each row
        currChar = START_CHAR;
        string currRow = to_string(NUM_ROWS - i);
        out << currRow << " "; // rowNum on the left hand side

        for (int j = 0; j < NUM_COLUMNS; ++j) { // print each column
            string pos{currChar};
            pos = pos + currRow;
            Tile &t = this->getTile(pos);
            out << t;
            ++currChar;
        }

        out << "\n";
    }

    // output bottom abcd... segment
    out << "\n  ";
    currChar = START_CHAR;

    for (int i = 0; i < NUM_COLUMNS; ++i) {
        out << currChar;
        ++currChar;
    }

    out << endl;
}

// Tile output operator
ostream &operator<<(ostream &out, const Tile &t) {
    t.printTile(out);
    return out;
}

// Board output operator
ostream &operator<<(ostream &out, const Board &b) {
    b.printBoard(out);
    return out;
}
