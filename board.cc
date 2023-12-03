#include "board.h"
#include "util.h"
#include "textdisplay.h"
// #include "graphicsdisplay.h"

using namespace std;

static const int NUM_ROWS = 8;
static const int NUM_COLS = 8;

// independent fns and helpers
// ! [added] Static helper
static unique_ptr<Player> createPlayer(const PlayerType pt, const int level)
{
    switch (level)
    {
    case (4):
        return make_unique<LevelFour>(Colour::White, pt, level);
    case (3):
        return make_unique<LevelThree>(Colour::White, pt, level);
    case (2):
        return make_unique<LevelTwo>(Colour::White, pt, level);
    case (1):
        return make_unique<LevelOne>(Colour::White, pt, level);
    default:
        return make_unique<Human>(Colour::White, pt);
    }
}

// determine if move is within the validMoves vector
//! removed static - used in Player
//? if not used here, move to player.cc
bool moveIsValid(Move &move, vector<Move> &validMoves)
{
    for (const auto &m : validMoves)
    {
        // ! [updated] since Move has more fields, we just cmp pos to check if same move
        if (move.startPos == m.startPos && move.endPos == m.endPos)
        {
            return true;
        }
    }
    return false;
}

// ____________________________________________

// Helpers
// ! [changed] updated to handle nullptr because p->getType is undefined for nullptr.
// ! consider if this is desired for isWhite()
bool isKing(Piece *p)
{
    return p == nullptr ? false : p->getType() == PieceType::King;
}

bool isPawn(Piece *p)
{
    return p == nullptr ? false : p->getType() == PieceType::Pawn;
}

bool isWhite(Piece *p)
{
    return p == nullptr ? false : p->getColour() == Colour::White;
}

Colour getNextColour(Colour clr)
{
    return (clr == Colour::White ? Colour::Black : Colour::White);
}

//! don't make static
int getCorrectRow(int r)
{
    return abs(r - 7);
}

// ____________________________________________

// for the sake of swap only
Board::Board() : state{GameState::NA} {}

//! [added] getValidMoves getting too big and this can be reused
vector<Piece *> Board::getPlayerPieces(const Player *plr) const
{
    vector<Piece *> pieces;
    for (const vector<unique_ptr<Piece>> &col : board)
    {
        for (const unique_ptr<Piece> &loc : col)
        {
            Piece *p = loc.get();
            // Add to pieces only if colour matches
            //! [added] check p not null FIRST!
            if ((p != nullptr) && (p->getType() != PieceType::NULL_PIECE) && (p->getColour() == plr->getColour()))
            {
                pieces.emplace_back(p);
            }
        }
    }
    return pieces;
}
// WORK IN PROGRESS vvv ------------------
//? what for vvv
bool Board::checkMovePiece(const Move &m) const
{
    // Get possible moves for piece
    vector<Position> mvs = getPiece(m.startPos)->getMoves();
    // Return true if the provided move is one of these moves, false otherwise
    for (Position &p : mvs)
    {
        if (m.endPos == p)
            return true;
    }
    return false;
}

//? repeated checks?
// TODO - Add advanced logic (Double move, en passant, castling)
bool Board::checkMoveEndPos(const Move &m) const
{
    Piece *p1 = getPiece(m.startPos);
    Piece *p2 = getPiece(m.endPos);
    return (
        //? checked in pieces already?
        m.endPos.col >= 0 && // The end position is within bounds
        m.endPos.col < NUM_COLS &&
        m.endPos.row >= 0 &&
        m.endPos.row < NUM_ROWS &&
        (!p2 || (p2->getType() == PieceType::NULL_PIECE) || !p1 || (p1->getColour() != p2->getColour())));
}

