#include "textdisplay.h"
#include <iostream>

using namespace std;

// abs(int) - Returns the absolute value of a number
int abs(int a)
{
    return (a >= 0) ? a : -a;
}

// squareCharDisplay(int, int) - Gets the character to print for an empty square
std::string squareCharDisplay(int c, int r)
{
    int sum = c + abs(r - 7);
    char ans = (sum % 2 == 0) ? '_' : ' ';
    std::string ans1 = "";
    return ans1 + ans;
}

// getPieceChar(Piece *) - Gets the character to print in order to represent a piece
std::string getPieceChar(Piece *p, const bool useUnicode) {
    if (useUnicode) {
        std::string ans = "";

        if (p->getColour() == Colour::White) {
            // Get the correct character for the type
            switch (p->getType())
            {
                case PieceType::King:
                    ans = "♔";
                    break;
                case PieceType::Queen:
                    ans = "♕";
                    break;
                case PieceType::Bishop:
                    ans = "♗";
                    break;
                case PieceType::Rook:
                    ans = "♖";
                    break;
                case PieceType::Knight:
                    ans = "♘";
                    break;
                default:
                    ans = "♙";
                    break;
            }
        }
        else {
            // Get the correct character for the type
            switch (p->getType())
            {
                case PieceType::King:
                    ans = "♚";
                    break;
                case PieceType::Queen:
                    ans = "♛";
                    break;
                case PieceType::Bishop:
                    ans = "♝";
                    break;
                case PieceType::Rook:
                    ans = "♜";
                    break;
                case PieceType::Knight:
                    ans = "♞";
                    break;
                default:
                    ans = "♟";
                    break;
            }
        }

        return ans;
    }
    else {
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
            default:
                ans = 'P';
                break;
        }

        // Convert to lowercase if the piece is Black
        ans = p->getColour() == Colour::White ? ans : tolower(ans);

        std::string ans1 = "";
        return ans1 + ans;
    }
}

// Constructor for TextDisplay
TextDisplay::TextDisplay(const Board *subject, const bool useUnicode): useUnicode{useUnicode}
{
    // Call doRefresh to populate theDisplay and print out the board
    doRefresh(subject);
}

// doNotify(pair, Piece *) - Overriden function to update and print the grid
bool TextDisplay::doNotify(Position pos, Piece *p)
{
    // Call getPieceChar() to get the appropriate character and update the grid
    //! [fixed] make sure you convert row for display purposes
    std::string tmp = (p == nullptr ? squareCharDisplay(pos.col, getRowForOutput(pos.row)) : getPieceChar(p, useUnicode));
    // If there is a change
    if (tmp != theDisplay[pos.col][getRowForOutput(pos.row)]) {
        // Update the grid
        theDisplay[pos.col][getRowForOutput(pos.row)] = tmp;
        return true;
    }
    return false;
}

void TextDisplay::doDisplay() {
    // Print out the updated TextDisplay
    cout << *this << endl;
}

// doRefresh - Force refresh everything
void TextDisplay::doRefresh(const Board *subject) {
    // Iterate through columns
    for (int c = 0; c < NUM_COLS; ++c) {
        // Initialize the column to a vector (the row)
        theDisplay.push_back(vector<std::string>(NUM_COLS, " "));
        // Iterate through the row
        for (int r = 0; r < NUM_ROWS; ++r) {
            // Call getPieceChar() to get the appropriate character and update the grid
            Piece *p = subject->getPieceByCoords(c, r);
            theDisplay[c][getRowForOutput(r)] = p == nullptr ? squareCharDisplay(c, getRowForOutput(r)) : getPieceChar(p, useUnicode);
        }
    }

    // Print out the game board
    doDisplay();
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
