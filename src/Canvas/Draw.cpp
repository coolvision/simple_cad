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
