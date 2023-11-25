#include "board.h"

using namespace std;

// TODO
Board::Board(Colour team1, PlayerType p1, Colour team2, PlayerType p2):
//! TO DO
    // board{<vector<unique_ptr<Piece>>[8]} 
    state{GameState::Play}, blackPlayer{make_unique<Player>(team1, p1)},
    whitePlayer{make_unique<Player>(team2, p2)}, currPlayer{whitePlayer.get()} {
        for (auto& ptr:board) {

        }
}



