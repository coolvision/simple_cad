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

    label = "move " + ofToString(v.x) + " " + ofToString(v.y);

    if (undo) {
        for (int i = 0; i < selection.vertices.size(); i++) {
            Vertex *vertex = c->getVertex(selection.vertices[i]);
            *vertex = *vertex + v;
            vertex->p->updatePath();
        }
    }
    undo = false;
}

void MoveSelectionAction::undoAction(Canvas *c) {

    label = "undo move " + ofToString(v.x) + " " + ofToString(v.y);

    if (!undo) {
        for (int i = 0; i < selection.vertices.size(); i++) {
            Vertex *vertex = c->getVertex(selection.vertices[i]);
            *vertex = *vertex - v;
            vertex->p->updatePath();
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
        for (int i = 0; i < prev_selection.vertices.size(); i++) {
            Vertex *vertex = c->getVertex(prev_selection.vertices[i]);
            vertex->selected = false;
        }
        for (int i = 0; i < new_selection.vertices.size(); i++) {
            Vertex *vertex = c->getVertex(new_selection.vertices[i]);
            vertex->selected = true;
        }
        c->selection = new_selection;
    }

    undo = false;
}

void ChangeSelectionAction::undoAction(Canvas *c) {

    label = "undo select";

    if (!undo) {
        for (int i = 0; i < new_selection.vertices.size(); i++) {
            Vertex *vertex = c->getVertex(new_selection.vertices[i]);
            vertex->selected = false;
        }
        for (int i = 0; i < prev_selection.vertices.size(); i++) {
            Vertex *vertex = c->getVertex(prev_selection.vertices[i]);
            vertex->selected = true;
        }
        c->selection = prev_selection;
    }
    
    undo = true;
}

//==============================================================================
ConnectPolylinesAction::ConnectPolylinesAction() {
    label = "ConnectPolylinesAction";
    undo = true;
}

void ConnectPolylinesAction::doAction(Canvas *c) {

    label = "connect " + ofToString(p1.i) + " " + ofToString(p2.i);

    if (undo) {

        // get the current polylines
        Polyline *p = c->lines[p1.i];
        Polyline *l = c->lines[p2.i];

        if (reverse) {
            l->reverse();
        }
        for (Vertex *v = l->back->prev; v != NULL; v = v->prev) {
            if (add_back) {
                p->addBack(ofPoint(*v));
            } else {
                p->addFront(ofPoint(*v));
            }
        }

        c->lines[p2.i]->release();
    }

    undo = false;
}

void ConnectPolylinesAction::undoAction(Canvas *c) {

    label = "undo connect " + ofToString(p1.i) + " " + ofToString(p2.i);

    if (!undo) {
        c->lines[p1.i]->release();
        c->lines[p2.i]->release();

        c->lines[p1.i]->cloneFrom(&p1);
        c->lines[p2.i]->cloneFrom(&p2);
    }

    undo = true;
}

//==============================================================================
ClosePolylineAction::ClosePolylineAction() {
    label = "ClosePolylineAction";
    undo = true;
}

void ClosePolylineAction::doAction(Canvas *c) {

    label = "close";

    if (undo) {
        c->lines[p_open.i]->release();
        c->lines[p_open.i]->cloneFrom(&p_closed);
    }

    undo = false;
}

void ClosePolylineAction::undoAction(Canvas *c) {

    label = "undo close";

    if (!undo) {
        c->lines[p_open.i]->release();
        c->lines[p_open.i]->cloneFrom(&p_open);
    }

    undo = true;
}

//==============================================================================
AddVertexAction::AddVertexAction() {
    label = "AddVertexAction";
    undo = true;
}

void AddVertexAction::doAction(Canvas *c) {

    label = "add vertex";

    if (undo) {

    }

    undo = false;
}

void AddVertexAction::undoAction(Canvas *c) {

    label = "undo add vertex";

    if (!undo) {
        
    }
    
    undo = true;
}

//==============================================================================
DeleteVertexAction::DeleteVertexAction() {
    label = "DeleteVertexAction";
    undo = true;
}

void DeleteVertexAction::doAction(Canvas *c) {

    label = "delete vertex";

    if (undo) {

    }

    undo = false;
}

void DeleteVertexAction::undoAction(Canvas *c) {

    label = "undo delete vertex";

    if (!undo) {

    }
    
    undo = true;
}
