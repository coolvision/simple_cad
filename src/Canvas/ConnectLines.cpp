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

// connect to an existing polyline, or add a new one
Polyline *Canvas::connectLine(ofPoint *p1, ofPoint *p2) {

    bool new_line = true;

    for (int i = 0; i < lines.size(); i++) {
        Polyline *l = lines[i];
        if (l->front == NULL || l->closed) continue;
        if ((close(*p1, *l->front) && close(*p2, *l->back)) ||
            (close(*p1, *l->back) && close(*p2, *l->front))) {
            // does it, by chance close the polyline?
            l->back->next = l->front;
            l->front->prev = l->back;
            l->closed = true;
            return lines[i];
        } else if (close(*p1, *l->front)) {
            l->addFront(*p2);
            return l;
        } else if (close(*p2, *l->front)) {
            l->addFront(*p1);
            return l;
        } else if (close(*p1, *l->back)) {
            l->addBack(*p2);
            return l;
        } else if (close(*p2, *l->back)) {
            l->addBack(*p1);
            return l;
        }
    }

    resetActions();
    AddLineAction* add_line = new AddLineAction();
    add_line->p[0] = *p1;
    add_line->p[1] = *p2;
    actions.push_back(add_line);
    actions.back()->doAction(this);

//    *curr_line.back = *p2;
//    lines.push_back(new Polyline());
//    lines.back()->cloneFrom(&curr_line);

//    return lines.back();

    return NULL;
}

// check if endpoints overlap, and if they do, connect the polylines
Polyline *Canvas::connectPolylines(Polyline *p) {

    if (p->front == NULL || p->closed) {
        return p;
    }

    if (close(*p->front, *p->back) && p->getLength() > 2) {

        Vertex *tmp = p->back;
        p->back = p->back->prev;
        delete tmp;

        p->back->next = p->front;
        p->front->prev = p->back;
        p->closed = true;
        return p;
    }

    ofPoint front[2];
    ofPoint back[2];

    front[0] = *p->front;
    back[0] = *p->back;
    for (int i = 0; i < lines.size(); i++) {
        Polyline *l = lines[i];
        if (l->front == NULL || l->closed) continue;
        if (l == p) continue;
        front[1] = *l->front;
        back[1] = *l->back;

        if (close(front[0], front[1])) {
            l->reverse();
            for (Vertex *v = l->back->prev; v != NULL; v = v->prev) {
                p->addFront(ofPoint(*v));
            }
            l->release();
            return p;
        } else if (close(front[0], back[1])) {
            for (Vertex *v = l->back->prev; v != NULL; v = v->prev) {
                p->addFront(ofPoint(*v));
            }
            l->release();
            return p;
        } else if (close(back[0], back[1])) {
            l->reverse();
            for (Vertex *v = l->front->next; v != NULL; v = v->next) {
                p->addBack(ofPoint(*v));
            }
            l->release();
            return p;
        } else if (close(back[0], front[1])) {
            for (Vertex *v = l->front->next; v != NULL; v = v->next) {
                p->addBack(ofPoint(*v));
            }
            l->release();
            return p;
        }
    }
}