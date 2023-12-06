#include "board.h"
#include "util.h"
#include "textdisplay.h"
#include "graphicsdisplay.h"

using namespace std;

// Helpers and Standalone fns
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

// used by graphics/textdisplays to index differently
int getRowForOutput(int r)
{
    return abs(r - NUM_ROWS + 1);
}

int isColour(Piece *p, const Colour &col)
{
    return (p->getColour() == col);
}

int isPieceType(Piece *p, const PieceType &type)
{
    return (p->getType() == type);
}

int getPromotedCount(int n, bool isQueen = false)
{
    if (isQueen)
    {
        return (n <= 1) ? 0 : (n - 1);
    }
    return (n <= 2) ? 0 : (n - 2);
}

bool isCastleMove(const Move &move, const Board &b)
{
    Piece *p = b.getPieceByPos(move.startPos);
    return p && p->getType() == PieceType::King &&
           abs(move.startPos.col - move.endPos.col) == 2;
}

static unique_ptr<Player> createPlayer(const PlayerType pt, const int level, const Colour clr)
{
    switch (level)
    {
    case (4):
        return make_unique<LevelFour>(clr, pt, level);
    case (3):
        return make_unique<LevelThree>(clr, pt, level);
    case (2):
        return make_unique<LevelTwo>(clr, pt, level);
    case (1):
        return make_unique<LevelOne>(clr, pt, level);
    default:
        return make_unique<Human>(clr, pt);
    }
}

// determine if move is within the validMoves vector
bool moveIsValid(Move &move, vector<Move> &validMoves, char mode)
{
    switch (mode)
    {
    case ('n'):
        for (const auto &m : validMoves)
        {
            if (move.startPos == m.startPos && move.endPos == m.endPos)
            {
                return true;
            }
        }
        return false;
    case ('e'):
        for (const auto &m : validMoves)
        {
            if (move.startPos == m.startPos && move.endPos == m.endPos && move.captured == m.captured && move.capturedPt == m.capturedPt && move.enPassentCapture == m.enPassentCapture &&
                move.epCaptureLoc == m.epCaptureLoc)
            {
                return true;
            }
        }
        return false;
    case ('p'):
        for (const auto &m : validMoves)
        {
            if (move.startPos == m.startPos && move.endPos == m.endPos && move.upgradePiece == m.upgradePiece &&
                move.upgradeTo == m.upgradeTo)
            {
                return true;
            }
        }
        return false;
    case ('k'):
        for (const auto &m : validMoves)
        {
            if (move.startPos == m.startPos && move.endPos == m.endPos && move.isCastleMove == m.isCastleMove)
            {
                return true;
            }
        }
        return false;
    case ('c'):
        for (const auto &m : validMoves)
        {
            if (move.startPos == m.startPos && move.endPos == m.endPos && move.captured == m.captured)
            {
                return true;
            }
        }
        return false;
    default:
        return false;
    }
}
// end of helpers and standalone fns ____________________________________________

// for the sake of swap only
Board::Board() : text{false}, graphics{false} {}

vector<Piece *> Board::getPlayerPieces(const Player *plr) const
{
    vector<Piece *> pieces;
    for (const vector<unique_ptr<Piece>> &col : board)
    {
        for (const unique_ptr<Piece> &loc : col)
        {
            Piece *p = loc.get();
            // Add to pieces only if colour matches
            if ((p != nullptr) && (p->getColour() == plr->getColour()))
            {
                pieces.emplace_back(p);
            }
        }
    }
    return pieces;
}

