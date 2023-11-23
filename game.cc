#include "game.h"

using namespace std;

Game::Game() {
    Board *b = new Board;
    this->gameBoard = b;
}

Game::~Game() {
    delete this->gameBoard;
}

Board &Game::getBoard() const {
    return *(this->gameBoard);
}
