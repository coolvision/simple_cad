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
    virtual void doAction(Canvas *c);
    virtual void undoAction(Canvas *c);

// input: list of vertices (line indexes, vertex indexes), move vector
// result: new positions for the vertices
    SelectionList selection;
    ofPoint v;
};

class ConnectPolylinesAction: public Action {
public:
    virtual void doAction(Canvas *c);
    virtual void undoAction(Canvas *c);

// input: 2 polylines: indexes, objects
// after: 1 polyline modified (index, object), one deleted (index)

};

class ChangeSelectionAction: public Action {
public:
    virtual void doAction(Canvas *c);
    virtual void undoAction(Canvas *c);

// input: list of selected vertices (line indexes, vertex indexes)
// after: list of selected vertices (line indexes, vertex indexes)
};

class AddVertexAction: public Action {
public:
    virtual void doAction(Canvas *c);
    virtual void undoAction(Canvas *c);

// input: a point
// after: 1 polyline modified (index, object)
};

class DeleteVertexAction: public Action {
public:
    virtual void doAction(Canvas *c);
    virtual void undoAction(Canvas *c);

// input: a polyline (index, object)
// after: a polyline (index, object)
};