// return true if there is a Piece between m.startPos and m.endPos (non-inclusive)
static bool pieceInTheWay(const Board &board, const Move &m)
{
    const int xDif = m.endPos.col - m.startPos.col;
    const int yDif = m.endPos.row - m.startPos.row;

    if (abs(xDif) <= 1 && abs(yDif) <= 1)
    { // we're not travelling far enough to care
        return false;
    }

    int xMult = 1;
    int yMult = 1;

    if (xDif < 0)
    {
        xMult = -1;
    }
    if (yDif < 0)
    {
        yMult = -1;
    }

    if (xDif == 0)
    { // straight up/down
        for (int i = 1; i <= abs(yDif) - 1; ++i)
        {
            Position pos{m.startPos.col, m.startPos.row + (i * yMult)};
            const Piece *p = board.getPiece(pos);
            if (p != nullptr)
            {
                return true;
            }
        }
    }

    if (yDif == 0)
    { // straight left/right
        for (int i = 1; i <= abs(xDif) - 1; ++i)
        {
            Position pos{m.startPos.col + (i * xMult), m.startPos.row};
            const Piece *p = board.getPiece(pos);
            if (p != nullptr)
            {
                return true;
            }
        }
    }

    if (abs(xDif) == abs(yDif))
    { // diagonals
        for (int i = 1; i <= abs(xDif) - 1; ++i)
        {
            Position pos{m.startPos.col + (i * xMult), m.startPos.row + (i * yMult)};
            const Piece *p = board.getPiece(pos);
            if (p != nullptr)
            {
                return true;
            }
        }
    }

    return false;
}

bool Board::checkMoveEndPos(const Move &m) const
{
    Piece *p1 = getPiece(m.startPos);
    Piece *p2 = getPiece(m.endPos);

    if (!p1)
        return false;

    PieceType p1t = p1->getType();

    if (p1t == PieceType::Pawn && p2 && p1->getPosition().col == p2->getPosition().col)
    { // remove pawn forward captures
        return false;
    }

    // remove pawn moving diagonally when there is no capture
    if (p1t == PieceType::Pawn && !p2 && p1->getPosition().col != m.endPos.col)
    {
        // Check for en passent capture
        Piece *p3 = getPieceByCoords(m.endPos.col, m.startPos.row);
        if (
            p3 &&
            p3->getType() == PieceType::Pawn &&
            p3->getColour() != p1->getColour() &&
            dynamic_cast<Pawn *>(p3)->getDoubleMoved())
        {
        }
        // Otherwise, the move is invalid
        else
        {
            return false;
        }
    }

    if (p1t != PieceType::Knight && pieceInTheWay(*this, m))
    { // remove pieces jumping over others
        return false;
    }

    return (
        m.endPos.col >= 0 && // The end position is within bounds
        m.endPos.col < NUM_COLS &&
        m.endPos.row >= 0 &&
        m.endPos.row < NUM_ROWS &&
        (!p2 || !p1 || (p1->getColour() != p2->getColour())));
}

bool Board::isCaslteValid(const Piece *rook, const Player *plr, bool isLeftCaslte) const
{
    if (!rook)
    {
        return false;
    }
    bool rookNotMoved = rook && (rook->getType() == PieceType::Rook) && !dynamic_cast<const Rook *>(rook)->getHasMoved();
    Position kingSPos = (rook->getColour() == Colour::White) ? Position{4, 0} : Position{4, 7};
    // 2. any pieces btw two
    bool piecesBlocking;
    // 3. king not in check in 3 pos: starting-middle-end
    bool inCheck;

    if (isLeftCaslte)
    {
        piecesBlocking = getPieceByCoords(kingSPos.col - 1, kingSPos.row) ||
                         getPieceByCoords(kingSPos.col - 2, kingSPos.row) ||
                         getPieceByCoords(kingSPos.col - 3, kingSPos.row);
        inCheck = isPlayerInCheck(plr) &&
                  putsPlayerInCheck(Move{kingSPos, Position{kingSPos.col - 1, kingSPos.row}}, plr) &&
                  putsPlayerInCheck(Move{kingSPos, Position{kingSPos.col - 2, kingSPos.row}}, plr);
    }
    else
    {
        piecesBlocking = getPieceByCoords(kingSPos.col + 1, kingSPos.row) ||
                         getPieceByCoords(kingSPos.col + 2, kingSPos.row);
        inCheck = isPlayerInCheck(plr) &&
                  putsPlayerInCheck(Move{kingSPos, Position{kingSPos.col + 1, kingSPos.row}}, plr) &&
                  putsPlayerInCheck(Move{kingSPos, Position{kingSPos.col + 2, kingSPos.row}}, plr);
    }

    return rookNotMoved && !piecesBlocking && !inCheck;
}

