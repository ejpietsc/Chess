
#include <iostream>
#include <string>
#include "game.h"
#include "util.h"

using namespace std;

int main()
{
    string cmd; // game - resign - player - setup - done
    string whitePlayer, blackPlayer;
    int computerLvl; // 1 - 4
    string tilePos1, tilePos2;
    char piece; // K - k
    char sym;   // + - =

    // game session
    while (true)
    {
        cin >> cmd;
        cin >> whitePlayer;
        if (whitePlayer == "computer") {
            cin >> computerLvl;
        }
        cin >> blackPlayer;
        Board board;
        


    }
}
