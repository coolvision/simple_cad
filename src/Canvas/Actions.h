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

class SelectionList {
public:
    vector<Vertex *> vertices;
    vector<ofPoint> start_p;
    void add(Vertex *v);
    void clear();
};

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

// input: 2 points
// result: new polyline inserted at the back of the vector
    ofPoint p[0];
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
};

class ChangeSelectionAction: public Action {
public:
    virtual void doAction(Canvas *c);
    virtual void undoAction(Canvas *c);
};

class AddVertexAction: public Action {
public:
    virtual void doAction(Canvas *c);
    virtual void undoAction(Canvas *c);
};

class DeleteVertexAction: public Action {
public:
    virtual void doAction(Canvas *c);
    virtual void undoAction(Canvas *c);
};