//! gets all moves for all pieces - even for human
vector<Move> Board::getValidMoves(const Player *plr, bool experiment) const
{
    vector<Move> moves; // List of possible moves
    vector<Piece *> pieces = getPlayerPieces(plr);

    for (Piece *p : pieces)
    {
        if (!p)
        {
            continue;
        }
        // 1. get moves that conform to piece move & don't go out of bound
        vector<Position> pmoves = p->getMoves();
        for (const Position &ep : pmoves)
        {
            Move m{p->getPosition(), ep};
            // 2. check moves that land us on valid spot
            if (checkMoveEndPos(m))
            {
                //! add capture info
                if (getPiece(ep))
                {
                    m.captured = true;
                    m.capturedPt = getPiece(ep)->getType();
                }
                moves.emplace_back(m);
            }
        }
    }
    //! when a player A has the opponent B in check, even A moves that put B the attacker
    //! in check are moves that A can't make - we can't uncheck ourselves by checking the opponent
    //! that's why the following checks should be omitted when in experiment
    if (!experiment)
    { // 3. are we in check
        if (isPlayerInCheck(plr))
        {
            moves = getMovesToUncheck(moves);
        }
        else
        { // 4. will move put us in check
            vector<Move> newMoves;
            for (const Move &m : moves)
            {
                if (!putsPlayerInCheck(m, plr))
                {
                    newMoves.emplace_back(m);
                }
            }
            moves = newMoves;
        }
    }

    return moves;
}

//! [added]
Player *Board::getNextPlayer() const
{
    return (currPlayer == whitePlayer.get()) ? blackPlayer.get() : whitePlayer.get();
}

//* returns moves that uncheck us AND don't put us in some other check either
vector<Move> Board::getMovesToUncheck(vector<Move> &moves) const
{
    // if we made the move, none of the opponent moves can put us in check
    vector<Move> movesToUncheck;
    for (const Move &m : moves)
    {
        if (!putsPlayerInCheck(m, currPlayer))
        {
            movesToUncheck.emplace_back(m);
        }
    }
    return movesToUncheck;
}

// checks if making move m puts player p in check - makes move and calls isPlayerInCheck
bool Board::putsPlayerInCheck(const Move &m, const Player *plr) const
{
    Board tmp{*this};
    tmp.board[m.endPos.col][m.endPos.row].reset(getPiece(m.startPos));
    tmp.board[m.startPos.col][m.startPos.row].reset(nullptr);
    return tmp.isPlayerInCheck(plr);
}

// checks in case it were the other player turn, if they could make a valid move that captures our king
bool Board::isPlayerInCheck(const Player *plr) const
{
    Board tmp{*this};
    // 1. switch player turns
    tmp.setTurn(plr->getColour());
    tmp.flipTurn();

    //! getValidMoves in experiment=true will stop inf recursion
    // 2. get valid moves
    vector<Move> moves = tmp.getValidMoves(tmp.getCurrPlayer(), true);

    // 3. check if any move captures [plr] king
    for (const auto &m : moves)
    {
        if (m.captured && m.capturedPt == PieceType::King)
        {
            return true;
        }
    }
    return false;
}

bool Board::isPlayerCheckmated(const Player *plr) const
{
    return isPlayerInCheck(plr) && getValidMoves(plr).empty();
}

bool Board::isPlayerStalemated(const Player *plr) const
{
    return !isPlayerInCheck(plr) && getValidMoves(plr).empty();
}

//! will prob not work with moves that are complex like en passant - UPDATE LATER
void Board::doMove(const Move &m)
{
    delPiece(m.endPos);
    Piece &p = *getPiece(m.startPos);
    addPiece(p.getType(), p.getColour(), m.endPos);
    delPiece(m.startPos);
    notifyObservers({m.startPos, m.endPos});
}

//! [update] makeMove returns Position and leaves checking for getNextMove(validMoves)
//? to be improved to return a move error object instead of Position
Position Board::makeMove()
{
    vector<Move> validMoves = getValidMoves(currPlayer, false);
    // TODO add pawn capture moves to validMoves !!!!
    Move move = currPlayer->getNextMove(validMoves);
    // check if move valid
    if (move.endPos.col >= 0)
    {
        doMove(move);
    }
    return move.endPos;
}

