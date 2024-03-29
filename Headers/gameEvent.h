#pragma once
#include "init.h"

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

class GameEvent {
public:
    bool (*eventCondition) ();
    void (*function)(void);

    GameEvent() {}
    virtual void setFunction(void (*func)(void)) { function = func; }
    //virtual void setCondition(bool eventCond) { eventCondition = eventCond; } TO BE FIXED
    virtual bool isActivated(Rect, sf::Event&);
};

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////