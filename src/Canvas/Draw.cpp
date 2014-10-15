//
//  Draw.cpp
//  simple_card
//
//  Created by sk on 10/15/14.
//
//

#include <Canvas.h>

void ofApp::drawLine(Polyline *l) {

    if (l->front == NULL || l->back == NULL) {
        return;
    }

    ofPushStyle();

    ofEnableSmoothing();
    ofEnableAntiAliasing();

    float point_size = 4.0f * zoom;
    if (point_size < 1.0f) {
        point_size = 1.0f;
    }

    if (l->closed) {
        l->path.setFilled(true);
        l->path.setFillColor(ofColor(200, 200, 200, 200));
    } else {
        l->path.setFilled(false);
    }
    l->path.draw();

    ofSetLineWidth(1.0f);
    ofSetColor(ofColor::black);
    for (Vertex *v = l->front; v != NULL && v->next != NULL; v = v->next) {
        if (v->hover && v->next->hover) {
            ofSetColor(ofColor::orangeRed);
        } else if (v->selected && v->next->selected) {
            ofSetColor(ofColor::steelBlue);
        } else {
            ofSetColor(ofColor::black);
        }
        ofLine(getPx(v), getPx(v->next));
        if (v->next == l->front) break; // closed polylines
    }

    ofSetColor(ofColor::black);
    for (Vertex *v = l->front; v != NULL; v = v->next) {
        if (!v->hover && !v->selected) {
            ofCircle(getPx(v), point_size);
        }
        if (v->next == l->front) break; // closed polylines
    }

    ofSetColor(ofColor::steelBlue);
    for (Vertex *v = l->front; v != NULL; v = v->next) {
        if (v->selected && !v->hover) {
            ofCircle(getPx(v), point_size);
        }
        if (v->next == l->front) break; // closed polylines
    }

    ofSetColor(ofColor::orangeRed);
    for (Vertex *v = l->front; v != NULL; v = v->next) {
        if (v->hover) {
            ofCircle(getPx(v), point_size);
        }
        if (v->next == l->front) break; // closed polylines
    }

    ofDisableAntiAliasing();
    ofDisableSmoothing();
}
