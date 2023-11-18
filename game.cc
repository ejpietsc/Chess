#include "game.h"

using namespace std;

Game::Game()
: gameState{"Setup"} 
{
    Board *b = new Board;
    this->gameBoard = b;
}

Game::~Game() {
    delete this->gameBoard;
}

Board &Game::getBoard() const {
    return *(this->gameBoard);
}
