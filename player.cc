#include "player.h"
#include "board.h"
#include "gameio.h"
#include "util.h"

using namespace std;

// Forward declarations
// std::string getUserInput(const std::string prompt);
// std::pair<std::string, std::string> getHumanMove();
// PieceType strToPieceType(const string &s);

// Constants for the computer players

// Values of different piece types
std::map<PieceType, int> PIECE_VALUES = {
    {PieceType::King, 500},
    {PieceType::Queen, 250},
    {PieceType::Bishop, 100},
    {PieceType::Rook, 100},
    {PieceType::Knight, 75},
    {PieceType::Pawn, 25}};

int CHECK_VALUE = 250;      // Value of a check
int CHECKMATE_VALUE = 2500; // Value of a checkmate

// TODO

// PUBLIC HELPER
//! DON'T MAKE STATIC - USED IN BOARD.cc
bool isHuman(const Player *p)
{
    return (p->getPlayerType() == PlayerType::Human);
}

//! DON'T MAKE STATIC - USED IN BOARD.cc
bool isWhiteTeam(const Player *p)
{
    return (p->getColour() == Colour::White);
}

// === PLAYER ===
Player::Player(const Colour team, const PlayerType p)
    : team{team}, p{p} {}

Colour Player::getColour() const
{
    return this->team;
}

PlayerType Player::getPlayerType() const
{
    return this->p;
}

Move Player::getNextMove(vector<Move> &validMoves, Board *b) const
{
    return this->doGetNextMove(validMoves, b);
}

// === HUMAN ===
Human::Human(const Colour team, const PlayerType p)
    : Player{team, p} {}

Move Human::doGetNextMove(vector<Move> &validMoves, Board *b) const
{
    return this->getHumanMove(validMoves, b);
}

//! check start and end are proper format
Move Human::getHumanMove(vector<Move> &validMoves, Board *b) const
{
    std::pair<std::string, std::string> mv = getMoveFromUser();
    Move move = Move{mv.first, mv.second};

    //!____________________________________________________
    // En Passent
    Position epp = Position{move.endPos.col, move.startPos.row};
    Piece *ep_piece = b->getPiece(epp);
    Piece *p = b->getPiece(move.startPos);

    if (
        ep_piece &&
        p->getType() == PieceType::Pawn &&
        ep_piece->getType() == PieceType::Pawn &&
        ep_piece->getColour() != p->getColour() &&
        dynamic_cast<Pawn *>(ep_piece)->getDoubleMoved())
    {
        move.captured = true;
        move.capturedPt = PieceType::Pawn;
        move.enPassentCapture = true;
        move.epCaptureLoc = epp;

        if (!moveIsValid(move, validMoves, 'e'))
        {
            move.endPos = Position(ILLEGAL_MOVE);
            return move;
        }
    }
    //!____________________________________________________
    // Pawn promotion
    else if (
        p->getType() == PieceType::Pawn &&
        (move.endPos.row == 0 ||
         move.endPos.row == NUM_ROWS - 1))
    {
        std::string pt = getUserInput();
        move.upgradePiece = true;
        move.upgradeTo = strToPieceType(toLowerString(pt));

        if (!moveIsValid(move, validMoves, 'p'))
        {
            move.endPos = Position(ILLEGAL_MOVE);
            return move;
        }
    }
    //!____________________________________________________
    else if (p->getType() == PieceType::King && !(dynamic_cast<King *>(p)->getHasMoved()) &&
             isCastleMove(move, *b))
    {
        move.isCastleMove = true;

        if (!moveIsValid(move, validMoves, 'k'))
        {
            move.endPos = Position(ILLEGAL_MOVE);
            return move;
        }
    }
    //!____________________________________________________

    else if (b->getPieceByPos(move.endPos))
    {
        move.captured = true;
        
        if (!moveIsValid(move, validMoves, 'c'))
        {
            move.endPos = Position(ILLEGAL_MOVE);
            return move;
        }
    }
    //!____________________________________________________

    else
    {
        if (!moveIsValid(move, validMoves, 'n'))
        {
            move.endPos = Position(ILLEGAL_MOVE);
            return move;
        }
    }

    return move;
}

