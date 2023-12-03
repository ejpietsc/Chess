#ifndef OBSERVER_H
#define OBSERVER_H

#include "util.h"
#include "pieces.h"

enum class GameState;

class Observer {
    virtual bool doNotify(Position pos, Piece *p) = 0;
    virtual void doUpdate() = 0;
    public:
        // NVI - call doNotify
        void notify(Position pos, Piece *p);
        void notify(std::vector<std::pair<Position, Piece *>> vec);
        void update();
        virtual ~Observer() = default;
};

#endif

