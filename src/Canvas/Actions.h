//
//  Actions.h
//  simple_card
//
//  Created by sk on 10/15/14.
//
//

#pragma once

#include "ofMain.h"

class Canvas;

class Action {
public:
    virtual void doAction(Canvas *c);
    virtual void undoAction(Canvas *c);

    string label;
    bool undo;
};

class AddLineAction: public Action {
public:
    virtual void doAction(Canvas *c);
    virtual void undoAction(Canvas *c);
};

class MoveSelectionAction: public Action {
public:
    virtual void doAction(Canvas *c);
    virtual void undoAction(Canvas *c);
};
