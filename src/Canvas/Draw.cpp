//
//  Draw.cpp
//  simple_card
//
//  Created by sk on 10/15/14.
//
//

#include <Canvas.h>

void Polyline::draw() {

    if (front == NULL || back == NULL) {
        return;
    }

    ofPushStyle();

    ofEnableSmoothing();
    ofEnableAntiAliasing();

    float point_size = 3.0f * InteractiveObject::zoom;
    if (point_size < 1.0f) {
        point_size = 1.0f;
    }

    if (closed) {
        path.setFilled(true);
        path.setFillColor(ofColor(200, 200, 200, 200));
    } else {
        path.setFilled(false);
    }
    path.draw();

    ofSetLineWidth(1.0f);
    ofSetColor(ofColor::black);
    for (Vertex *v = front; v != NULL && v->next != NULL; v = v->next) {
        if (v->hover && v->next->hover) {
            ofSetColor(ofColor::orangeRed);
        } else if (v->selected && v->next->selected) {
            ofSetColor(ofColor::steelBlue);
        } else {
            ofSetColor(ofColor::black);
        }
        ofLine(v->getPx(*v), v->getPx(*v->next));
        if (v->next == front) break; // closed polylines
    }

    ofSetColor(ofColor::black);
    for (Vertex *v = front; v != NULL; v = v->next) {
        if (!v->hover && !v->selected) {
            ofCircle(v->getPx(*v), point_size);
        }
        if (v->next == front) break; // closed polylines
    }

    ofSetColor(ofColor::steelBlue);
    for (Vertex *v = front; v != NULL; v = v->next) {
        if (v->selected && !v->hover) {
            ofCircle(v->getPx(*v), point_size);
        }
        if (v->next == front) break; // closed polylines
    }

    ofSetColor(ofColor::orangeRed);
    for (Vertex *v = front; v != NULL; v = v->next) {
        if (v->hover) {
            ofCircle(v->getPx(*v), point_size);
        }
        if (v->next == front) break; // closed polylines
    }

    ofDisableAntiAliasing();
    ofDisableSmoothing();
}
