#include "graphicsdisplay.h"
#include "window.h"
#include "board.h"
#include "util.h"

using namespace std;

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
            "##......##",
            ".##....##.",
            "..##..##..",
            "...####...",
            "....##....",
            "...####...",
            "..##..##..",
            ".##....##.",
            "##......##"
        }
    }
};

// squareColourDisplay(int, int) - Determines whether an empty square is black or white
bool squareColourDisplay(int c, int r)
{
    int sum = c + abs(r - 7);
    return (sum % 2 == 0);
}

void GraphicsDisplay::displayTile(int c, int r) {
    window.fillRectangle(
        c * TILE_SIZE,
        r * TILE_SIZE,
        TILE_SIZE,
        TILE_SIZE,
        squareColourDisplay(c, r) ? Xwindow::Blue : Xwindow::Green
    );
}

void GraphicsDisplay::displayPiece(int c, int r, Piece *p) {
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

GraphicsDisplay::GraphicsDisplay(): window{Xwindow{10, 10}} {}

GraphicsDisplay::GraphicsDisplay(Board *subject): window{Xwindow{WINDOW_SIZE, WINDOW_SIZE}} {
    window.fillRectangle(0, 0, WINDOW_SIZE, WINDOW_SIZE, Xwindow::Black);

    // For some reason, the only way I'm able to get the tiles to display correctly is by printing out the grid twice
    // I have no idea why this is happening HELP ME I'M GOING INSANE!

    // Iterate through columns
    for (int c = 0; c < NUM_COLS; ++c) {
        // Iterate through the row
        for (int r = 0; r < NUM_ROWS; ++r) {
            displayTile(c, getCorrectRow(r));
        }
    }

    // Iterate through columns
    for (int c = 0; c < NUM_COLS; ++c) {
        // Iterate through the row
        for (int r = 0; r < NUM_ROWS; ++r) {
            displayTile(c, getCorrectRow(r));

            Piece *p = subject->getPieceByCoords(c, getCorrectRow(r));
            if (p) displayPiece(c, getCorrectRow(r), p);
        }
    }
}

bool GraphicsDisplay::doNotify(Position pos, Piece *p) {
    displayTile(pos.col, getCorrectRow(pos.row));
    if (p) displayPiece(pos.col, getCorrectRow(pos.row), p);

    return true;
}

void GraphicsDisplay::doUpdate() {}

GraphicsDisplay::~GraphicsDisplay() {}
