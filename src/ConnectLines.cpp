//
//  ConnectLines.cpp
//  simple_card
//
//  Created by sk on 10/2/14.
//
//

#include "ofApp.h"

// connect to an existing polyline, or add a new one
Polyline *ofApp::connectLine(ofPoint *p1, ofPoint *p2) {

    bool new_line = true;

    for (int i = 0; i < lines.size(); i++) {
        Polyline *l = lines[i];
        if (l->front == NULL || l->closed) continue;
        if ((*p1 == *l->front && *p2 == *l->back) ||
            (*p1 == *l->back && *p2 == *l->front)) {
            // does it, by chance close the polyline?
            l->back->next = l->front;
            l->front->prev = l->back;
            l->closed = true;
            return lines[i];
        } else if (*p1 == *l->front) {
            l->addFront(*p2);
            return l;
        } else if (*p2 == *l->front) {
            l->addFront(*p1);
            return l;
        } else if (*p1 == *l->back) {
            l->addBack(*p2);
            return l;
        } else if (*p2 == *l->back) {
            l->addBack(*p1);
            return l;
        }
    }

    *curr_line.back = *p2;
    lines.push_back(new Polyline());
    lines.back()->cloneFrom(&curr_line);
    return lines.back();
}

// check if endpoints overlap, and if they do, connect the polylines
Polyline *ofApp::connectPolylines(Polyline *p) {

    if (p->front == NULL || p->closed) {
        return p;
    }

    if (*p->front == *p->back && p->getLength() > 2) {

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

        if (front[0] == front[1]) {
            l->reverse();
            for (Vertex *v = l->back->prev; v != NULL; v = v->prev) {
                p->addFront(ofPoint(*v));
            }
            l->release();
            return p;
        } else if (front[0] == back[1]) {
            for (Vertex *v = l->back->prev; v != NULL; v = v->prev) {
                p->addFront(ofPoint(*v));
            }
            l->release();
            return p;
        } else if (back[0] == back[1]) {
            l->reverse();
            for (Vertex *v = l->front->next; v != NULL; v = v->next) {
                p->addBack(ofPoint(*v));
            }
            l->release();
            return p;
        } else if (back[0] == front[1]) {
            for (Vertex *v = l->front->next; v != NULL; v = v->next) {
                p->addBack(ofPoint(*v));
            }
            l->release();
            return p;
        }
    }
}