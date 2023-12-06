#include "util.h"
#include "player.h"
#include "pieces.h"
#include "board.h"
#include "gameio.h"

using namespace std;

const float WIN_POINTS = 1;
const float STALEMATE_POINTS = 0.5;
const string INVALID_COMMAND = "Invalid command read! Try again";

// convert string to be fully lowercase (eg. "HElLo" -> "hello")
string toLowerString(const string &s)
{
    const int len = s.length();
    string newStr = s;

    for (int i = 0; i < len; ++i)
    {
        newStr[i] = tolower(s[i]);
    }

    return newStr;
}

// determine if a string is all lowercase
bool isLowerCaseString(const string &s)
{
    const int len = s.length();

    for (int i = 0; i < len; ++i)
    {
        if (tolower(s[i]) != s[i])
        {
            return false;
        }
    }
    return true;
}

// given a string (eg. "computer4"), return the appropriate PlayerType
//  or NULL_PLR if invalid input
PlayerType strToPlayer(const string &s)
{
    const string lowerS = toLowerString(s);
    const int len = s.length();

    if (lowerS == "human")
    {
        return PlayerType::Human;
    }
    else if (len >= 1 && lowerS.substr(0, len - 1) == "computer")
    {
        return PlayerType::Computer;
    }

    return PlayerType::NULL_PLR;
}

// given a string (eg. "computer4"), return the appropriate PlayerType
//  or NULL_PLR if invalid input strToPieceType
PieceType strToPieceType(const string &s)
{
    const string lowerS = toLowerString(s);

    if (lowerS == "k")
    {
        return PieceType::King;
    }
    else if (lowerS == "q")
    {
        return PieceType::Queen;
    }
    else if (lowerS == "r")
    {
        return PieceType::Rook;
    }
    else if (lowerS == "n")
    {
        return PieceType::Knight;
    }
    else if (lowerS == "b")
    {
        return PieceType::Bishop;
    }
    else if (lowerS == "p")
    {
        return PieceType::Pawn;
    }

    throw(std::invalid_argument("Incorrect piece string"));
}

// given a string (eg. "computer4"), return the associated computer level
//  or 0 if it does not have a level strToComputerLvl
int strToComputerLvl(const string &s)
{
    int level;
    const int len = s.length();

    // prevent bad input from throwing on s.substr
    if (len < 1)
    {
        return 0;
    }

    const string levelStr = s.substr(len - 1);

    stringstream ss{levelStr};
    ss >> level;

    if (ss.fail())
    {
        return 0;
    }

    return level;
}

std::string getUserInput()
{
    cout << "> ";
    std::string inp;
    cin >> inp;

    // handle fatal read error
    if (cin.fail())
    { // handler is in main
        throw ios_base::failure("Failure to read from cin");
    }

    return inp;
}

std::pair<std::string, std::string> getMoveFromUser()
{
    std::pair<std::string, std::string> ans;
    cin >> ans.first >> ans.second;

    // handle fatal read error
    if (cin.fail())
    { // handler is in main
        throw ios_base::failure("Failure to read from cin");
    }

    return ans;
}

