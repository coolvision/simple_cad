//
//  Actions.cpp
//  simple_card
//
//  Created by sk on 10/15/14.
//
//

#include "Actions.h"
#include "Canvas.h"

Action::Action() {
    label = "Action";
    undo = false;
}

void Action::doAction(Canvas *c) {
    label = "base";
}

void Action::undoAction(Canvas *c) {
    cout << "base undo" << endl;
}

AddLineAction::AddLineAction() {
    label = "AddLineAction";
    undo = true;
}

void AddLineAction::doAction(Canvas *c) {

    label = "add line";

    if (undo) {
        c->lines.push_back(new Polyline());
        c->lines.back()->i = c->lines.size() - 1;
        c->lines.back()->addBack(p[0]);
        c->lines.back()->addBack(p[1]);
    }

    undo = false;
}

void AddLineAction::undoAction(Canvas *c) {

    label = "undo add line";
    undo = true;

    if (!c->lines.empty()) {
        delete c->lines.back();
        c->lines.pop_back();
    }
}

void MoveSelectionAction::doAction(Canvas *c) {

    label = "move selection";
    undo = false;
}

void MoveSelectionAction::undoAction(Canvas *c) {

    label = "undo move";
    undo = true;
}
