//
//  ConnectLines.cpp
//  simple_card
//
//  Created by sk on 10/2/14.
//
//

#include "ofApp.h"

bool close(ofPoint p1, ofPoint p2) {
    if ((p1 - p2).length() < 2.0f) {
        return true;
    }
    return false;
}

// check if endpoints overlap, and if they do, connect the polylines
void Canvas::connectPolylines(InteractiveContainer *p) {

    if (p == NULL) {
        return;
    }

    if (p->front == NULL || p->closed) {
        return;
    }

    if (close(p->front->p, p->back->p) && p->getLength() > 2) {

        ModifyPolylineAction *close = new ModifyPolylineAction();
        close->p_before.cloneFrom(p);

        InteractiveObject *tmp = p->back;
        p->back = p->back->prev;
        delete tmp;
        p->back->next = p->front;
        p->front->prev = p->back;
        p->closed = true;

        close->p_after.cloneFrom(p);
        addAction(close);

        return;
    }

    ofPoint front[2];
    ofPoint back[2];

    front[0] = p->front->p;
    back[0] = p->back->p;

    for (int i = 0; i < lines.size(); i++) {
        if (lines[i] == NULL) {
            continue;
        }
        InteractiveContainer *l = lines[i];
        if (l->front == NULL || l->closed) continue;
        if (l == p) continue;
        front[1] = l->front->p;
        back[1] = l->back->p;

        if (close(front[0], front[1])) {

            ConnectPolylinesAction *connect = new ConnectPolylinesAction();
            connect->p1.cloneFrom(p);
            connect->p2.cloneFrom(l);
            connect->reverse = true;
            connect->add_back = false;
            addAction(connect);

            return;
        } else if (close(front[0], back[1])) {

            ConnectPolylinesAction *connect = new ConnectPolylinesAction();
            connect->p1.cloneFrom(p);
            connect->p2.cloneFrom(l);
            connect->reverse = false;
            connect->add_back = false;
            addAction(connect);

            return;
        } else if (close(back[0], back[1])) {

            ConnectPolylinesAction *connect = new ConnectPolylinesAction();
            connect->p1.cloneFrom(p);
            connect->p2.cloneFrom(l);
            connect->reverse = true;
            connect->add_back = true;
            addAction(connect);

            return;
        } else if (close(back[0], front[1])) {

            ConnectPolylinesAction *connect = new ConnectPolylinesAction();
            connect->p1.cloneFrom(p);
            connect->p2.cloneFrom(l);
            connect->reverse = false;
            connect->add_back = true;
            addAction(connect);

            return;
        }
    }
}