// enable Board setup mode
bool enterSetupMode(Board &gameBoard)
{
    cout << "Entered setup mode" << endl;
    cout << "Commands:\n"
         << "---------" << endl;
    cout << "• '+ K e1' places the piece K (i.e., white king in this case) on the square e1. If a piece is already on that square, it is replaced" << endl;
    cout << "• '- e1' removes the piece from the square e1. If there is no piece at that square, take no action" << endl;
    cout << "• '= colour' makes it colour's turn to go next" << endl;
    cout << "• 'clear' clears the board from all pieces" << endl;
    cout << "• 'print' prints out the current board again" << endl;
    cout << "• 'refresh' Force refresh all displays (use this if the graphics display is corrupted)" << endl;
    cout << "• 'done' leaves setup mode\n"
         << endl;

    string curLine;
    string cmd, option1, option2;

    while (true)
    {
        cout << "> ";
        getline(cin, curLine);

        // handle fatal read fail
        if (cin.fail())
        {
            return false; // indicate that setup failed
        }

        stringstream ss{curLine};
        ss >> cmd;

        // handle read fail (1)
        if (ss.fail())
        {
            cout << INVALID_COMMAND << endl;
            continue;
        }

        const string lowerCmd = toLowerString(cmd);

        if (lowerCmd == "done")
        {
            if (gameBoard.boardIsValid())
            { // remove castling if setup was used
                for (int i = 0; i < NUM_ROWS; ++i) {
                    for (int j = 0; j < NUM_COLS; ++j) {
                        Piece *p = gameBoard.getPieceByCoords(i, j);
                        if (p) {
                            if (p->getType() == PieceType::King) {
                                King *k = dynamic_cast<King*>(p);
                                if (k) {
                                    k->setHasMoved(true);
                                }
                            }
                        }
                    }
                }
                return true; // indicate that setup succeeded
            }
            else
            {
                cout << "Invalid board setup. Retry" << endl;
                continue;
            }
        }

        else if (lowerCmd == "print")
        {
            gameBoard.displayObservers();
        }

        else if (lowerCmd == "refresh")
        {
            gameBoard.refreshObservers();
        }

        else if (lowerCmd == "+")
        { // + K e1 command
            ss >> option1 >> option2;

            // handle read fail (2)
            if (ss.fail())
            {
                cout << INVALID_COMMAND << endl;
                continue;
            }

            const string lowerOption2 = toLowerString(option2);

            // determine piece colour to add
            Colour pieceColour;
            pieceColour = isLowerCaseString(option1) ? Colour::Black : Colour::White;

            // determine piece type to add
            const string lowerOption1 = toLowerString(option1);
            PieceType pieceType;

            try
            {
                pieceType = strToPieceType(lowerOption1);
            }
            catch (...)
            { // handle invalid input
                cout << "Invalid piece type! Try again" << endl;
                continue;
            }

            Position piecePosition{lowerOption2};
            if (piecePosition.col < 0 || piecePosition.col >= 8 || piecePosition.row < 0 || piecePosition.row >= 8)
            {
                cout << "Invalid position input! Try again" << endl;
                continue;
            }
            gameBoard.addPiece(pieceType, pieceColour, piecePosition);
            gameBoard.notifyObservers(piecePosition);
        }

        else if (lowerCmd == "-")
        { // - e1 command
            ss >> option1;

            // handle read fail (3)
            if (ss.fail())
            {
                cout << INVALID_COMMAND << endl;
                continue;
            }

            const string lowerOption1 = toLowerString(option1);
            Position pos{lowerOption1};
            if (pos.col < 0 || pos.col >= 8 || pos.row < 0 || pos.row >= 8)
            {
                cout << INVALID_COMMAND << endl;
                continue;
            }

            gameBoard.delPiece(pos);
            gameBoard.notifyObservers(pos);
        }

        else if (lowerCmd == "=")
        { // = colour command
            ss >> option1;

            // handle read fail (4)
            if (ss.fail())
            {
                cout << INVALID_COMMAND << endl;
                continue;
            }

            const string lowerOption1 = toLowerString(option1);

            if (lowerOption1 == "white")
            {
                gameBoard.setTurn(Colour::White);
            }
            else if (lowerOption1 == "black")
            {
                gameBoard.setTurn(Colour::Black);
            }
        }
        else if (lowerCmd == "clear")
        {
            for (int i = 0; i < NUM_COLS; ++i)
            {
                for (int j = 0; j < NUM_ROWS; ++j)
                {
                    Position pos = Position{j, i};
                    gameBoard.delPiece(pos);
                }
            }
            gameBoard.refreshObservers();
        }

        else
        {
            cout << INVALID_COMMAND << endl;
        }
    }
} // end of enterSetupMode

