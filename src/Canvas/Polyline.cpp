//
//  Geometry.cpp
//  simple_card
//
//  Created by sk on 10/1/14.
//
//

#include "Polyline.h"

void Vertex::draw() {

    float point_size = 3.0f * zoom;
    if (point_size < 1.0f) {
        point_size = 1.0f;
    }

    if (!hover && !selected) {
        ofSetColor(ofColor::black);
    }
    if (selected && !hover) {
        ofSetColor(ofColor::steelBlue);
    }
    if (hover) {
        ofSetColor(ofColor::orangeRed);
    }

    ofCircle(getPx(p), point_size);
}

void Polyline::cloneFrom(Polyline *p) {

    *this = *p;
    front = NULL;
    back = NULL;

    release();

    for (InteractiveObject *v = p->front; v != NULL; v = v->next) {
        addBack(v->p);
        if (v->next == p->front) break; // closed polylines
    }
    if (closed) {
        back->next = front;
        front->prev = back;
    }
    
    update();
}

void Polyline::init(ofPoint p) {
    front = new Vertex();
    back = front;
    front->p = p;
    front->parent = this;
    update();
}

void Polyline::addBack(Joint *j_in) {

    if (front == NULL) {
        Joint *j = new Joint();
        j->p = j_in->p;
        j->start_p = j_in->start_p;
        j->type = j_in->type;
        j->parent = this;
        front = j;
        back = front;
    } else {
        // new element
        Joint *j = new Joint();
        j->p = j_in->p;
        j->start_p = j_in->start_p;
        j->type = j_in->type;
        j->parent = this;
        // update the list
        back->next = j;
        j->prev = back;
        back = j;
    }
    update();
}

void Polyline::addBack(Vertex *vertex) {

    if (front == NULL) {
        init(vertex->p);
    } else {
        // new element
        Vertex *v = new Vertex();
        v->p = vertex->p;
        v->start_p = vertex->start_p;
        v->parent = this;
        // update the list
        back->next = v;
        v->prev = back;
        back = v;
    }
    update();
}

void Polyline::addBack(ofPoint p) {

    if (front == NULL) {
        init(p);
    } else {
        // new element
        Vertex *v = new Vertex();
        v->p = p;
        v->parent = this;
        // update the list
        back->next = v;
        v->prev = back;
        back = v;
    }
    update();
}

void Polyline::addFront(ofPoint p) {

    if (front == NULL) {
        init(p);
    } else {
        // new element
        Vertex *v = new Vertex();
        v->p = p;
        v->parent = this;
        // update the list
        front->prev = v;
        v->next = front;
        front = v;
    }
    update();
}

void Polyline::addFront(Polyline *p) {

    if (front == NULL || p->closed) {
        return;
    }

    for (InteractiveObject *v = p->back; v != NULL; v = v->prev) {
        addFront(v->p);
    }
}

void Polyline::addBack(Polyline *p) {

    if (front == NULL || p->closed) {
        return;
    }

    for (InteractiveObject *v = p->front; v != NULL; v = v->next) {
        addBack(v->p);
    }
}

void Polyline::toPolygon() {
    back->next = front;
    front->prev = back;
}

void Polyline::update() {

    int n = getLength();
    items.resize(n);

    int l = 0;
    for (InteractiveObject *v = front; v != NULL; v = v->next) {
        items[l] = v;
        v->id = l;
        l++;
        if (v->next == front) break; // closed polylines
    }

    path.clear();
    for (InteractiveObject *v = front; v != NULL; v = v->next) {
        if (v == front) {
            path.newSubPath();
            path.moveTo(v->getPx(v->p));
        }
        path.lineTo(v->getPx(v->p));
        if (v->next == front) break; // closed polylines
    }

    ofp.clear();
    for (InteractiveObject *v = front; v != NULL; v = v->next) {
        ofp.addVertex(v->getPx(v->p));
        if (v->next == front) break; // closed polylines
    }
    ofp.setClosed(closed);
}

float segmentDistance(ofPoint v, ofPoint w, ofPoint p) {

    // Return minimum distance between line segment vw and point p
    float l2 = (v - w).lengthSquared();  // i.e. |w-v|^2 -  avoid a sqrt

    if (l2 == 0.0) {
        return (p, v).length();   // v == w case
    }
    // Consider the line extending the segment, parameterized as v + t (w - v).
    // We find projection of point p onto the line.
    // It falls where t = [(p-v) . (w-v)] / |w-v|^2
    float t = (p - v).dot(w - v) / l2;
    if (t < 0.0) {
        return (p - v).length();       // Beyond the 'v' end of the segment
    } else if (t > 1.0) {
        return (p - w).length();  // Beyond the 'w' end of the segment
    }
    ofPoint projection = v + t * (w - v);  // Projection falls on the segment

    return (p - projection).length();
}

ofPoint lineProjection(ofPoint v, ofPoint w, ofPoint p) {

    // Return minimum distance between line segment vw and point p
    float l2 = (v - w).lengthSquared();  // i.e. |w-v|^2 -  avoid a sqrt
    if (l2 == 0.0) {
        return p;   // v == w case
    }
    // Consider the line extending the segment, parameterized as v + t (w - v).
    // We find projection of point p onto the line.
    // It falls where t = [(p-v) . (w-v)] / |w-v|^2
    float t = (p - v).dot(w - v) / l2;

    return v + t * (w - v);
}