vector<Move> Board::getCastlingMoves(const Player *plr) const
{
    Position whiteKingPos = Position{4, 0};
    Position blackKingPos = Position{4, 7};

    Position leftWhiteRookStart = Position{0, 0};
    Position rightWhiteRookStart = Position{7, 0};

    Position leftBlackRookStart = Position{0, 7};
    Position rightBlackRookStart = Position{7, 7};

    vector<Move> moves;
    if (plr->getColour() == Colour::White)
    {
        // check left castle
        if (isCaslteValid(getPieceByPos(leftWhiteRookStart), plr, true))
        {
            moves.emplace_back(Move{whiteKingPos, Position{whiteKingPos.col - 2, whiteKingPos.row}, true});
        }

        if (isCaslteValid(getPieceByPos(rightWhiteRookStart), plr))
        {
            moves.emplace_back(Move{whiteKingPos, Position{whiteKingPos.col + 2, whiteKingPos.row}, true});
        }
    }
    else
    {
        // check left castle
        if (isCaslteValid(getPieceByPos(leftBlackRookStart), plr, true))
        {
            moves.emplace_back(Move{blackKingPos, Position{blackKingPos.col - 2, blackKingPos.row}, true});
        }

        if (isCaslteValid(getPieceByPos(rightBlackRookStart), plr))
        {
            moves.emplace_back(Move{blackKingPos, Position{blackKingPos.col + 2, blackKingPos.row}, true});
        }
    }
    return moves;
}

// gets all moves for all pieces - even for human
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
        //! add capture=true for pawn capturing moves
        for (const Position &ep : pmoves)
        {
            Move m{p->getPosition(), ep};

            // 2. check moves that land us on valid spot
            if (checkMoveEndPos(m))
            {
                Position epp = Position{ep.col, p->getPosition().row};
                Piece *ep_piece = getPiece(epp);

                // Piece capturing
                if (getPiece(ep) != nullptr)
                {
                    m.captured = true;
                    m.capturedPt = getPiece(ep)->getType();
                }

                // En Passant capture
                if (
                    ep_piece &&
                    ep_piece->getType() == PieceType::Pawn &&
                    ep_piece->getColour() != p->getColour() &&
                    dynamic_cast<Pawn *>(ep_piece)->getDoubleMoved())
                {
                    m.captured = true;
                    m.capturedPt = PieceType::Pawn;
                    m.enPassentCapture = true;
                    m.epCaptureLoc = epp;
                }

                // Castling moves
                if (!experiment && p->getType() == PieceType::King && 
                    ( (p->getColour() == Colour::White && p->getPosition() == Position{4, 0}) || (p->getColour() == Colour::Black && p->getPosition() == Position{4, 7}) ) // ! hotfix
                    && !(dynamic_cast<King *>(p)->getHasMoved()))
                {
                    auto castlingMoves = getCastlingMoves(plr);
                    int len = castlingMoves.size();
                    for (int i = 0; i < len; ++i)
                    {
                        castlingMoves[i].isCastleMove = true;
                        moves.emplace_back(castlingMoves[i]);
                    }
                }

                // Pawn promotion
                if (
                    ep_piece &&
                    ep_piece->getType() == PieceType::Pawn &&
                    (ep.row == 0 ||
                     ep.row == NUM_ROWS - 1))
                {
                    m.upgradePiece = true;
                    for (PieceType p : std::vector<PieceType>{PieceType::Queen, PieceType::Rook, PieceType::Bishop, PieceType::Knight})
                    {
                        m.upgradeTo = p;
                        moves.emplace_back(m);
                    }
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
            // !! debug output
            if (debugOutput)
            {
                for (Move &m : moves)
                {
                    cout << "Moves to uncheck you:" << m.startPos.col << " " << m.startPos.row << " to " << m.endPos.col << " " << m.endPos.row << endl;
                }
            }
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

        // !! debug output
        if (debugOutput)
        {
            cout << "Valid Moves:\n---------" << endl;
            for (Move &m : moves)
            {
                cout << posToStr(m.startPos) << " to " << posToStr(m.endPos) << endl;
            }
        }
    }

    return moves;
}

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
    tmp.delPiece(m.endPos);

    Piece *p = tmp.getPiece(m.startPos);
    if (p) {
        p->movePiece(m.endPos);
    }

    tmp.board[m.endPos.col][m.endPos.row].swap(tmp.board[m.startPos.col][m.startPos.row]);
    return tmp.isPlayerInCheck(plr);
}