// run the general game loop to play a chess game given the Board to play on
void playGame(Board &gameBoard)
{
    while (true)
    { // this loop is for every INDIVIDUAL GAME
        gameBoard.initBoard();

        cout << "A new game has started" << endl;
        cout << "Commands:\n"
             << "---------" << endl;
        cout << "• 'setup' will enter setup mode" << endl;
        cout << "• 'start' will start the game\n"
             << endl;

        // we need the user to explicitly call a "start" command
        //  otherwise, say they first run 'game computer1 human'
        //  BUT, they want to also enter setup mode
        //  in this scenario, computer gets to make a move before human
        //  has the ability to provide any input at all
        string curLine;
        string cmd;
        while (true)
        { // this loop is to await start/setup command
            cout << "> ";
            getline(cin, curLine);

            // handle fatal read fail
            if (cin.fail())
            {
                return;
            }

            stringstream ss{curLine};
            ss >> cmd;

            // handle read fail
            if (ss.fail())
            {
                cout << INVALID_COMMAND << endl;
                continue;
            }

            const string lowerCmd = toLowerString(cmd);

            if (lowerCmd == "start")
            { // get out of this loop and progress to gameplay
                break;
            }
            else if (lowerCmd == "setup")
            { // enter setup then progress to gameplay if setup succeeded
                bool succ = enterSetupMode(gameBoard);
                if (succ)
                {
                    break;
                }
                else
                { // fatal read fail occurred in enterSetupMode
                    return;
                }
            }
            else
            {
                cout << INVALID_COMMAND << endl;
            }
        }

        cout << "A game is in progress" << endl;
        cout << "Commands:\n"
             << "---------" << endl;
        cout << "• 'move <start> <end>' moves the piece in <start> to <end>" << endl;
        cout << "• 'resign' concedes the game to the opponenent (they will win)" << endl;
        cout << "• 'print' prints out the current board again" << endl;
        cout << "• 'refresh' Force refresh all displays (use this if the graphics display is corrupted)\n"
             << endl;

        while (true)
        { // this loop iterates each player to process their moves until the game ends
            Player &currPlr = *(gameBoard.getCurrPlayer());
            Player *currPlrPtr = gameBoard.getCurrPlayer();
            Colour currClr = currPlr.getColour();
            Colour otherClr = (currClr == Colour::White) ? Colour::Black : Colour::White;
            const string clrStr = (currClr == Colour::White ? "White" : "Black");
            const string otherClrStr = (otherClr == Colour::White ? "White" : "Black");

            if (gameBoard.isPlayerCheckmated(currPlrPtr))
            { // current player is checkmated
                cout << "Checkmate! " << otherClrStr << " wins!" << endl;
                gameBoard.incrementScore(otherClr, WIN_POINTS);
                break;
            }
            else if (gameBoard.isPlayerStalemated(currPlrPtr))
            {
                cout << "Stalemate!" << endl;
                gameBoard.incrementScore(currClr, STALEMATE_POINTS);
                gameBoard.incrementScore(otherClr, STALEMATE_POINTS);
                break;
            }

            else
            { // normal game operations, the player can move

                if (gameBoard.isPlayerInCheck(currPlrPtr))
                {
                    cout << clrStr << " is in check." << endl;
                }

                // ! handle computer move
                if (currPlr.getPlayerType() == PlayerType::Computer)
                {
                    gameBoard.makeMove();
                    gameBoard.flipTurn();
                    continue;
                }

                cout << clrStr << "'s turn:" << endl;
                cout << "> ";

                string cmdPrefix;
                cin >> cmdPrefix;

                // handle fatal read error
                if (cin.fail())
                {
                    return;
                }

                const string lowerCmdPrefix = toLowerString(cmdPrefix);
                if (lowerCmdPrefix == "resign")
                {
                    string trash;
                    getline(cin, trash); // flush current line
                    cout << clrStr << " has resigned! Ending game..." << endl;
                    gameBoard.incrementScore(otherClr, WIN_POINTS);
                    break;
                }
                else if (lowerCmdPrefix == "print")
                {
                    gameBoard.displayObservers();
                }

                else if (lowerCmdPrefix == "refresh")
                {
                    gameBoard.refreshObservers();
                }
                else if (lowerCmdPrefix == "move")
                {
                    try
                    {
                        Position landingPos = gameBoard.makeMove();
                        if (landingPos == INVALID_INPUT)
                        {
                            cout << "Invalid Input! Input format is: move [a-h,1-8] [a-h,1-8]" << endl;
                        }
                        else if (landingPos == ILLEGAL_MOVE)
                        {
                            // ? add more for more specific error messages
                            cout << "Illegal Move! Try again" << endl;
                        }
                        else
                        {
                            gameBoard.flipTurn();
                        }
                        string trash;
                        getline(cin, trash); // flush current line
                    }
                    catch (...) // handle fatal read error
                    {
                        return;
                    }
                }
                else
                {
                    string trash;
                    getline(cin, trash); // flush current line
                    cout << INVALID_COMMAND << endl;
                    continue;
                }
            }
        }
    }
} // end of playGame()

// output formatted score
// todo maybe move to text display (View)
void outScore(const float whiteScore, const float blackScore)
{
    cout << "Final Score:\n";
    cout << "White: " << whiteScore << "\n";
    cout << "Black: " << blackScore << endl;
}

bool isInVector(std::vector<std::string> &vec, std::string key)
{
    for (std::string i : vec)
    {
        if (i == key)
            return true;
    }
    return false;
}
