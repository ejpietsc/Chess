#include "board.h"
#include "textdisplay.h"
#include "graphicsdisplay.h"

using namespace std;

// ! renamed globals for clarity - col/row are also board class members
static const int NUM_ROWS = 8;
static const int NUM_COLS = 8;

// ! [added] Static helper
static unique_ptr<Player> createPlayer(const PlayerType pt, const int level) {
    switch(level) {
        case (4):
            return move(make_unique<LevelFour>(Colour::White, pt, level));
        case (3):
            return move(make_unique<LevelThree>(Colour::White, pt, level));
        case (2):
            return move(make_unique<LevelTwo>(Colour::White, pt, level));
        case (1):
            return move(make_unique<LevelOne>(Colour::White, pt, level));
        default:
            return move(make_unique<Human>(Colour::White, pt));
    }
}

// ____________________________________________

// Private Helpers
// ! [changed] updated to handle nullptr because p->getType is undefined for nullptr. consider if this is desired for isWhite()
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

// ____________________________________________

// for the sake of swap only
Board::Board() : state{GameState::NA} {}


// TODO vvv ------------------
bool Board::isPlayerInCheck(Player *plr) const {
    cout << "-Incomplete method-" << endl;
    return true;
}

bool Board::isPlayerCheckmated(Player *plr) const {
    cout << "-Incomplete method-" << endl;
    return true;
}

bool Board::isPlayerStalemated(Player *plr) const {
    cout << "-Incomplete method-" << endl;
    return true;
}

vector<Move> Board::getValidMoves(Player *plr) {
    cout << "-Incomplete method-" << endl;
    vector<Move> v;
    return v;
}

// TODO ^^^ ------------------


// Default board, you are white
// ! row indices are flipped to match board layout
Board::Board(const PlayerType whitePl, const int whiteLevel, const PlayerType blackPl, const int blackLevel) : state{GameState::NA}
{
    // set up players
    // ! [changed] Player/Computer is an ABC - can't instatiate directly
    whitePlayer = createPlayer(whitePl, whiteLevel);
    blackPlayer = createPlayer(blackPl, blackLevel);
    currPlayer = whitePlayer.get();
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

void Board::notifyObservers(Position pos, Piece *p) const {
    for (const unique_ptr<Observer>& obs : observers) {
        obs.get()->notify(pos, p);
    }
}

void Board::attach(unique_ptr<Observer> o)
{
    observers.push_back(move(o)); //? does this work? move needed!
}

void Board::initBoard()
{
    // ! [changed] set state AFTER clearBoard, because clearBoard invokes the default ctor which creates a board with state GameState::NA
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
    }                                                  // board setup loop
    unique_ptr<Observer> td = make_unique<TextDisplay>(this); // todo update when td ctor is done
    // unique_ptr<Observer> gd = make_unique<GraphicsDisplay>(); // todo update when gd ctor is done
    attach(move(td));
    // attach(move(gd));
}

//* SETUP mode methods
bool Board::boardIsValid() const
{
    int blackKing = 0;
    int whiteKing = 0;
    // exactly one w/b king
    // ! [changed] check # of kings loop
    for (int i = 0; i < NUM_COLS; ++i) {
        for (int j = 0; j < NUM_ROWS; ++j) {
            Piece *p = (board[i][j]).get();
            if (p == nullptr) {
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

    // ! this doesn't iterate all of the pieces properly
   /* for (const auto &col : board)
    {
        for (const auto &piece : col)
        {
            if (piece && piece.get() != nullptr)
            {
                cout << piece->getCol() << " " << piece->getRow() << endl;
                Piece *p = piece.get();
                blackKing += (!isWhite(p) && isKing(p)) ? 1 : 0;
                whiteKing += (isWhite(p) && isKing(p)) ? 1 : 0;
            }
        }
        if (blackKing != 1 || whiteKing != 1)
        {
            cout << "KING";
            return false;
        }
    }*/

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

void Board::addPiece(PieceType pt, Colour clr, Position pos)
{
    auto newPiece = createPiece(pt, clr, pos);
    board[pos.col][pos.row].reset(newPiece.get());
    notifyObservers(pos, newPiece.get()); // NEW!
}

void Board::delPiece(Position pos)
{
    board[pos.col][pos.row].reset(nullptr);
    notifyObservers(pos, nullptr); // NEW!
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

Piece *Board::getPiece(Position pos)
{
    return board[pos.col][pos.row].get();
}

Piece *Board::getPieceByCoords(int c, int r)
{
    return board[c][r].get();
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
    if (clr == Colour::Black) blackScore += addTo;
    else whiteScore += addTo;
}

// determine if move is within the validMoves vector
static bool moveIsValid(Move &move, vector<Move> &validMoves)
{
    for (const auto &childMove : validMoves)
    {
        if (move == childMove) return true;
    }
    return false;
}

// todo update: called right after move cmd is read
bool Board::makeMove()
{
    Move move = currPlayer->getNextMove();

    vector<Move> validMoves = getValidMoves(currPlayer);

    if (moveIsValid(move, validMoves))
    {
        Piece *pieceToMove = getPiece(move.startPos);

        if (pieceToMove != nullptr)
        { // a valid move has occurred!
            pieceToMove->makeMove(move.endPos);
            setTurn(getNextColour(currPlayer->getColour()));
            return true;
        }
    } // else do ntg here

    return false;
}
