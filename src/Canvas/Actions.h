//
//  Actions.h
//  simple_card
//
//  Created by sk on 10/15/14.
//
//

#pragma once

#include "Canvas.h"

class Action {
public:
    virtual void doAction(Canvas *c);
    virtual void undoAction(Canvas *c);
};

class AddLineAction: public Action {
public:
    void doAction(Canvas *c);
    void undoAction(Canvas *c);

};

class MoveSelectionAction: public Action {
public:
    void doAction(Canvas *c);
    void undoAction(Canvas *c);


};
