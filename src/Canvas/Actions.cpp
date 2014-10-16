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

//==============================================================================
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

    if (!undo) {
        if (!c->lines.empty()) {
            delete c->lines.back();
            c->lines.pop_back();
        }
    }
    undo = true;
}

//==============================================================================
MoveSelectionAction::MoveSelectionAction() {
    label = "MoveSelectionAction";
    undo = true;
}

void MoveSelectionAction::doAction(Canvas *c) {

    label = "move";

    if (undo) {
        for (int i = 0; i < selection.vertices.size(); i++) {
            Vertex *vertex = c->getVertex(selection.vertices[i]);
            vertex->p->updatePath();
            *vertex = selection.start_p[i] + v;
        }
    }
    undo = false;
}

void MoveSelectionAction::undoAction(Canvas *c) {

    label = "undo move";

    if (!undo) {
        for (int i = 0; i < selection.vertices.size(); i++) {
            Vertex *vertex = c->getVertex(selection.vertices[i]);
            vertex->p->updatePath();
            *vertex = selection.start_p[i];
        }
    }
    undo = true;
}

//==============================================================================
ChangeSelectionAction::ChangeSelectionAction() {
    label = "ChangeSelectionAction";
    undo = true;
}

void ChangeSelectionAction::doAction(Canvas *c) {

    label = "select";

    if (undo) {

    }

    undo = false;
}

void ChangeSelectionAction::undoAction(Canvas *c) {

    label = "undo select";

    if (!undo) {

    }
    
    undo = true;
}



