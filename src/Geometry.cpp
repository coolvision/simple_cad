//
//  Geometry.cpp
//  simple_card
//
//  Created by sk on 10/1/14.
//
//

#include "Geometry.h"

void Polyline::release() {

    if (front != NULL) {
        Vertex *v = front;
        Vertex *tmp;
        while (v != NULL) {
            tmp = v->next;
            delete v;
            v = tmp;
        }
    }
    front = NULL;
    back = NULL;

    updatePath();
}

void Polyline::cloneFrom(Polyline *p) {

    release();

    closed = p->closed;

    for (Vertex *v = p->front; v != NULL; v = v->next) {
        addBack(ofPoint(*v));
        if (v->next == p->front) break; // closed polylines
    }
}

void Polyline::init(ofPoint p) {
    front = new Vertex();
    back = front;
    *front = p;
    front->p = this;
}

void Polyline::addBack(ofPoint p) {

    if (front == NULL) {
        init(p);
    } else {
        // new element
        Vertex *v = new Vertex();
        *v = p;
        v->p = this;
        // update the list
        back->next = v;
        v->prev = back;
        back = v;
    }
    updatePath();
}

void Polyline::addFront(ofPoint p) {

    if (front == NULL) {
        init(p);
    } else {
        // new element
        Vertex *v = new Vertex();
        *v = p;
        v->p = this;
        // update the list
        front->prev = v;
        v->next = front;
        front = v;
    }
    updatePath();
}

void Polyline::updatePath() {

    path.clear();
    for (Vertex *v = front; v != NULL; v = v->next) {
        if (v == front) {
            path.newSubPath();
            path.moveTo(*v);
        }
        path.lineTo(*v);
        if (v->next == front) break; // closed polylines
    }
}

int Polyline::getLength() {


}


void Polyline::reverse() {


}

void Polyline::connectToFront(Polyline *p) {

}

void Polyline::connectToBack(Polyline *p) {

}