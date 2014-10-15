//
//  Actions.cpp
//  simple_card
//
//  Created by sk on 10/15/14.
//
//

#include "Actions.h"

void Action::doAction(Canvas *c) {
    label = "base";
}

void Action::undoAction(Canvas *c) {
    cout << "base undo" << endl;
}

void AddLineAction::doAction(Canvas *c) {

    label = "add line";
    undo = false;
}

void AddLineAction::undoAction(Canvas *c) {

    label = "undo add line";
    undo = true;
}

void MoveSelectionAction::doAction(Canvas *c) {

    label = "move selection";
    undo = false;
}

void MoveSelectionAction::undoAction(Canvas *c) {

    label = "undo move";
    undo = true;
}
