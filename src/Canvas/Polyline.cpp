//
//  Geometry.cpp
//  simple_card
//
//  Created by sk on 10/1/14.
//
//

#include "Polyline.h"

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
