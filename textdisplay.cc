#include "textdisplay.h"
#include <iostream>

using namespace std;

static const int NUM_COLS = 8;
static const int NUM_ROWS = 8;

// Priv
int abs(int a)
{
    return (a > 0) ? a : -a;
}

SquareColor getSquareColor(int c, int r)
{
    int sum = c + abs(r - 7); // flip back row indices
    return (sum % 2 == 0) ? SquareColor::Dark : SquareColor::Light;
}

char squareCharDisplay(int c, int r)
{
    int sum = c + abs(r - 7);
    return (sum % 2 == 0) ? '_' : ' ';
}

char getPieceChar(Piece *p) {
    char ans = ' ';

    switch (p->getType())
    {
        case PieceType::King:
            ans = 'K';
            break;
        case PieceType::Queen:
            ans = 'Q';
            break;
        case PieceType::Bishop:
            ans = 'B';
            break;
        case PieceType::Rook:
            ans = 'R';
            break;
        case PieceType::Knight:
            ans = 'N';
            break;
        case PieceType::Pawn:
            ans = 'P';
            break;

        default:
            cout << "ERROR : Attempt to set up NULL_PIECE";
            break;
    }

    ans = p->getColour() == Colour::White ? ans : tolower(ans);

    return ans;
}

// ! updated to handle the fact that getPieceByCoords can return nullptr 
TextDisplay::TextDisplay(Board *subject)
{
    for (int c = 0; c < NUM_COLS; ++c) {
        theDisplay.push_back(vector<char>(NUM_COLS, ' '));
        for (int r = 0; r < NUM_ROWS; ++r) {
            Piece *p = subject->getPieceByCoords(c, r);
            theDisplay[c][r] = p == nullptr ? squareCharDisplay(c, r) : getPieceChar(p);
        }
    }

    cout << *this << endl;
}

void TextDisplay::doNotify(pair<int, int> pos, Piece *p)
{
    theDisplay[pos.first][pos.second] = p == nullptr ? squareCharDisplay(pos.first, pos.second) : getPieceChar(p);
}

TextDisplay::~TextDisplay() {}

ostream &operator<<(ostream &out, const TextDisplay &td)
{
    char rowDigit = '8';

    for (int r = 0; r < NUM_ROWS; ++r)
    {
        out << rowDigit-- << " ";
        for (const auto &col : td.theDisplay) out << col[r];
        out << endl;
    }
    out << endl << "  ";
    for (char c = 'a'; c <= 'h'; ++c) out << c;
    out << endl;
    return out;
}