// Default board, you are white
// ! row indices are flipped to match board layout
Board::Board(const PlayerType whitePl, const int whiteLevel, const PlayerType blackPl, const int blackLevel) : state{GameState::NA}
{
    // set up players
    // ! [changed] Player/Computer is an ABC - can't instantiate directly
    whitePlayer = createPlayer(whitePl, whiteLevel); //? need move
    blackPlayer = createPlayer(blackPl, blackLevel);
    currPlayer = whitePlayer.get();
}

//! does not copy observers since we will never have more than one active board at once
//? are observers correctly set to empty vector (no deep copy! or will change displays as we experiment)
Board::Board(const Board &other) : observers{},
                                   whitePlayer{isHuman(other.whitePlayer.get()) ? createPlayer(PlayerType::Human, 0) : createPlayer(PlayerType::Computer, (static_cast<Computer *>(other.whitePlayer.get()))->getLvl())},
                                   blackPlayer{isHuman(other.blackPlayer.get()) ? createPlayer(PlayerType::Human, 0) : createPlayer(PlayerType::Computer, (static_cast<Computer *>(other.blackPlayer.get()))->getLvl())},
                                   currPlayer{isWhiteTeam(other.currPlayer) ? whitePlayer.get() : blackPlayer.get()},
                                   whiteScore{other.whiteScore}, blackScore{other.blackScore}
{
    // SETUP BOARD
    board.resize(NUM_COLS);
    for (int i = 0; i < NUM_COLS; ++i)
    {
        board[i].resize(NUM_ROWS);
    }

    for (int i = 0; i < NUM_COLS; ++i)
    {
        for (int j = 0; j < NUM_ROWS; ++j)
        {
            Piece *p = (other.board[i][j]).get();
            if (p)
            {
                board[i][j] = createPiece(p->getType(), p->getColour(), p->getPosition());
            }
        }
    }
}

void Board::clearBoard()
{
    // clear observers and board
    //? if this doesn't work, use reset
    Board tmp;
    swap(observers, tmp.observers);
    swap(board, tmp.board);
    swap(state, tmp.state);
}

void Board::notifyObservers(Position pos) const
{
    Piece *p = getPiece(pos);
    for (const unique_ptr<Observer> &obs : observers)
    {
        obs.get()->notify(pos, p);
    }
}

void Board::notifyObservers(std::vector<Position> vec) const
{
    std::vector<std::pair<Position, Piece *>> vec1;
    for (Position p : vec) {
        vec1.push_back({p, getPiece(p)});
    }

    for (const unique_ptr<Observer> &obs : observers)
    {
        obs.get()->notify(vec1);
    }
}

void Board::updateObservers() const
{
    for (const unique_ptr<Observer> &obs : observers)
    {
        obs.get()->update();
    }
}

void Board::attach(unique_ptr<Observer> o)
{
    observers.emplace_back(std::move(o)); //? does this work? move needed!
}

void Board::initBoard()
{
    if (!observers.empty())
    {
        clearBoard();
    }
    state = GameState::Play;

    currPlayer = whitePlayer.get();
    // SETUP BOARD
    board.resize(NUM_COLS);
    for (int i = 0; i < NUM_ROWS; ++i)
    {
        board[i].resize(NUM_ROWS);
    }

    // ? need move ?
    for (int i = 0; i < NUM_ROWS; ++i)
    {
        // pawns
        board[i][1] = createPiece(PieceType::Pawn, Colour::Black, Position{i, 1});
        board[i][6] = createPiece(PieceType::Pawn, Colour::White, Position{i, 6});

        // rooks
        if (i == 0 || i == 7)
        {
            board[i][0] = createPiece(PieceType::Rook, Colour::Black, Position{i, 0});
            board[i][7] = createPiece(PieceType::Rook, Colour::White, Position{i, 7});

        } // knights
        else if (i == 1 || i == 6)
        {
            board[i][0] = createPiece(PieceType::Knight, Colour::Black, Position{i, 0});
            board[i][7] = createPiece(PieceType::Knight, Colour::White, Position{i, 7});

        } // bishops
        else if (i == 2 || i == 5)
        {
            board[i][0] = createPiece(PieceType::Bishop, Colour::Black, Position{i, 0});
            board[i][7] = createPiece(PieceType::Bishop, Colour::White, Position{i, 7});
        }
        if (i == 3)
        {
            board[i][0] = createPiece(PieceType::Queen, Colour::Black, Position{i, 0});
            board[i][7] = createPiece(PieceType::Queen, Colour::White, Position{i, 7});
        }
        if (i == 4)
        {
            board[i][0] = createPiece(PieceType::King, Colour::Black, Position{i, 0});
            board[i][7] = createPiece(PieceType::King, Colour::White, Position{i, 7});
        }
    }                                                         // board setup loop
    unique_ptr<Observer> td = make_unique<TextDisplay>(this); // todo update when td ctor is done
    // unique_ptr<Observer> gd = make_unique<GraphicsDisplay>(); // todo update when gd ctor is done
    attach(std::move(td));
    // attach(move(gd));
}

