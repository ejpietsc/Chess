#include "util.h"

using namespace std;

static const float WIN_POINTS = 1;
static const float STALEMATE_POINTS = 0.5;
static const string INVALID_COMMAND = "Invalid command read! Try again";

// convert string to be fully lowercase (eg. "HElLo" -> "hello")
static string toLowerString(const string &s)
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
static bool isLowerCaseString(const string &s)
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

// return a Position given a tile string (eg. "a4" -> Position{0, 3})
//  return Position{-1, -1} for invalid input
static Position strToPosition(const string &s)
{
    const int len = s.length();
    if (len != 2)
    {
        return Position{-1, -1};
    }

    const int col = s[0] - 'a';
    const int row = s[1] - '1';
    return Position{row, col};
}

// given a string (eg. "computer4"), return the appropriate PlayerType
//  or NULL_PLR if invalid input
static PlayerType strToPlayer(const string &s)
{
    const string lowerS = toLowerString(s);

    if (lowerS == "human")
    {
        return PlayerType::Human;
    }
    else
    {
        const int len = s.length();
        if (len < 1)
        {
            return PlayerType::NULL_PLR;
        }

        if (lowerS.substr(0, len - 1) == "computer")
        {
            return PlayerType::Computer;
        }
    }

    return PlayerType::NULL_PLR;
}

// given a string (eg. "computer4"), return the appropriate PlayerType
//  or NULL_PLR if invalid input strToPieceType
static PieceType strToPieceType(const string &s)
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

    return PieceType::NULL_PIECE;
}

// given a string (eg. "computer4"), return the associated computer level
//  or 0 if it does not have a level strToComputerLvl
static int strToComputerLvl(const string &s)
{
    int level;
    const int len = s.length();

    if (len < 1)
    { // prevent bad input from throwing on s.substr
        return 0;
    }

    const string levelStr = s.substr(len - 1, len);

    stringstream ss{levelStr};
    ss >> level;

    if (ss.fail())
    {
        return 0;
    }
    else
    {
        return level;
    }
}

// enable Board setup mode
static bool enterSetupMode(Board &gameBoard)
{
    gameBoard.setState(GameState::Setup);
    string curLine;
    string cmd, option1, option2;

    while (true)
    {
        getline(cin, curLine);

        // handle fatal read fail
        if (cin.fail())
        {
            gameBoard.setState(GameState::Play);
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
            {
                gameBoard.setState(GameState::Play);
                return true; // indicate that setup succeeded
            }
            else
            {
                cout << INVALID_COMMAND << endl;
                continue;
            }
        }

        if (lowerCmd == "+")
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
            if (isLowerCaseString(option1))
            {
                pieceColour = Colour::Black;
            }
            else
            {
                pieceColour = Colour::White;
            }

            // determine piece type to add
            const string lowerOption1 = toLowerString(option1);
            PieceType pieceType = strToPieceType(lowerOption1);

            // handle invalid input
            if (pieceType == PieceType::NULL_PIECE)
            {
                cout << "Invalid piece type! Try again" << endl;
                continue;
            }

            Position piecePosition = strToPosition(lowerOption2);
            if (piecePosition.col < 0 || piecePosition.col >= 8 || piecePosition.row < 0 || piecePosition.row >= 8)
            {
                cout << "Invalid position input! Try again" << endl;
                continue;
            }
            gameBoard.addPiece(piecePosition, pieceType, pieceColour);
        }

        if (lowerCmd == "-")
        { // - e1 command
            ss >> option1;

            // handle read fail (3)
            if (ss.fail())
            {
                cout << INVALID_COMMAND << endl;
                continue;
            }

            const string lowerOption1 = toLowerString(option1);
            Position pos = strToPosition(lowerOption1);
            if (pos.col < 0 || pos.col >= 8 || pos.row < 0 || pos.row >= 8)
            {
                cout << INVALID_COMMAND << endl;
                continue;
            }

            gameBoard.delPiece(pos);
        }

        if (lowerCmd == "=")
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
    }
}

// run the general game loop to play a chess game given the Board to play on
static void playGame(Board &gameBoard)
{
    while (true)
    { // this loop is for every INDIVIDUAL GAME
        gameBoard.initBoard();

        // we need the user to explicitly call a "start" command
        //  otherwise, say they first run 'game computer1 human'
        //  BUT, they want to also enter setup mode
        //  in this scenario, computer gets to make a move before human
        //  has the ability to provide any input at all
        string curLine;
        string cmd;
        while (true)
        { // this loop is to await start/setup command
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
                { // fatal read fail occured in enterSetupMode
                    return;
                }
            }
        }

        while (true)
        { // this loop iterates each player to process their moves until the game ends
            Player &currPlr = *(gameBoard.getCurrPlayer());
            Colour currClr = currPlr.getColour();
            Colour otherClr = currClr == Colour::White ? Colour::Black : Colour::White;

            GameState state = gameBoard.getState();

            if (state == GameState::Play || state == GameState::Check)
            { // normal game operations, the player can move
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
                    gameBoard.incrementScore(otherClr, WIN_POINTS);
                    break;
                }
                else if (lowerCmdPrefix == "move")
                {
                    gameBoard.move();
                }
                else
                {
                    string trash;
                    getline(cin, trash); // flush current line
                    cout << INVALID_COMMAND << endl;
                    continue;
                }
            }

            else if (state == GameState::Checkmate)
            { // current player is checkmated
                gameBoard.incrementScore(otherClr, WIN_POINTS);
                break;
            }
            else if (state == GameState::Stalemate)
            {
                gameBoard.incrementScore(currClr, STALEMATE_POINTS);
                gameBoard.incrementScore(otherClr, STALEMATE_POINTS);
                break;
            }
            else {
                cout << "Invalid GameState! Restarting..." << endl;
                break;
            }
        }
    }
}

// output formatted score
static void outScore(const float whiteScore, const float blackScore) 
{
    cout << "Final Score:\n";
    cout << "White: " << whiteScore << "\n";
    cout << "Black: " << blackScore << endl;
}

int main()
{
    string curLine; // current line from input
    string cmd;     // game - resign - player - setup - done
    string whitePlayer, blackPlayer;
    int whiteLevel, blackLevel; // 0 - 4 (0 for humans)

    // create new board after [game] cmd
    PlayerType whitePt;
    PlayerType blackPt;
    while (true)
    {
        getline(cin, curLine);

        // handle fatal read fail
        if (cin.fail())
        {
            outScore(0, 0);
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

        if (lowerCmd == "game")
        { // try to obtain sufficient info to start the game
            ss >> whitePlayer >> blackPlayer;

            // handle read fail
            if (ss.fail())
            {
                cout << INVALID_COMMAND << endl;
                continue;
            }

            whitePt = strToPlayer(whitePlayer);
            blackPt = strToPlayer(blackPlayer);

            // handle invalid input
            if (whitePt == PlayerType::NULL_PLR || blackPt == PlayerType::NULL_PLR)
            {
                continue; // try again -> return to beginning of outer loop
            }

            // retrieve Player level
            whiteLevel = strToComputerLvl(whitePlayer);
            blackLevel = strToComputerLvl(blackPlayer);

            // we have all the information we need - we never need to return to this loop
            //  therefore break the loop and start the main game loop
            break;
        }
    }

    // create board and commence main gameplay loop
    Board gameBoard{whitePt, whiteLevel, blackPt, blackLevel};

    playGame(gameBoard);

    // ctrl-d has been pressed:
    const float whiteScore = gameBoard.getScore(Colour::White);
    const float blackScore = gameBoard.getScore(Colour::Black);

    outScore(whiteScore, blackScore);
}
