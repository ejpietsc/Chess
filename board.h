// provides board class
#ifndef BOARD_H
#define BOARD_H

#include "util.h"

using namespace std;

enum class GameState { Setup, Play, Check, Checkmate, Stalemate, CTRL_D };

// class for the game board
class Board {
    vector<vector<Piece *>> board;
    GameState state;
    unique_ptr<Player> blackPlayer;
    unique_ptr<Player> whitePlayer;
    Colour turn;

    // Determine GameStates
    bool isPlayerInCheck(Player &plr);
    bool isPlayerCheckmated(Player &plr);
    bool isPlayerStalemated(Player &plr);

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

    // Change the pieces
    void initBoard();
    void addPiece(Position pos, PieceType pt, Colour clr);
    void delPiece(Position pos);

    // Getters and setters
    GameState getState();
    void setState(GameState state);
    Player *getPlayerByColour(Colour clr);
    Colour getTurn();
    Piece *getPiece(Position pos);
    void updateState();

    // General gameplay logic
    vector<Move> getValidMoves(Player &plr);
    bool isPlayerMoveValid();
    // Check if plr is checkmated or stalemated, 
    //  if this returns true, main can then call getState() to determine what to do
    bool didPlayerLose(Player &plr);

};

#endif
