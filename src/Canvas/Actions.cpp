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
AddJointAction::AddJointAction() {
    label = "AddJointAction";
    undo = true;
}

void AddJointAction::doAction(Canvas *c) {

    label = "add joint";

    if (undo) {
        Joint *j = new Joint;
        j->joint_type = type;
        j->p = p;
        j->id = c->joints.size();
        j->setupGui();
        c->joints.push_back(j);
    }
    undo = false;
}

void AddJointAction::undoAction(Canvas *c) {

    label = "undo add joint";

    if (!undo) {
        delete c->joints.back();
        c->joints.pop_back();
    }
    undo = true;
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
        c->lines.back()->id = c->lines.size() - 1;
        c->lines.back()->addBack(&p[0]);
        c->lines.back()->addBack(&p[1]);
        c->lines.back()->update();
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
        for (int i = 0; i < selection.items.size(); i++) {
            InteractiveObject *item = c->getItem(selection.items[i]);
            if (item == NULL) continue;
            //item->p += v;
            item->update();
        }
    }
    undo = false;
}

void MoveSelectionAction::undoAction(Canvas *c) {

    label = "undo move " + ofToString(v.x) + " " + ofToString(v.y);

    if (!undo) {
        for (int i = 0; i < selection.items.size(); i++) {
            InteractiveObject *item = c->getItem(selection.items[i]);
            if (item == NULL) continue;
            item->p -= v;
            item->update();
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
        for (int i = 0; i < prev_selection.items.size(); i++) {
            InteractiveObject *item = c->getItem(prev_selection.items[i]);
            if (item != NULL) {
                item->selected = false;
            }
        }
        for (int i = 0; i < new_selection.items.size(); i++) {
            InteractiveObject *item = c->getItem(new_selection.items[i]);
            if (item != NULL) {
                item->selected = true;
            }
        }
        c->selection = new_selection;
    }

    undo = false;
}

void ChangeSelectionAction::undoAction(Canvas *c) {

    label = "undo select";

    if (!undo) {
        for (int i = 0; i < new_selection.items.size(); i++) {
            InteractiveObject *vertex = c->getItem(new_selection.items[i]);
            vertex->selected = false;
        }
        for (int i = 0; i < prev_selection.items.size(); i++) {
            InteractiveObject *vertex = c->getItem(prev_selection.items[i]);
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

    label = "connect " + ofToString(p1.id) + " " + ofToString(p2.id);

    if (undo) {

        // get the current polylines
        Polyline *p = c->lines[p1.id];
        Polyline *l = c->lines[p2.id];

        if (reverse) {
            l->reverse();
        }
        if (add_back) {
            for (Vertex *v = l->front->next; v != NULL; v = v->next) {
                p->addBack(v);
            }
        } else {
            for (Vertex *v = l->back->prev; v != NULL; v = v->prev) {
                p->addFront(v);
            }
        }

        l->release();

        p->update();
    }

    undo = false;
}

void ConnectPolylinesAction::undoAction(Canvas *c) {

    label = "undo connect " + ofToString(p1.id) + " " + ofToString(p2.id);

    if (!undo) {
        c->lines[p1.id]->release();
        c->lines[p2.id]->release();

        c->lines[p1.id]->cloneFrom(&p1);
        c->lines[p2.id]->cloneFrom(&p2);
    }

    undo = true;
}

//==============================================================================
ModifyPolylineAction::ModifyPolylineAction() {
    label = "ModifyPolylineAction";
    undo = true;
}

void ModifyPolylineAction::doAction(Canvas *c) {

    label = "modify " + ofToString(p_after.id);

    if (undo) {
        c->lines[p_after.id]->release();
        c->lines[p_after.id]->cloneFrom(&p_after);
    }

    undo = false;
}

void ModifyPolylineAction::undoAction(Canvas *c) {

    label = "undo modify " + ofToString(p_after.id);

    if (!undo) {
        c->lines[p_after.id]->release();
        c->lines[p_after.id]->cloneFrom(&p_before);
    }

    undo = true;
}


//==============================================================================
ModifyJointsAction::ModifyJointsAction() {
    label = "ModifyJointsAction";
    undo = true;
}

ModifyJointsAction::~ModifyJointsAction() {
    while (!before.empty()) {
        delete before.back();
        before.pop_back();
    }
    while (!after.empty()) {
        delete after.back();
        after.pop_back();
    }
}

void ModifyJointsAction::doAction(Canvas *c) {

    label = "modify joints";

    if (undo) {

        while (!c->joints.empty()) {
            delete c->joints.back();
            c->joints.pop_back();
        }

        for (int i = 0; i < after.size(); i++) {
            if (after[i] != NULL) {
                c->joints.push_back((Joint *)after[i]->getCopy());
            } else {
                c->joints.push_back(NULL);
            }
        }
    }

    undo = false;
}

void ModifyJointsAction::undoAction(Canvas *c) {

    label = "undo modify";

    if (!undo) {

        while (!c->joints.empty()) {
            delete c->joints.back();
            c->joints.pop_back();
        }

        for (int i = 0; i < before.size(); i++) {
            if (before[i] != NULL) {
                c->joints.push_back((Joint *)before[i]->getCopy());
            } else {
                c->joints.push_back(NULL);
            }
        }
    }
    
    undo = true;
}




