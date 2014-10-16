//
//  Actions.h
//  simple_card
//
//  Created by sk on 10/15/14.
//
//

#pragma once

#include "ofMain.h"
#include "Polyline.h"

class Canvas;

class Action {
public:
    Action();
    virtual void doAction(Canvas *c);
    virtual void undoAction(Canvas *c);
    string label;
    bool undo;
};

class AddLineAction: public Action {
public:
    AddLineAction();
    virtual void doAction(Canvas *c);
    virtual void undoAction(Canvas *c);

// input: 2 points
// result: new polyline inserted at the back of the vector
    ofPoint p[2];
};

class MoveSelectionAction: public Action {
public:
    MoveSelectionAction();
    virtual void doAction(Canvas *c);
    virtual void undoAction(Canvas *c);

// input: list of vertices (line indexes, vertex indexes), move vector
// result: new positions for the vertices
    SelectionList selection;
    ofPoint v;
};

class ChangeSelectionAction: public Action {
public:
    ChangeSelectionAction();
    virtual void doAction(Canvas *c);
    virtual void undoAction(Canvas *c);

// input: list of selected vertices (line indexes, vertex indexes)
// after: list of selected vertices (line indexes, vertex indexes)
    SelectionList prev_selection;
    SelectionList new_selection;
};

class ConnectPolylinesAction: public Action {
public:
    ConnectPolylinesAction();
    virtual void doAction(Canvas *c);
    virtual void undoAction(Canvas *c);

// input: 2 polylines: indexes, objects
// after: 1 polyline modified (index, object), one deleted (index)
    Polyline p1;
    Polyline p2;
    bool reverse;
    bool add_back;
    Polyline p_linked;
};

class ClosePolylineAction: public Action {
public:
    ClosePolylineAction();
    virtual void doAction(Canvas *c);
    virtual void undoAction(Canvas *c);

    Polyline p_open;
    Polyline p_closed;
};

class AddVertexAction: public Action {
public:
    AddVertexAction();
    virtual void doAction(Canvas *c);
    virtual void undoAction(Canvas *c);

// input: a point
// after: 1 polyline modified (index, object)
};

class DeleteVertexAction: public Action {
public:
    DeleteVertexAction();
    virtual void doAction(Canvas *c);
    virtual void undoAction(Canvas *c);

// input: a polyline (index, object)
// after: a polyline (index, object)
};
