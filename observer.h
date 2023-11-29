#ifndef OBSERVER_H
#define OBSERVER_H

#include "util.h"
#include "pieces.h"

enum class GameState;

class Observer {
    virtual void doNotify(pair<int, int> pos, Piece *p) = 0;
    public:
        // NVI - call doNotify
        void notify(pair<int, int> pos, Piece *p);
        virtual ~Observer() = default;
};

#endif

