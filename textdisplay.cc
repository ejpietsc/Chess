#include "textdisplay.h"

using namespace std;

static const int col = 8;
static const int row = 8;

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

// ! updated to handle the fact that getPieceByCoords can return nullptr 
TextDisplay::TextDisplay(Board *subject)
{
    for (int c = 0; c < col; ++c)
    {
        theDisplay.push_back(vector<char>(col, ' '));
        // set up board squares
        for (int r = 0; r < row; ++r)
        {
            theDisplay[c][r] = squareCharDisplay(c, r);
        }
    }
    // set up pieces
    for (int c = 0; c < col; ++c)
    {
        for (int r = 0; r < row; ++r)
        {
            // ! added check to ensure Piece methods aren't called on nullptr
            Piece *p = subject->getPieceByCoords(c, r);
            if (p == nullptr) {
                continue;
            }
            PieceType type = p->getType();
            Colour colour = p->getColour();
            
            switch (type)
            {
            case PieceType::King:
                theDisplay[c][r] = (colour == Colour::White) ? 'K' : 'k';
                break;
            case PieceType::Queen:
                theDisplay[c][r] = (colour == Colour::White) ? 'Q' : 'q';
                break;
            case PieceType::Bishop:
                theDisplay[c][r] = (colour == Colour::White) ? 'B' : 'b';
                break;
            case PieceType::Rook:
                theDisplay[c][r] = (colour == Colour::White) ? 'R' : 'r';
                break;
            case PieceType::Knight:
                theDisplay[c][r] = (colour == Colour::White) ? 'N' : 'n';
                break;
            case PieceType::Pawn:
                theDisplay[c][r] = (colour == Colour::White) ? 'P' : 'p';
                break;
            default:
                cout << "ERROR : Attempt to set up NULL_PIECE";
                break;
            }

        }
    }
}

// TODO vvv
void TextDisplay::doNotify(Move& move, GameState state) {
    cout << "-Incomplete method-" << endl;
    return;
}

TextDisplay::~TextDisplay() {}

// TODO ^^^

ostream &operator<<(ostream &out, const TextDisplay &td)
{
    char rowDigit = '8';

    for (int r = 0; r < row; ++r)
    {
        out << rowDigit-- << " ";
        for (const auto &col : td.theDisplay)
        { // for each column
            out << col[r];
        }
        out << endl;
    }
    out << endl;
    out << "  ";
    for (char c = 'a'; c <= 'h'; ++c)
    {
        out << c;
    }
    out << endl;
    return out;
}

