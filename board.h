// provides board class
#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "util.h"
#include "pieces.h"
#include "observer.h"

using namespace std;

// class for the game board
class Board {
    vector<vector<Piece *>> board;
    GameState state;
    Player *blackPlayer;
    Player *whitePlayer;
    Player *currPlayer;

    public:
    // Ctor
    Board();

    // Copy ctor and assignment operator
    Board(const Board& other);
    Board& operator=(const Board& other);

    // Move ctor and assignment operator
    Board(Board&& other);
    Board& operator=(Board&& other);

    // Dtor
    ~Board();

    // Notify observers
    void notifyObservers();

    // Change the pieces
    void initBoard();
    void addPiece(Position pos, PieceType pt, Colour col);
    void delPiece(Position pos);

    // Getters and setters
    GameState getState();
    void setState(GameState state);
    Player *getPlayerByColour(Colour clr);
    Player *getCurrPlayer();
    Piece *getPiece(Position p);

    // General gameplay logic
    vector<Move> getValidMoves(Player &plr);
};

#endif
