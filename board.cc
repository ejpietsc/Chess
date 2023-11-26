#include "board.h"

using namespace std;

static const int row = 8;
static const int col = 8;

// for the sake of swap only
Board::Board() : state{GameState::NA} {}

// Default board, you are white
// ! row indices are flipped to match board layout
Board::Board(PlayerType whitePl, const int whiteLevel, PlayerType blackPl, const int blackLevel) : state{GameState::NA}
{
    // set up players
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
    Board tmp;
    swap(observers, tmp.observers);
    swap(board, tmp.board);
    swap(state, tmp.state);
}

void Board::attach(unique_ptr<Observer> o)
{
    observers.push_back(move(o)); // ! does this work?
}

void Board::initBoard()
{
    state = GameState::Play;
    if (!observers.empty())
    {
        clearBoard();
    }
    unique_ptr<Observer> td = make_unique<Observer>(); // todo update when td ctor is done
    unique_ptr<Observer> gd = make_unique<Observer>(); // todo update when gd ctor is done
    attach(move(td));
    attach(move(gd));

    currPlayer = whitePlayer.get();
    // SETUP BOARD
    board.resize(col);
    for (int i = 0; i < row; ++i)
    {
        board[i].resize(row);
    }
    // pawns
    for (int i = 0; i < row; ++i)
    {
        // pawns
        board[i][1] = move(make_unique<Piece>(PieceType::Pawn, Colour::Black, Position{i, 1}));
        board[i][6] = move(make_unique<Piece>(PieceType::Pawn, Colour::White, Position{i, 6}));
        // rooks
        if (i == 0 || i == 7)
        {
            board[i][0] = move(make_unique<Piece>(PieceType::Rook, Colour::Black, Position{i, 0}));
            board[i][7] = move(make_unique<Piece>(PieceType::Rook, Colour::White, Position{i, 7}));

        } // knights
        else if (i == 1 || i == 6)
        {
            board[i][0] = move(make_unique<Piece>(PieceType::Knight, Colour::Black, Position{i, 0}));
            board[i][7] = move(make_unique<Piece>(PieceType::Knight, Colour::White, Position{i, 7}));

        } // bishops
        else if (i == 2 || i == 5)
        {
            board[i][0] = move(make_unique<Piece>(PieceType::Bishop, Colour::Black, Position{i, 0}));
            board[i][7] = move(make_unique<Piece>(PieceType::Bishop, Colour::White, Position{i, 7}));
        }
        if (i == 3)
        {
            board[i][0] = move(make_unique<Piece>(PieceType::Queen, Colour::Black, Position{i, 0}));
            board[i][7] = move(make_unique<Piece>(PieceType::Queen, Colour::White, Position{i, 7}));
        }
        if (i == 4)
        {
            board[i][0] = move(make_unique<Piece>(PieceType::King, Colour::Black, Position{i, 0}));
            board[i][7] = move(make_unique<Piece>(PieceType::King, Colour::White, Position{i, 7}));
        }
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

Colour Board::getOtherColour(Colour clr) const
{
    return (clr == Colour::White ? Colour::Black : Colour::White);
}

// todo update: called right after move cmd is read
//  ! modified fn to return bool
bool Board::makeMove()
{
    Move move = currPlayer->getNextMove();

    vector<Move> validMoves = getValidMoves(*currPlayer);

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
