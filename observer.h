#ifndef OBSERVER_H
#define OBSERVER_H

#include "util.h"
#include "pieces.h"

enum class GameState;

class Board; // Forward declaration

class Observer {
    virtual bool doNotify(Position pos, Piece *p) = 0;
    virtual void doDisplay() = 0;
    virtual void doRefresh(const Board *subject) = 0;

    public:
        // Notify the Observer of an update to one of the tiles
        // NVI - call doNotify
        void notify(Position pos, Piece *p);
        void notify(std::vector<std::pair<Position, Piece *>> vec);

        // Tell the display to display (re-print) itself (only for text-based observers)
        void display();

        // Make the display force refresh everything (go over all tiles)
        // Added to make it easier to debug the graphicsdisplay
        void refresh(const Board *subject);

        virtual ~Observer() = default;
};

#endif