// 0: none
// 1: check
// 2: checkmate
int Board::putsPlayerInCheckMate(const Move &m, const Player *plr) const
{
    Board tmp{*this};
    tmp.delPiece(m.endPos);

    Piece *p = tmp.getPiece(m.startPos);
    if (p) {
        p->movePiece(m.endPos);
    }

    tmp.board[m.endPos.col][m.endPos.row].swap(tmp.board[m.startPos.col][m.startPos.row]);

    if (tmp.isPlayerInCheck(plr))
    {
        if (tmp.getValidMoves(plr).empty())
            return 2;
        return 1;
    }
    return 0;
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
            if (debugOutput)
            {
                if (plr->getColour() == Colour::Black)
                {
                    cout << "BLACK IN CHECK" << endl;
                }
                else
                {
                    cout << "WHITE IN CHECK" << endl;
                }
                cout << " >> Move that captures king: " << m.startPos.col << m.startPos.row << " " << m.endPos.col << m.endPos.row << endl;
            }
            return true;
        }
    }
    return false;
}

bool Board::isPlayerCheckmated(const Player *plr) const
{
    //! for debug purposes
    // auto moves = getValidMoves(plr);
    // for (const auto &m : moves)
    // {
    //     cout << "MOVES THAT SAVE ME: " << m.startPos.col << " " << m.startPos.row << " to " << m.endPos.col << " " << m.endPos.row << endl;
    // }
    // // !___________________________________________________________...
    return isPlayerInCheck(plr) && getValidMoves(plr).empty();
}

bool Board::isPlayerStalemated(const Player *plr) const
{
    return !isPlayerInCheck(plr) && getValidMoves(plr).empty();
}

void Board::doMove(const Move &m)
{
    std::vector<Position> toNotify = {m.startPos, m.endPos};

    if (m.enPassentCapture)
    {
        delPiece(m.epCaptureLoc);
        toNotify.emplace_back(m.epCaptureLoc);
    }

    if (m.isCastleMove)
    {
        // identify if white or black castle
        bool iswhite = m.startPos.row == 0;
        int rookRow = iswhite ? 0 : 7;
        int rookCol = (m.startPos.col - m.endPos.col > 0) ? (m.startPos.col - 1) : (m.startPos.col + 1);
        Position rookEndPos = Position{rookCol, rookRow};
        rookCol = (m.startPos.col - m.endPos.col > 0) ? 0 : 7;
        Position rookStartPos = Position{rookCol, rookRow};

        getPiece(rookStartPos)->movePiece(rookEndPos);
        board[rookStartPos.col][rookStartPos.row].swap(board[rookEndPos.col][rookEndPos.row]);

        toNotify.emplace_back(rookStartPos);
        toNotify.emplace_back(rookEndPos);
    }
    delPiece(m.endPos);
    Piece *p = getPiece(m.startPos);
    p->movePiece(m.endPos);
    board[m.startPos.col][m.startPos.row].swap(board[m.endPos.col][m.endPos.row]);

    if (m.upgradePiece)
    {
        Piece *op = getPiece(m.endPos);
        Colour c = op->getColour();
        delPiece(m.endPos);
        addPiece(m.upgradeTo, c, m.endPos);
    }

    notifyObservers(toNotify);
}

