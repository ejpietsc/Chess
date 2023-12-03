#include "graphicsdisplay.h"
#include "window.h"
#include "board.h"

using namespace std;

static const int NUM_COLS = 8;
static const int NUM_ROWS = 8;

const int WINDOW_SIZE = 800;
const int TILE_SIZE = 100;
const int TILE_PADDING = 10;
const int NUM_PIXELS = 10;
const int PIXEL_SIZE = ((TILE_SIZE - (2 * TILE_PADDING)) / NUM_PIXELS);

std::map<PieceType, std::vector<std::string>> PIECE_SHAPES{
    {
        PieceType::King,
        {
            "...####...",
            "....##....",
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
            "..##..##..",
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
    {
        PieceType::NULL_PIECE,
        {
            "..........",
            "..........",
            "..........",
            "..........",
            "..........",
            "..........",
            "..........",
            "..........",
            "..........",
            ".........."
        }
    }
};


// squareColourDisplay(int, int) - Determines whether an empty square is black or white
bool squareColourDisplay(int c, int r)
{
    int sum = c + abs(r - 7);
    return (sum % 2 == 0);
}

GraphicsDisplay::GraphicsDisplay(): window{Xwindow{10, 10}} {}

GraphicsDisplay::GraphicsDisplay(Board *subject): window{Xwindow{WINDOW_SIZE, WINDOW_SIZE}} {
    window.fillRectangle(0, 0, WINDOW_SIZE, WINDOW_SIZE, Xwindow::Black);

    // For some reason, the only way I'm able to get the tiles to display correctly is by printing out the grid twice
    // I have no idea why this is happening HELP ME I'M GOING INSANE!

    // Iterate through columns
    for (int c = 0; c < NUM_COLS; ++c) {
        // Iterate through the row
        for (int r = 0; r < NUM_ROWS; ++r) {
            window.fillRectangle(
                c * TILE_SIZE,
                r * TILE_SIZE,
                TILE_SIZE,
                TILE_SIZE,
                squareColourDisplay(c, r) ? Xwindow::Blue : Xwindow::Green
            );
        }
    }

    // Iterate through columns
    for (int c = 0; c < NUM_COLS; ++c) {
        // Iterate through the row
        for (int r = 0; r < NUM_ROWS; ++r) {
            window.fillRectangle(c * TILE_SIZE, r * TILE_SIZE, TILE_SIZE, TILE_SIZE, squareColourDisplay(c, r) ? Xwindow::Blue : Xwindow::Green);

            Piece *p = subject->getPieceByCoords(c, r);
            if (p) {
                std::vector<std::string> shape = PIECE_SHAPES[p->getType()];
                for (int i = 0; i < shape.size(); ++i) {
                    for (int j = 0; j < shape[i].length(); ++j) {
                        if (shape[i][j] == '#') {
                            window.fillRectangle(
                                ((c * TILE_SIZE) + TILE_PADDING + (j * PIXEL_SIZE)),
                                ((r * TILE_SIZE) + TILE_PADDING + (i * PIXEL_SIZE)),
                                PIXEL_SIZE,
                                PIXEL_SIZE,
                                p->getColour() == Colour::White ? Xwindow::White : Xwindow::Black
                            );
                        }
                    }
                }
            }
        }
    }
}

bool GraphicsDisplay::doNotify(Position pos, Piece *p) {
    return false;
}

void GraphicsDisplay::doUpdate() {}

GraphicsDisplay::~GraphicsDisplay() {}

// #include "graphicsdisplay.h"

// // TODO - NONE OF THESE ARE IMPLEMENTED

// GraphicsDisplay::GraphicsDisplay() {}

// GraphicsDisplay::~GraphicsDisplay() {}

// void GraphicsDisplay::doNotify(Position pos, Piece *p) {
//     cout << "-Incomplete method-" << endl;
//     return;
// }
