#include "graphicsdisplay.h"
#include "window.h"
#include "board.h"
#include "util.h"

using namespace std;

// Constants for displaying
const int WINDOW_SIZE = 800;
const int TILE_SIZE = 100;
const int TILE_PADDING = 10;
const int NUM_PIXELS = 10;
const int PIXEL_SIZE = ((TILE_SIZE - (2 * TILE_PADDING)) / NUM_PIXELS);

// Shapes of all the pieces
// '.' indicates nothing (don't fill)
// '#' for filling in piece colour
// '+' for filling in gold
std::map<PieceType, std::vector<std::string>> PIECE_SHAPES{
    {
        PieceType::King,
        {
            "...++++...",
            "....++....",
            ".########.",
            "..######..",
            "...####...",
            "....##....",
            "....##....",
            "..######..",
            "..######..",
            ".########."
        }
    },
    {
        PieceType::Queen,
        {
            "....##....",
            ".########.",
            "..##++##..",
            "...####...",
            "....##....",
            "...####...",
            "....##....",
            "..######..",
            "..######..",
            ".########."
        }
    },
    {
        PieceType::Bishop,
        {
            "...##.#...",
            "..####.#..",
            ".######.#.",
            "..######..",
            "...####...",
            "....##....",
            "....##....",
            "..######..",
            "..######..",
            ".########."

        }
    },
    {
        PieceType::Rook,
        {
            "..........",
            "..#.##.#..",
            "..######..",
            "..######..",
            "....##....",
            "....##....",
            "....##....",
            "..######..",
            "..######..",
            ".########."
        }
    },
    {
        PieceType::Knight,
        {
            "..######..",
            ".########.",
            ".####..##.",
            "..###.....",
            "...###....",
            "....##....",
            "....##....",
            "..######..",
            "..######..",
            ".########."

        }
    },
    {
        PieceType::Pawn,
        {
            "..........",
            "...####...",
            "..######..",
            "...####...",
            "....##....",
            "...####...",
            "....##....",
            "..######..",
            "..######..",
            ".########."

        }
    },
};

// squareColourDisplay(int, int) - Determines whether an empty square is black or white
bool squareColourDisplay(int c, int r)
{
    int sum = c + abs(r - 7);
    return (sum % 2 == 0);
    // True: Black, False: White
}

// displayTile(int, int) - Displays the tile at location (c, r)
// This will overwrite row/column numbers and pieces
void GraphicsDisplay::displayTile(int c, int r) {
    window.fillRectangle(
        c * TILE_SIZE,
        r * TILE_SIZE,
        TILE_SIZE,
        TILE_SIZE,
        squareColourDisplay(c, r) ? Xwindow::DarkBrown : Xwindow::LightBrown
        // Custom colours - inspired by Macbook chess app
    );
}

// displayPiece(int, int, Piece *) - Displays the piece p at location (c, r)
void GraphicsDisplay::displayPiece(int c, int r, Piece *p) {
    // Get the shape of the piece
    std::vector<std::string> shape = PIECE_SHAPES[p->getType()];
    // Iterate through the shape vector
    for (int i = 0; i < shape.size(); ++i) {
        // Iterate through each string
        for (int j = 0; j < shape[i].length(); ++j) {
            // If the current pixel needs to be displayed
            if (shape[i][j] == '#' || shape[i][j] == '+') {
                // Display the pixel
                window.fillRectangle(
                    ((c * TILE_SIZE) + TILE_PADDING + (j * PIXEL_SIZE)),
                    ((r * TILE_SIZE) + TILE_PADDING + (i * PIXEL_SIZE)),
                    PIXEL_SIZE,
                    PIXEL_SIZE,
                    shape[i][j] == '+' ?
                        Xwindow::Gold :
                        (p->getColour() == Colour::White ? Xwindow::White : Xwindow::Black)
                );
            }
        }
    }
}

// displayGridLoc(int, int) - Displays the row number and / or column letter
// if the tile in (c, r) is an edge tile
void GraphicsDisplay::displayGridLoc(int c, int r) {
    // Top row - print column letters
    if (r == 0) {
        std::string val;
        val += static_cast<char>('A' + c);
        window.drawString(
            ((c * TILE_SIZE) + 5),
            12,
            val,
            Xwindow::Black
        );
    }

    // Left column - print row numbers
    if (c == 0) {
        std::string val = std::to_string(getRowForOutput(r) + 1);
        window.drawString(
            5,
            ((r * TILE_SIZE) + 50),
            val,
            Xwindow::Black
        );
    }

    // Bottom row - print column letters
    if (r == NUM_ROWS - 1) {
        std::string val;
        val += static_cast<char>('A' + c);
        window.drawString(
            ((c * TILE_SIZE) + 5),
            (((r + 1) * TILE_SIZE) - 5),
            val,
            Xwindow::Black
        );
    }

    // Right column - print row numbers
    if (c == NUM_COLS - 1) {
        std::string val = std::to_string(getRowForOutput(r) + 1);
        window.drawString(
            (((c + 1) * TILE_SIZE) - 10),
            ((r * TILE_SIZE) + 50),
            val,
            Xwindow::Black
        );
    }
}

// Default ctor (for board copy/move ctor) - Makes a tiny display that the end user hopefully doesn't notice
// Should only be called if the display created by it is going to be destructed soon (ex. std::swap)
GraphicsDisplay::GraphicsDisplay(): window{Xwindow{10, 10}} {}

// Regular ctor - Always call this if you intend to actually use the display that you create
GraphicsDisplay::GraphicsDisplay(const Board *subject): window{Xwindow{WINDOW_SIZE, WINDOW_SIZE}} {
    // Populate the window
    doRefresh(subject);
}

bool GraphicsDisplay::doNotify(Position pos, Piece *p) {
    displayTile(pos.col, getRowForOutput(pos.row));
    if (p) displayPiece(pos.col, getRowForOutput(pos.row), p);
    displayGridLoc(pos.col, getRowForOutput(pos.row));

    return false; // We never need to call update() for the GraphicsDisplay
}

// This function is only intended for text displays
// We don't need to re-print a GraphicsDisplay, so this function does nothing
void GraphicsDisplay::doDisplay() {}

// doRefresh - Force refresh everything
void GraphicsDisplay::doRefresh(const Board *subject) {
    window.fillRectangle(0, 0, WINDOW_SIZE, WINDOW_SIZE, Xwindow::Black);

    // For some reason, the only way I'm able to get the tiles to display correctly is by printing out the grid twice
    // I have no idea why this is happening, but this janky fix will do.

    // Iterate through columns
    for (int c = 0; c < NUM_COLS; ++c) {
        // Iterate through the row
        for (int r = 0; r < NUM_ROWS; ++r) {
            displayTile(c, getRowForOutput(r));
        }
    }

    // Iterate through columns
    for (int c = 0; c < NUM_COLS; ++c) {
        // Iterate through the row
        for (int r = 0; r < NUM_ROWS; ++r) {
            displayTile(c, getRowForOutput(r));

            Piece *p = subject->getPieceByCoords(c, r);
            if (p) displayPiece(c, getRowForOutput(r), p);
            displayGridLoc(c, getRowForOutput(r));
        }
    }
}

GraphicsDisplay::~GraphicsDisplay() {}
