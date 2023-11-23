// provides board and tile classes

#ifndef BOARD_H
#define BOARD_H

#include "pieces.h"
#include <iostream>
#include <map>
#include <string>

using namespace std;

// class for each tile on the board (8x8)
class Tile {
    Piece *occupant = nullptr; // pointer to piece occupying the tile
    bool isWhite; // true if tile is white

    public:
    // ctor
    Tile(bool isWhite);

    // dtor
    ~Tile();

    // return isWhite bool
    bool tileIsWhite() const;

    // return pointer to current piece on Tile, or nullptr if no occupant
    Piece *getPiece() const;

    // set piece on Tile to newOccupant
    void setPiece(Piece *newOccupant);

    // print out content of Tile (" ", "_", or occupying piece icon)
    void printTile(ostream &out) const;

};

// class for the game board
class Board {
    map<string, Tile *> tiles; // map of all tile pointers, referenced by position string (eg. "e4")

    public:
    // ctor
    Board();

    // dtor
    ~Board();

    // get reference to Tile at pos (eg. "e4")
    Tile &getTile(string pos) const;

    // print entire Board to out
    void printBoard(ostream &out) const;

};

// output the icon of the piece on Tile t
ostream &operator<<(ostream &out, const Tile &t);

// print the entire Board b out
ostream &operator<<(ostream &out, const Board &b);

#endif