//* SETUP mode methods
bool Board::boardIsValid() const
{
    int blackKing = 0;
    int whiteKing = 0;
    // exactly one w/b king
    for (int i = 0; i < NUM_COLS; ++i)
    {
        for (int j = 0; j < NUM_ROWS; ++j)
        {
            Piece *p = (board[i][j]).get();
            if (p == nullptr)
            {
                continue;
            }

            blackKing += (!isWhite(p) && isKing(p)) ? 1 : 0;
            whiteKing += (isWhite(p) && isKing(p)) ? 1 : 0;
        }
    }

    if (blackKing != 1 || whiteKing != 1)
    {
        return false;
    }

    // 2. no pawn on first or last row
    for (int i = 0; i < NUM_COLS; ++i)
    {
        for (int j = 0; j < NUM_ROWS; ++j)
        {
            Piece *p1 = board[i][0].get(); // first row
            Piece *p2 = board[i][7].get(); // last row
            if (isPawn(p1) || isPawn(p2))
            {
                return false;
            }
        }
    }
    // 3. no king in check
    if (isPlayerInCheck(whitePlayer.get()) || isPlayerInCheck(blackPlayer.get()))
    {
        return false;
    }
    return true;
} // end of boardIsValid()

void Board::addPiece(const PieceType &pt, const Colour &clr, const Position &pos)
{
    auto newPiece = createPiece(pt, clr, pos);
    board[pos.col][pos.row].reset(newPiece.get());
}

//! TO AMOL!!!!! THIS DOESN'T UPDATE BOARD PROPERLY WHEN THERE'S NO PIECE AT POS 
//! IT TOGGLES THE SQAURE DISPLAY WHEN IT SHOULDN'T CHANGE THE BOARD IN THAT CASE
// ! AND JUST REDISPLAY IT!
void Board::delPiece(const Position &pos)
{
    board[pos.col][pos.row].reset(nullptr);
}

void Board::flipTurn()
{
    currPlayer = (currPlayer == blackPlayer.get()) ? whitePlayer.get() : blackPlayer.get();
}

void Board::setTurn(Colour clr)
{
    if (clr == Colour::White)
    {
        currPlayer = whitePlayer.get();
    }
    else
    {
        currPlayer = blackPlayer.get();
    }
}

// getters
GameState Board::getState() const
{
    return state;
}

Player *Board::getPlayerByColour(Colour clr) const
{
    return (clr == Colour::Black) ? blackPlayer.get() : whitePlayer.get();
}

Player *Board::getCurrPlayer() const
{
    return currPlayer;
}

Piece *Board::getPiece(const Position &pos) const
{
    return (board[pos.col][pos.row]).get();
}

Piece *Board::getPieceByCoords(int c, int r) const
{
    return (board[c][r]).get();
}

float Board::getScore(Colour clr)
{
    return (clr == Colour::Black) ? blackScore : whiteScore;
}

void Board::setState(GameState state)
{
    this->state = state;
}

void Board::incrementScore(Colour clr, float addTo)
{
    if (clr == Colour::Black)
    {
        blackScore += addTo;
    }
    else
    {
        whiteScore += addTo;
    }
}