// === COMPUTER ===
//! ADDED LVL FIELD
Computer::Computer(const Colour team, const PlayerType p, const int lvl)
    : Player{team, p}, lvl{lvl} {}

Move Computer::doGetNextMove(vector<Move> &validMoves, Board *b) const
{
    //! pick from validMoves
    return generateMove(validMoves, b);
}

int Computer::getLvl() const
{
    return lvl;
}

LevelOne::LevelOne(const Colour team, const PlayerType p, const int lvl) : Computer{team, p, lvl} {}
LevelTwo::LevelTwo(const Colour team, const PlayerType p, const int lvl) : Computer{team, p, lvl} {}
LevelThree::LevelThree(const Colour team, const PlayerType p, const int lvl) : Computer{team, p, lvl} {}
LevelFour::LevelFour(const Colour team, const PlayerType p, const int lvl) : Computer{team, p, lvl} {}

LevelOne::~LevelOne() {}
LevelTwo::~LevelTwo() {}
LevelThree::~LevelThree() {}
LevelFour::~LevelFour() {}

// Level 1: random legal moves.
// Picks a random move from the list of legal moves
// DOES NOT compare moves
Move LevelOne::generateMove(vector<Move> &moves, Board *b) const
{
    // Pick a random move from the vector of possible legal moves
    return moves[rand() % moves.size()];
}

// Level 2: prefers capturing moves and checks over other moves
// Prioritizes checks and checkmates first
// Compares the value of each piece captured
Move LevelTwo::generateMove(vector<Move> &moves, Board *b) const
{
    int num_moves = moves.size();
    Move bestMove;      // Best move so far
    int bestscore = -1; // Best score so far

    // Iterate through moves
    for (Move m : moves)
    {
        // Calculate score for move
        int currscore = m.captured ? 10 : 0;

        // Update the best move and score if the score is better
        if (currscore > bestscore)
        {
            // Check if the move causes a check - increase move score
            if (b->putsPlayerInCheck(m, b->getNextPlayer()))
                currscore = 100;

            // If the score is better - use that move
            // If the score is equal - use RNG to decide whether to use this move
            if (
                (currscore > bestscore) ||
                ((currscore == bestscore) && (num_moves > 2) && ((rand() % num_moves) <= 2)))
            {
                bestscore = currscore;
                bestMove = m;
            }
        }
    }

    return bestMove; // Return the best move
}

int calculateMoveValue(const Move m, const Board *b, int scale = 1, int rec = 0)
{
    int score = m.captured ? (PIECE_VALUES[m.capturedPt]) : 0;
    int ch = b->putsPlayerInCheckMate(m, b->getNextPlayer());
    score += (ch == 0) ? 0 : ((ch == 1) ? (CHECK_VALUE) : (CHECKMATE_VALUE));

    Board tmp{*b};
    tmp.doMove(m);
    tmp.flipTurn();

    if (rec)
    {
        int nextMoveVals = 0;
        std::vector<Move> nextMoves = tmp.getValidMoves(tmp.getCurrPlayer(), false);
        for (Move m1 : nextMoves)
        {
            nextMoveVals += calculateMoveValue(m1, &tmp, scale, rec - 1);
        }

        score = (score * scale) - (nextMoveVals / nextMoves.size());
    }

    return score;
}

// Level 3: prefers avoiding capture, capturing moves, and checks
Move LevelThree::generateMove(vector<Move> &moves, Board *b) const
{
    Move bestMove;      // Best move so far
    int bestscore = -1; // Best score so far
    int first = true;

    // Iterate through moves
    for (Move m : moves)
    {
        // Calculate score for move
        int val = calculateMoveValue(m, b, 3, 1);

        if (val > bestscore || first)
        {
            bestscore = val;
            bestMove = m;

            first = false;
        }
    }

    return bestMove;
}

// Level 4: something more sophisticated
Move LevelFour::generateMove(vector<Move> &moves, Board *b) const
{
    Move bestMove;      // Best move so far
    int bestscore = -1; // Best score so far
    int first = true;

    // Iterate through moves
    for (Move m : moves)
    {
        // Calculate score for move
        int val = calculateMoveValue(m, b, 2, 2);

        if (val > bestscore || first)
        {
            bestscore = val;
            bestMove = m;

            first = false;
        }
    }

    return bestMove;
}
