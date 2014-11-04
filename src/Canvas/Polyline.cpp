//
//  Geometry.cpp
//  simple_card
//
//  Created by sk on 10/1/14.
//
//

#include "Polyline.h"

void Polyline::draw() {

    if (front == NULL || back == NULL) {
        return;
    }

    ofPoint center;
    int n = 0;
    for (InteractiveObject *v = front; v != NULL; v = v->next) {
        center += v->p;
        n++;
        if (v->next == front) break; // closed polylines
    }
    center /= (float)n;
    p = center;

    ofPushStyle();

    ofEnableSmoothing();
    ofEnableAntiAliasing();

    float point_size = 3.0f * InteractiveObject::zoom;
    if (point_size < 1.0f) {
        point_size = 1.0f;
    }

    selected = true;
    for (InteractiveObject *v = front; v != NULL && v->next != NULL; v = v->next) {
        if (!v->selected) {
            selected = false;
            break;
        }
        if (v->next == front) break; // closed polylines
    }

    if (closed) {
        path.setFilled(true);
        if (hover || selected) {
            path.setFillColor(ofColor(190, 190, 190, 200));
        } else {
            path.setFillColor(ofColor(200, 200, 200, 200));
        }
    } else {
        path.setFilled(false);
    }
    path.draw();

    ofSetLineWidth(1.0f);
    ofSetColor(ofColor::black);

    for (InteractiveObject *v = front; v != NULL && v->next != NULL; v = v->next) {
        if (v->hover && v->next->hover) {
            ofSetColor(ofColor::orangeRed);
        } else if (v->selected && v->next->selected) {
            ofSetColor(ofColor::steelBlue);
        } else {
            ofSetColor(ofColor::black);
        }
        ofLine(v->getPx(v->p), v->getPx(v->next->p));
        if (v->next == front) break; // closed polylines
    }

    for (InteractiveObject *v = front; v != NULL; v = v->next) {
        if (!v->hover && !v->selected) {
            v->draw();
        }
        if (v->next == front) break; // closed polylines
    }

    for (InteractiveObject *v = front; v != NULL; v = v->next) {
        if (v->selected && !v->hover) {
            v->draw();
        }
        if (v->next == front) break; // closed polylines
    }

    for (InteractiveObject *v = front; v != NULL; v = v->next) {
        if (v->hover) {
            v->draw();
        }
        if (v->next == front) break; // closed polylines
    }

    ofDisableAntiAliasing();
    ofDisableSmoothing();
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

bool Polyline::inside(ofPoint p) {

    if (ofp.inside(p)) {
        return true;
    }

    return false;
}

void Polyline::update(ofPoint p) {
    update();

    if (ofp.inside(p)) {
        hover = true;
    } else {
        hover = false;
    }
}

void Polyline::update() {

    updateIndexes();

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