Position Board::makeMove()
{
    vector<Move> validMoves = getValidMoves(currPlayer, false);

    Move move = currPlayer->getNextMove(validMoves, this);

    if (move.endPos.col < 0)
    {
        return move.endPos;
    }

    if (isCastleMove(move, *this))
    {
        move.isCastleMove = true;
    }

    // check if move valid
    if (move.endPos.col >= 0)
    {
        doMove(move);
    }
    return move.endPos;
}

// Default board, you are white
// ! row indices are flipped to match board layout
Board::Board(
    const PlayerType whitePl,
    const int whiteLevel,
    const PlayerType blackPl,
    const int blackLevel,
    const bool graphics,
    const bool text,
    const bool useUnicode,
    const bool debug,
    const bool noCheat) : text{text}, graphics{graphics}, useUnicode{useUnicode}, debugOutput{debug}, noCheat{noCheat}
{
    // set up players
    // ! [changed] Player/Computer is an ABC - can't instantiate directly
    whitePlayer = createPlayer(whitePl, whiteLevel, Colour::White); //? need move
    blackPlayer = createPlayer(blackPl, blackLevel, Colour::Black);
    currPlayer = whitePlayer.get();
}

//! does not copy observers since we will never have more than one active board at once
Board::Board(const Board &other) : observers{},
                                   whitePlayer{isHuman(other.whitePlayer.get()) ? createPlayer(PlayerType::Human, 0, Colour::White) : createPlayer(PlayerType::Computer, (static_cast<Computer *>(other.whitePlayer.get()))->getLvl(), Colour::White)},
                                   blackPlayer{isHuman(other.blackPlayer.get()) ? createPlayer(PlayerType::Human, 0, Colour::Black) : createPlayer(PlayerType::Computer, (static_cast<Computer *>(other.blackPlayer.get()))->getLvl(), Colour::Black)},
                                   currPlayer{isWhiteTeam(other.currPlayer) ? whitePlayer.get() : blackPlayer.get()},
                                   whiteScore{other.whiteScore}, blackScore{other.blackScore},
                                   text{false},
                                   graphics{false},
                                   debugOutput{false},
                                   noCheat{false}
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
            else
            {
                board[i][j] = unique_ptr<Piece>{nullptr};
            }
        }
    }
}

void Board::clearBoard()
{
    // clear observers and board
    Board tmp;
    swap(observers, tmp.observers);
    swap(board, tmp.board);
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
    for (Position p : vec)
    {
        vec1.push_back({p, getPiece(p)});
    }

    for (const unique_ptr<Observer> &obs : observers)
    {
        obs.get()->notify(vec1);
    }
}

void Board::displayObservers() const
{
    for (const unique_ptr<Observer> &obs : observers)
    {
        obs.get()->display();
    }
}

void Board::refreshObservers() const
{
    for (const unique_ptr<Observer> &obs : observers)
    {
        obs.get()->refresh(this);
    }
}

void Board::attach(unique_ptr<Observer> o)
{
    observers.emplace_back(std::move(o));
}

