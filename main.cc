#include "util.h"
#include "player.h"
#include "pieces.h"
#include "board.h"
#include "gameio.h"

using namespace std;

int main(int argc, char **argv)
{
    cout << "          " << endl;
    cout << "   ####   " << endl;
    cout << "  ######     _____ _    _ ______  _____ _____ " << endl;
    cout << "   ####     / ____| |  | |  ____|/ ____/ ____|" << endl;
    cout << "    ##     | |    | |__| | |__  | (___| (___   " << endl;
    cout << "   ####    | |    |  __  |  __|  \\___ \\\\___ \\ " << endl;
    cout << "    ##     | |____| |  | | |____ ____) |___) |" << endl;
    cout << "  ######    \\_____|_|  |_|______|_____/_____/ " << endl;
    cout << "  ######  " << endl;
    cout << " ######## " << endl;
    cout << "\nWelcome to Chess - By Imane, Amol & Evan with <3!\n" << endl;

    bool useGraphics = true;
    bool useText = true;
    bool useUnicode = false;

    std::vector<std::string> args;
    for (int i = 1; i < argc; ++i) {
        args.emplace_back(argv[i]);
    }

    if (isInVector(args, "-d")) {
        cout << "Debug" << endl;
    }

    if (isInVector(args, "-ng")) {
        cout << "[INFO] Graphics display is disabled." << endl;
        useGraphics = false;
    }

    if (isInVector(args, "-nt")) {
        if (useGraphics) {
            cout << "[INFO] Text display is disabled." << endl;
            useText = false;
        }
        else {
            cout << "[WARNING] You have requested for both displays to be disabled. This makes the game unplayable, and thus the text display had been left enabled." << endl;
        }
    }

    if (isInVector(args, "-u")) {
        if (useText) {
            cout << "[INFO] Unicode display is enabled." << endl;
            useUnicode = true;
        }
        else {
            cout << "[WARNING] You have requested for unicode display but have disabled the text display. This will not habe any effect." << endl;
        }
    }

    cout << "Commands:\n"
         << "---------" << endl;
    cout << "• 'game white-player black-player' starts a game with designated player types." << endl;
    cout << "The parameters white-player and black-player can be either [human] or [computer[1-4]]\n"
         << endl;

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
            outScore(0, 0); // we don't have enough info to create a gameBoard, output dummy values
            return 0;
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
                cout << INVALID_COMMAND << endl;
                continue; // try again -> return to beginning of outer loop
            }

            // retrieve Player level
            whiteLevel = strToComputerLvl(whitePlayer);
            blackLevel = strToComputerLvl(blackPlayer);

            // we have all the information we need - we never need to return to this loop
            //  therefore break the loop and start the main game loop
            break;
        }
        else
        {
            cout << INVALID_COMMAND << endl;
        }
    }

    // create board and commence main gameplay loop
    Board gameBoard{whitePt, whiteLevel, blackPt, blackLevel, useGraphics, useText, useUnicode};

    playGame(gameBoard);

    // ctrl-d has been pressed:
    const float whiteScore = gameBoard.getScore(Colour::White);
    const float blackScore = gameBoard.getScore(Colour::Black);

    outScore(whiteScore, blackScore);
}
