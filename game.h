// includes game interface

#ifndef GAME
#define GAME

#include "board.h"
#include "pieces.h"

using namespace std;

class Game {
    string gameState; // "Setup", "In-play", "_ is in check." "Checkmate! _ wins!", "Stalemate!"
    Board *gameBoard; // pointer to current gameboard

    public:
    // create a new game
    Game();

    // destroy game object
    ~Game();

    // return reference to Board gameBoard
    Board &getBoard() const;

};

#endif
