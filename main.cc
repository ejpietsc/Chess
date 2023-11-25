#include "util.h"

using namespace std;

// TODO consider storing these globals somewhere more graceful
// increment a player's points by this when they win/stalemate
static const float WIN_POINTS = 1;
static const float STALEMATE_POINTS = 0.5;

// given a string (eg. "computer4"), return the appropriate PlayerType
//  or NULL_PLR if invalid input
static PlayerType playerStringToPlayerType(const string &s) {
    if (s == "human") {
        return PlayerType::Human;

    } else {
        const int len = s.length();
        if (s.substr(0, len - 1) == "computer") {
            return PlayerType::Computer;
        }
    }

    return PlayerType::NULL_PLR;
}

// given a string (eg. "computer4"), return the associated computer level
//  or 0 if it does not have a level
static int playerStringToLevel(const string &s) {
    int level;
    const int len = s.length();
    string levelStr = s.substr(len - 1, len);

    stringstream ss{levelStr};
    ss >> level;

    if (ss.fail()) {
        return 0;
    } else {
        return level;
    }
}

// determine if move is within the validMoves vector
static bool moveIsValid(Move &move, vector<Move> &validMoves) {
    for (auto childMove : validMoves) {
        if (move == childMove) {
            return true;
        }
    }

    return false;
}

// given a clr, get the OTHER colour (white -> black, black -> white)
static Colour getOtherColour(Colour clr) {
    return (clr == Colour::White ? Colour::Black : Colour::White);
}

// perform Board setup
// TODO
static void enterSetupMode(Board &gameBoard) {

}

// run the general game loop to play a chess game given the Board to play on
static void playGame(Board &gameBoard) {
    while (true) { // this loop is for every INDIVIDUAL GAME
        gameBoard.initBoard();

        // we need the user to explicitly call a "start" command
        //  otherwise, say they first run 'game computer1 human'
        //  BUT, they want to also enter setup mode
        //  in this scenario, computer gets to make a move before human 
        //  has the ability to provide any input at all 
        string curLine;
        string cmd;
        while (true) { // this loop is to await start/setup command
            getline(cin, curLine);

            // handle fatal read fail
            if (cin.fail()) {
                return;
            }

            stringstream ss{curLine};
            ss >> cmd;

            // handle read fail
            if (ss.fail()) {
                continue;
            }
            
            if (cmd == "start") { // get out of this loop and progress to gameplay
                break;

            } else if (cmd == "setup") {
                enterSetupMode(gameBoard);
            }
        }

        while (true) { // this loop iterates each player to process their moves until the game ends
            Colour currClr = gameBoard.getTurn();
            Colour otherClr = getOtherColour(currClr);
            Player &currPlr = *(gameBoard.getPlayerByColour(currClr));

            GameState state = gameBoard.getState();
            if (state == GameState::Play) { // nobody is in check, stalemate, etc - normal game operations
                Move move = currPlr.getNextMove(); // prompt plr for move if human, or generate one if computer
                
                if (move == RESIGNATION_MOVE) { // player resigns
                    gameBoard.incrementScore(otherClr, WIN_POINTS);
                    break;
                }
                
                vector<Move> validMoves = gameBoard.getValidMoves(currPlr);

                if (moveIsValid(move, validMoves)) {
                    Position &startPos = move.startPos;
                    Position &endPos = move.endPos;
                    Piece *pieceToMove = gameBoard.getPiece(startPos);

                    if (pieceToMove != nullptr) { // a valid move has occurred!
                        pieceToMove->makeMove(endPos);
                        gameBoard.setTurn(otherClr);
                    }
                }
            
            }
        }


    }
}

int main()
{   
    Board *gameBoardPtr = nullptr;
    string curLine; // current line from input
    string cmd; // game - resign - player - setup - done
    string whitePlayer, blackPlayer;
    int whiteLevel, blackLevel; // 0 - 4 (0 for humans)

    // game session
    while (true) {
        getline(cin, curLine);

        // handle fatal read fail
        if (cin.fail()) {
            return;
        }

        stringstream ss{curLine};
        ss >> cmd;

        if (cmd == "game") { // try to obtain sufficient info to start the game
            ss >> whitePlayer >> blackPlayer;

            // handle read fail
            if (ss.fail()) {
                continue;
            }

            PlayerType whitePt = playerStringToPlayerType(whitePlayer);
            PlayerType blackPt = playerStringToPlayerType(blackPlayer);

            // handle invalid input
            if (whitePt == PlayerType::NULL_PLR || blackPt == PlayerType::NULL_PLR) {
                continue; // try again -> return to beginning of outer loop
            }

            // retrieve Player level
            whiteLevel = playerStringToLevel(whitePlayer);
            blackLevel = playerStringToLevel(blackPlayer);

            // create board and commence main gameplay loop
            Board gameBoard{whitePt, whiteLevel, blackPt, blackLevel};
            gameBoardPtr = &gameBoard;

            // we have all the information we need - we never need to return to this loop
            //  therefore break the loop and start the main game loop
            break;
        }    
    }

    Board &gameBoard = *gameBoardPtr;
    playGame(gameBoard);

    // ctrl-d has been pressed:
    const float whiteScore = gameBoard.getScore(Colour::White);
    const float blackScore = gameBoard.getScore(Colour::Black);

    cout << "Final Score:\n";
    cout << "White: " << whiteScore << "\n";
    cout << "Black: " << blackScore << endl;
}
