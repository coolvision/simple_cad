//
//  Vertex.cpp
//  simple_cad
//
//  Created by sk on 11/26/14.
//
//

#include "Vertex.h"

ofImage Vertex::dot_icon;

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