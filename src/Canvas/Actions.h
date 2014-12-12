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
#include "Joint.h"

class Canvas;

class Action {
public:
    Action();
    virtual ~Action() {};
    virtual void doAction(Canvas *c);
    virtual void undoAction(Canvas *c);
    string label;
    bool undo;
};

class AddLineAction: public Action {
public:
    AddLineAction();
    virtual ~AddLineAction() {};
    virtual void doAction(Canvas *c);
    virtual void undoAction(Canvas *c);

// input: 2 points
// result: new polyline inserted at the back of the vector
    Vertex p[2];
};

class AddJointAction: public Action {
public:
    AddJointAction();
    virtual ~AddJointAction() {};
    virtual void doAction(Canvas *c);
    virtual void undoAction(Canvas *c);

// input: joint position and type
// result: new joint added
    ofPoint p;
    JointType type;
};

class MoveSelectionAction: public Action {
public:
    MoveSelectionAction();
    virtual ~MoveSelectionAction() {};
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
    virtual ~ChangeSelectionAction() {};
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
    virtual ~ConnectPolylinesAction() {};
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

class ModifyPolylineAction: public Action {
public:
    ModifyPolylineAction();
    virtual ~ModifyPolylineAction() {};
    virtual void doAction(Canvas *c);
    virtual void undoAction(Canvas *c);

    Polyline p_before;
    Polyline p_after;
};

class ModifyJointsAction: public Action {
public:
    ModifyJointsAction();
    virtual ~ModifyJointsAction();
    virtual void doAction(Canvas *c);
    virtual void undoAction(Canvas *c);

    vector<Joint *> before;
    vector<Joint *> after;
};