void Board::initBoard()
{
    if (!observers.empty())
    {
        clearBoard();
    }

    currPlayer = whitePlayer.get();

    // SETUP BOARD
    board.resize(NUM_COLS);
    for (int i = 0; i < NUM_ROWS; ++i)
    {
        board[i].resize(NUM_ROWS);
    }

    for (int i = 0; i < NUM_ROWS; ++i)
    {
        for (int j = 0; j < NUM_COLS; ++j)
        {
            if (j >= 2 && j <= 5)
            {
                board[i][j] = unique_ptr<Piece>{nullptr};
            }
        }
    }

    for (int i = 0; i < NUM_ROWS; ++i)
    {
        // pawns
        board[i][1] = createPiece(PieceType::Pawn, Colour::White, Position{i, 1});
        board[i][6] = createPiece(PieceType::Pawn, Colour::Black, Position{i, 6});

        // rooks
        if (i == 0 || i == 7)
        {
            board[i][0] = createPiece(PieceType::Rook, Colour::White, Position{i, 0});
            board[i][7] = createPiece(PieceType::Rook, Colour::Black, Position{i, 7});

        } // knights
        else if (i == 1 || i == 6)
        {
            board[i][0] = createPiece(PieceType::Knight, Colour::White, Position{i, 0});
            board[i][7] = createPiece(PieceType::Knight, Colour::Black, Position{i, 7});

        } // bishops
        else if (i == 2 || i == 5)
        {
            board[i][0] = createPiece(PieceType::Bishop, Colour::White, Position{i, 0});
            board[i][7] = createPiece(PieceType::Bishop, Colour::Black, Position{i, 7});
        }
        if (i == 3)
        {
            board[i][0] = createPiece(PieceType::Queen, Colour::White, Position{i, 0});
            board[i][7] = createPiece(PieceType::Queen, Colour::Black, Position{i, 7});
        }
        if (i == 4)
        {
            board[i][0] = createPiece(PieceType::King, Colour::White, Position{i, 0});
            board[i][7] = createPiece(PieceType::King, Colour::Black, Position{i, 7});
        }
    } // board setup loop

    // Only add the text display if required
    if (text)
    {
        unique_ptr<Observer> td = make_unique<TextDisplay>(this, useUnicode);
        attach(std::move(td));
    }

    // Only add the graphics display if required
    if (graphics)
    {
        unique_ptr<Observer> gd = make_unique<GraphicsDisplay>(this);
        attach(std::move(gd));
    }
}

int Board::getPieceTypeCount(const PieceType &pt, const Colour &col) const
{
    int count = 0;
    for (int i = 0; i < NUM_COLS; ++i)
    {
        for (int j = 0; j < NUM_ROWS; ++j)
        {
            Piece *p = (board[i][j]).get();
            if (p == nullptr)
            {
                continue;
            }
            count += (isPieceType(p, pt) && isColour(p, col)) ? 1 : 0;
        }
    }
    return count;
}

bool Board::boardIsValid() const
{
    // one king per colour
    if (getPieceTypeCount(PieceType::King, Colour::Black) != 1 || getPieceTypeCount(PieceType::King, Colour::White) != 1)
    {
        return false;
    }

    if (noCheat) {
        //* FEATURE - extra check: number pawns + number promotions <= 8
        // right number of promotions (num pawns + num promotions <= 8)
        int blackPawn = getPieceTypeCount(PieceType::Pawn, Colour::Black);
        int whitePawn = getPieceTypeCount(PieceType::Pawn, Colour::White);
        int promotedWhite = 0;
        int promotedBlack = 0;
        promotedWhite = getPromotedCount(getPieceTypeCount(PieceType::Rook, Colour::White)) +
                        getPromotedCount(getPieceTypeCount(PieceType::Knight, Colour::White)) +
                        getPromotedCount(getPieceTypeCount(PieceType::Bishop, Colour::White)) +
                        getPromotedCount(getPieceTypeCount(PieceType::Queen, Colour::White), true);

        promotedBlack = getPromotedCount(getPieceTypeCount(PieceType::Rook, Colour::Black)) +
                        getPromotedCount(getPieceTypeCount(PieceType::Knight, Colour::Black)) +
                        getPromotedCount(getPieceTypeCount(PieceType::Bishop, Colour::Black)) +
                        getPromotedCount(getPieceTypeCount(PieceType::Queen, Colour::Black), true);

        if ((promotedWhite + whitePawn > 8) || (promotedBlack + blackPawn > 8))
        {
            return false;
        }
    }
    // 2. no pawn on first or last row
    for (int i = 0; i < NUM_COLS; ++i)
    {
        Piece *p1 = board[i][0].get(); // first row
        Piece *p2 = board[i][7].get(); // last row
        if (isPawn(p1) || isPawn(p2))
        {
            return false;
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
    board[pos.col][pos.row] = createPiece(pt, clr, pos);
}

void Board::delPiece(const Position &pos)
{
    board[pos.col][pos.row].reset();
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

Piece *Board::getPieceByPos(const Position &pos) const
{
    return (board[pos.col][pos.row]).get();
}

float Board::getScore(Colour clr)
{
    return (clr == Colour::Black) ? blackScore : whiteScore;
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
