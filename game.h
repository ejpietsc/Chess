// includes game interface

#ifndef GAME_H
#define GAME_H

#include "board.h"
#include "pieces.h"
#include "player.h"

using namespace std;

enum class GameState { Setup, Play, Check, Checkmate, Stalemate };

class Game {
    GameState gameState = GameState::Setup;
    Board *gameBoard; // pointer to current gameboard
    Player *player1;
    Player *player2;
    int nextPlay = 0;

    public:
    // create a new game
    Game();

    // destroy game object
    ~Game();

    // return reference to Board gameBoard
    Board &getBoard() const;

};

#endif
