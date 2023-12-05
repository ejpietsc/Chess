#include "util.h"
#include "player.h"
#include "pieces.h"
#include "board.h"

using namespace std;

extern const float WIN_POINTS;
extern const float STALEMATE_POINTS;
extern const string INVALID_COMMAND;

string toLowerString(const string &s);

bool isLowerCaseString(const string &s);

PlayerType strToPlayer(const string &s);

PieceType strToPieceType(const string &s);

int strToComputerLvl(const string &s);

bool enterSetupMode(Board &gameBoard);

void playGame(Board &gameBoard);

void outScore(const float whiteScore, const float blackScore);

bool isInVector(std::vector<std::string>& vec, std::string key);
