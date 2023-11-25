#include "board.h"

using namespace std;

// determine if move is within the validMoves vector
static bool moveIsValid(Move &move, vector<Move> &validMoves)
{
    for (const auto &childMove : validMoves)
    {
        if (move == childMove)
        {
            return true;
        }
    }
    return false;
}

Colour Board::getOtherColour(Colour clr) const
{
    return (clr == Colour::White ? Colour::Black : Colour::White);
}

//todo update: called right after move cmd is read
// ! modified fn to return bool
bool Board::move()
{
    Move move = currPlayer->getNextMove();

    vector<Move> validMoves = getValidMoves(*currPlayer);

    if (moveIsValid(move, validMoves))
    {
        Piece *pieceToMove = getPiece(move.startPos);

        if (pieceToMove != nullptr)
        { // a valid move has occurred!
            pieceToMove->makeMove(move.endPos);
            setTurn(getOtherColour(currPlayer->getColour()));
            return true;
        }
    } // else do ntg here

    return false;
}

// TODO
// Board::Board(Colour team1, PlayerType p1, Colour team2, PlayerType p2):
// //! TO DO
//     // board{<vector<unique_ptr<Piece>>[8]}
//     state{GameState::Play}, blackPlayer{make_unique<Player>(team1, p1)},
//     whitePlayer{make_unique<Player>(team2, p2)}, currPlayer{whitePlayer.get()} {
//         for (auto& ptr:board) {

//         }
// }

