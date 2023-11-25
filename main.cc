#include "util.h"

using namespace std;

// TODO consider storing these globals somewhere more graceful
// increment a player's points by this when they win/stalemate
static const float WIN_POINTS = 1;
static const float STALEMATE_POINTS = 0.5;

// convert string to be fully lowercase (eg. "HElLo" -> "hello")
static string toLowerString(const string &s) {
    const int len = s.length();
    string newStr = s;

    for (int i = 0; i < len; ++i) {
        newStr[i] = tolower(s[i]);
    }

    return newStr;
}

// determine if a string is lowercase (that is, it has no uppercase characters)
static bool isLowerCaseString(const string &s) {
    const int len = s.length();

    for (int i = 0; i < len; ++i) {
        if (tolower(s[i]) != s[i]) {
            return false;
        }
    }

    return true;
}

// return a Position given a tile string (eg. "a4" -> Position{0, 3})
static Position positionStringToPosition(const string &s) {
    const int len = s.length();
    if (len != 2) {
        return Position{-1, -1};
    }
    
    int col = s[0] - 'a';
    int row = s[1] - '1';
    return Position{row, col};
}

// given a string (eg. "computer4"), return the appropriate PlayerType
//  or NULL_PLR if invalid input
static PlayerType playerStringToPlayerType(const string &s) {
    const string lowerS = toLowerString(s);

    if (lowerS == "human") {
        return PlayerType::Human;

    } else {
        const int len = s.length();
        if (lowerS.substr(0, len - 1) == "computer") {
            return PlayerType::Computer;
        }
    }

    return PlayerType::NULL_PLR;
}

// given a string (eg. "computer4"), return the appropriate PlayerType
//  or NULL_PLR if invalid input
static PieceType pieceStringToPieceType(const string &s) {
    const string lowerS = toLowerString(s);

    if (lowerS == "k") {
        return PieceType::King;
    } else if (lowerS == "q") {
        return PieceType::Queen;
    } else if (lowerS == "r") {
        return PieceType::Rook;
    } else if (lowerS == "n") {
        return PieceType::Knight;
    } else if (lowerS == "b") {
        return PieceType::Bishop;
    } else if (lowerS == "p") {
        return PieceType::Pawn;
    }

    return PieceType::NULL_PIECE;
}

// given a string (eg. "computer4"), return the associated computer level
//  or 0 if it does not have a level
static int playerStringToLevel(const string &s) {
    int level;
    const int len = s.length();
    const string levelStr = s.substr(len - 1, len);

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

// enable Board setup mode
// TODO (complete this)
static bool enterSetupMode(Board &gameBoard) {
    string curLine;
    string cmd, option1, option2;

    while (true) {
        getline(cin, curLine);

        // handle fatal read fail
        if (cin.fail()) {
            return false; // indicate that setup failed
        }

        stringstream ss{curLine};
        ss >> cmd;

        // handle read fail (1)
        if (ss.fail()) {
            cout << "Invalid command read!" << endl;
            continue;
        }

        const string lowerCmd = toLowerString(cmd);

        if (lowerCmd == "done") {
            if (gameBoard.boardIsValid()) {
                return true; // indicate that setup succeeded
            } else {
                cout << "Invalid board setup!" << endl;
                continue;
            }
        }
        
        if (lowerCmd == "+") { // + K e1 command
            ss >> option1 >> option2;

            // handle read fail (2)
            if (ss.fail()) {
                cout << "Invalid command read!" << endl;
                continue;
            }

            const string lowerOption2 = toLowerString(option2);
            
            // determine piece colour to add
            Colour pieceColour;
            if (isLowerCaseString(option1)) {
                pieceColour = Colour::Black;
            } else {
                pieceColour = Colour::White;
            }

            // determine piece type to add
            const string lowerOption1 = toLowerString(option1);
            PieceType pieceType = pieceStringToPieceType(lowerOption1);

            // handle invalid input
            if (pieceType == PieceType::NULL_PIECE) {
                cout << "Invalid piece type!" << endl;
                continue;
            }

            Position piecePosition = positionStringToPosition(lowerOption2);
            gameBoard.addPiece(piecePosition, pieceType, pieceColour);
        }

        if (lowerCmd == "-") { // - e1 command
            ss >> option1;

            // handle read fail (3)
            if (ss.fail()) {
                cout << "Invalid command read!" << endl;
                continue;
            }

            const string lowerOption1 = toLowerString(option1);
            Position pos = positionStringToPosition(lowerOption1);
            gameBoard.delPiece(pos);
        }

        if (lowerCmd == "=") { // = colour command
            ss >> option1;

            // handle read fail (4)
            if (ss.fail()) {
                cout << "Invalid command read!" << endl;
                continue;
            }

            const string lowerOption1 = toLowerString(option1);

            if (lowerOption1 == "white") {
                gameBoard.setTurn(Colour::White);
            } else if (lowerOption1 == "black") {
                gameBoard.setTurn(Colour::Black);
            }
        }
    }

    
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
                cout << "Invalid command read!" << endl;
                continue;
            }

            const string lowerCmd = toLowerString(cmd);
            
            if (lowerCmd == "start") { // get out of this loop and progress to gameplay
                break;

            } else if (lowerCmd == "setup") { // enter setup then progress to gameplay if setup succeeded
                bool succ = enterSetupMode(gameBoard);
                if (succ) {
                    break;
                } else { // fatal read fail occured in enterSetupMode
                    return;
                }
            }
        }

        while (true) { // this loop iterates each player to process their moves until the game ends
            Colour currClr = gameBoard.getTurn();
            Colour otherClr = getOtherColour(currClr);
            Player &currPlr = *(gameBoard.getPlayerByColour(currClr));

            GameState state = gameBoard.getState();
            if (state == GameState::Play) { // nobody is in check, stalemate, etc - normal game operations
                string cmdPrefix;
                cin >> cmdPrefix;

                // handle fatal read error
                if (cin.fail()) {
                    return;
                }

                const string lowerCmdPrefix = toLowerString(cmdPrefix);
                if (lowerCmdPrefix == "resign") {
                    gameBoard.incrementScore(otherClr, WIN_POINTS);
                    break;

                } else if (lowerCmdPrefix == "move") {
                    // TODO vvv the below logic should be ideally wrapped in a Board.getMove() method vvv
                    Move move = currPlr.getNextMove(); // prompt plr for move if human, or generate one if computer
                    
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
                    // TODO ^^^ the above logic should be ideally wrapped in a Board.getMove() method ^^^
                
                } else {
                    string trash;
                    getline(cin, trash); // flush current line
                    cout << "Invalid command read!" << endl;
                    continue;
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
        
        // handle read fail
        if (ss.fail()) {
            cout << "Invalid command read!" << endl;
            continue;
        }

        const string lowerCmd = toLowerString(cmd);

        if (lowerCmd == "game") { // try to obtain sufficient info to start the game
            ss >> whitePlayer >> blackPlayer;

            // handle read fail
            if (ss.fail()) {
                cout << "Invalid command read!" << endl;
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
