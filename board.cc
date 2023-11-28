#include "board.h"

using namespace std;

static const int row = 8;
static const int col = 8;

// Private Helpers
bool isKing(Piece *p)
{
    return (p->getType() == PieceType::King);
}

bool isPawn(Piece *p)
{
    return (p->getType() == PieceType::Pawn);
}

bool isWhite(Piece *p)
{
    return (p->getColour() == Colour::White);
}

Colour getOtherColour(Colour clr)
{
    return (clr == Colour::White ? Colour::Black : Colour::White);
}
// ____________________________________________

// for the sake of swap only
Board::Board() : state{GameState::NA} {}

// Default board, you are white
// ! row indices are flipped to match board layout
Board::Board(PlayerType whitePl, const int whiteLevel, PlayerType blackPl, const int blackLevel) : state{GameState::NA}
{
    // set up players
    // ! Player is an ABC - can't instatiate - make_unique creates a Player object
    if (whiteLevel)
    {
        whitePlayer = move(make_unique<Player>(Colour::White, whitePl, whiteLevel));
    }
    else
    {
        whitePlayer = move(make_unique<Player>(Colour::White, whitePl));
    }
    if (blackLevel)
    {
        blackPlayer = move(make_unique<Player>(Colour::Black, blackPl, blackLevel));
    }
    else
    {
        blackPlayer = move(make_unique<Player>(Colour::Black, blackPl));
    }
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

void Board::attach(unique_ptr<Observer> o)
{
    observers.push_back(move(o)); //? does this work? move needed!
}

void Board::initBoard()
{
    state = GameState::Play;
    if (!observers.empty())
    {
        clearBoard();
    }

    currPlayer = whitePlayer.get();
    // SETUP BOARD
    board.resize(col);
    for (int i = 0; i < row; ++i)
    {
        board[i].resize(row);
    }

    // ? need move ?
    for (int i = 0; i < row; ++i)
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
    unique_ptr<Observer> td = make_unique<Observer>(); // todo update when td ctor is done
    unique_ptr<Observer> gd = make_unique<Observer>(); // todo update when gd ctor is done
    attach(move(td));
    attach(move(gd));
}

//* SETUP mode methods
bool Board::boardIsValid() const
{
    int blackKing = 0;
    int whiteKing = 0;
    // exactly one w/b king
    for (const auto &col : board)
    {
        for (const auto &piece : col)
        {
            if (!piece)
            {
                Piece *p = piece.get();
                blackKing += (!isWhite(p) && isKing(p)) ? 1 : 0;
                whiteKing += (isWhite(p) && isKing(p)) ? 1 : 0;
            }
        }
        if (blackKing != 1 || whiteKing != 1)
        {
            return false;
        }
    }
    // 2. no pawn on first or last row
    for (int i = 0; i < col; ++i)
    {
        for (int j = 0; j < row; ++j)
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
    // todo NOTIFY_OBSERVERS
}

void Board::delPiece(Position pos)
{
    board[pos.col][pos.row].reset(nullptr);
    // todo NOTIFY_OBSERVERS
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
    if (clr == Colour::Black)
    {
        blackScore += addTo;
    }
    else
    {
        whiteScore += addTo;
    }
}

// determine if move is within the validMoves vector
static bool moveIsValid(Move &move, vector<Move> &validMoves)
{
    for (const auto &childMove : validMoves)
    {
        if (move == childMove)
        {
            return true;
        }
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
            setTurn(getOtherColour(currPlayer->getColour()));
            return true;
        }
    } // else do ntg here

    return false;
}
