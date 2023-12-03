#include "graphicsdisplay.h"
#include "window.h"
#include "board.h"

using namespace std;

static const int NUM_COLS = 8;
static const int NUM_ROWS = 8;

const int WINDOW_SIZE = 800;
const int TILE_SIZE = 100;
const int TILE_PADDING = 10;

// squareColourDisplay(int, int) - Determines whether an empty square is black or white
bool squareColourDisplay(int c, int r)
{
    int sum = c + abs(r - 7);
    return (sum % 2 == 0);
}

GraphicsDisplay::GraphicsDisplay(): window{Xwindow{10, 10}} {}

GraphicsDisplay::GraphicsDisplay(Board *subject): window{Xwindow{WINDOW_SIZE, WINDOW_SIZE}} {
    window.fillRectangle(0, 0, WINDOW_SIZE, WINDOW_SIZE, Xwindow::White);
    doUpdate();
}

bool GraphicsDisplay::doNotify(Position pos, Piece *p) {
    return false;
}

void GraphicsDisplay::doUpdate() {
    window.fillRectangle(0, 0, WINDOW_SIZE, WINDOW_SIZE, Xwindow::White);

    // Iterate through columns
    for (int c = 0; c < NUM_COLS; ++c) {
        // Iterate through the row
        for (int r = 0; r < NUM_ROWS; ++r) {
            // Piece *p = subject->getPieceByCoords(c, r);
            // theDisplay[c][r] = p == nullptr ? squareCharDisplay(c, r) : getPieceChar(p);

            window.fillRectangle(c * TILE_SIZE, r * TILE_SIZE, TILE_SIZE, TILE_SIZE, squareColourDisplay(c, r) ? Xwindow::Black : Xwindow::White);
        }
    }
}

GraphicsDisplay::~GraphicsDisplay() {}

// #include "graphicsdisplay.h"

// // TODO - NONE OF THESE ARE IMPLEMENTED

// GraphicsDisplay::GraphicsDisplay() {}

// GraphicsDisplay::~GraphicsDisplay() {}

// void GraphicsDisplay::doNotify(Position pos, Piece *p) {
//     cout << "-Incomplete method-" << endl;
//     return;
// }
