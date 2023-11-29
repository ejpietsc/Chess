#include "textdisplay.h"
#include <iostream>

using namespace std;

// Constants to store the size of the grid
static const int NUM_COLS = 8;
static const int NUM_ROWS = 8;

// abs(int) - Returns the absolute value of a number
int abs(int a)
{
    return (a > 0) ? a : -a;
}

// squareCharDisplay(int, int) - Gets the character to print for an empty square
char squareCharDisplay(int c, int r)
{
    int sum = c + abs(r - 7);
    return (sum % 2 == 0) ? '_' : ' ';
}

// getPieceChar(Piece *) - Gets the character to print in order to represent a piece
char getPieceChar(Piece *p) {
    char ans = ' ';

    // Get the correct character for the type
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

    // Convert to lowercase if the piece is Black
    ans = p->getColour() == Colour::White ? ans : tolower(ans);

    return ans;
}

// Constructor for TextDisplay
TextDisplay::TextDisplay(Board *subject)
{
    // Iterate through columns
    for (int c = 0; c < NUM_COLS; ++c) {
        // Initialize the column to a vector (the row)
        theDisplay.push_back(vector<char>(NUM_COLS, ' '));
        // Iterate through the row
        for (int r = 0; r < NUM_ROWS; ++r) {
            // Call getPieceChar() to get the appropriate character and update the grid
            Piece *p = subject->getPieceByCoords(c, r);
            theDisplay[c][r] = p == nullptr ? squareCharDisplay(c, r) : getPieceChar(p);
        }
    }

    // Print out the updated TextDisplay
    cout << *this << endl;
}

// doNotify(pair, Piece *) - Overriden function to update and print the grid
void TextDisplay::doNotify(pair<int, int> pos, Piece *p)
{
    // Call getPieceChar() to get the appropriate character and update the grid
    theDisplay[pos.first][pos.second] = p == nullptr ? squareCharDisplay(pos.first, pos.second) : getPieceChar(p);

    // Print out the updated TextDisplay
    cout << *this << endl;
}

// Destructor for TextDisplay - Doesn't need to do anything
TextDisplay::~TextDisplay() {}

// Output operator for TextDisplay
ostream &operator<<(ostream &out, const TextDisplay &td)
{
    // Iterate through columns
    char rowDigit = '8';
    for (int r = 0; r < NUM_ROWS; ++r)
    {
        // Print out row
        out << rowDigit-- << " ";
        for (const auto &col : td.theDisplay) out << col[r];
        out << endl;
    }
    // Print out characters to represent columns
    out << endl << "  ";
    for (char c = 'a'; c <= 'h'; ++c) out << c;
    out << endl;

    return out; // For chaining
}
