//
//  Geometry.cpp
//  simple_card
//
//  Created by sk on 10/1/14.
//
//

#include "Polyline.h"

int Vertex::points_step = 0;
ofPoint Vertex::offset;

ofPoint Vertex::getPx() {

    // how many px per mm
    // points_step = 8.0f * zoom;

    ofPoint p = *this;
    p *= points_step;
    p += offset;
    
    return p;
}

VertexId Vertex::getId() {

    VertexId v_id;
    v_id.line_i = p->getId();
    v_id.v_i = i;

    return v_id;
}

int Polyline::getId() {
    return i;
}

// can be made more efficient by maintaining an array of the vertices
Vertex *Polyline::getVertex(int i) {

    if (i < vertices.size()) {
        return vertices[i];
    }
    return NULL;
}

void Polyline::updateIndexes() {

    int n = getLength();
    vertices.resize(n);

    int l = 0;
    for (Vertex *v = front; v != NULL; v = v->next) {
        vertices[l] = v;
        v->i = l;
        l++;
        if (v->next == front) break; // closed polylines
    }
}

void Polyline::release() {

    if (front != NULL) {
        Vertex *v = front;
        Vertex *tmp;
        while (v != NULL) {
            tmp = v->next;
            delete v;
            v = tmp;
            if (tmp == front) {  // closed polylines
                break;
            }
        }
    }
    front = NULL;
    back = NULL;

    updatePath();
    updateIndexes();
}

void Polyline::cloneFrom(Polyline *p) {

    *this = *p;
    front = NULL;
    back = NULL;

    release();

    for (Vertex *v = p->front; v != NULL; v = v->next) {
        addBack(ofPoint(*v));
        if (v->next == p->front) break; // closed polylines
    }
    if (closed) {
        back->next = front;
        front->prev = back;
    }
    
    updateIndexes();
}

void Polyline::init(ofPoint p) {
    front = new Vertex();
    back = front;
    *front = p;
    front->p = this;
    updateIndexes();
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
    updateIndexes();
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
    updateIndexes();
}

void Polyline::reverse() {

    if (front == NULL) {
        return;
    }

    Vertex *tmp;
    Vertex *v = front;
    for (Vertex *v = front; v != NULL; v = tmp) {
        tmp = v->next;
        v->next = v->prev;
        v->prev = tmp;
        v = tmp;
        if (tmp == front) break; // closed polylines
    }
    tmp = front;
    front = back;
    back = tmp;
    updateIndexes();
}

void Polyline::addFront(Polyline *p) {

    if (front == NULL || p->closed) {
        return;
    }

    for (Vertex *v = p->back; v != NULL; v = v->prev) {
        addFront(ofPoint(*v));
    }
}

void Polyline::addBack(Polyline *p) {

    if (front == NULL || p->closed) {
        return;
    }

    for (Vertex *v = p->front; v != NULL; v = v->next) {
        addBack(ofPoint(*v));
    }
}

void Polyline::updatePath() {

    path.clear();
    for (Vertex *v = front; v != NULL; v = v->next) {
        if (v == front) {
            path.newSubPath();
            path.moveTo(v->getPx());
        }
        path.lineTo(v->getPx());
        if (v->next == front) break; // closed polylines
    }

    ofp.clear();
    for (Vertex *v = front; v != NULL; v = v->next) {
        ofp.addVertex(v->getPx());
        if (v->next == front) break; // closed polylines
    }
    ofp.setClosed(closed);
}

int Polyline::getLength() {
    int l = 0;
    for (Vertex *v = front; v != NULL; v = v->next) {
        l++;
        if (v->next == front) break; // closed polylines
    }
    return l;
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
