//
//  Vertex.cpp
//  simple_cad
//
//  Created by sk on 11/26/14.
//
//

#include "Vertex.h"
#include "Polyline.h"

ofImage Vertex::dot_icon;

void Vertex::release() {

    if (next != NULL) {
        next->prev = prev;
    }
    if (prev != NULL) {
        prev->next = next;
    }
    
    Polyline *p = (Polyline *)parent;

    if (this == p->front && this == p->back) {
        p->front = NULL;
        p->back = NULL;
    } else if (this == p->front) {
        p->front = next;
    } else if (this == p->back) {
        p->back = prev;
    }
};

void Vertex::update() {

    Polyline *p = (Polyline *)parent;
    p->update();
}

InteractiveObject *Vertex::getCopy() {

    Vertex *v = new Vertex();
    v->p = this->p;
    v->start_p = this->start_p;
    v->parent = NULL;

    return v;
}

void Vertex::draw() {

    float point_size = 3.0f * zoom;
    if (point_size < 1.0f) {
        point_size = 1.0f;
    }
    if (point_size > 3.0f) {
        point_size = 3.0f;
    }

    if (!hover && !selected) {
        ofSetColor(80);
    }
    if (selected && !hover) {
        ofSetColor(ofColor::steelBlue);
    }
    if (hover) {
        ofSetColor(ofColor::orangeRed);
    }

    ofCircle(getPx(p), point_size);
}