//
//  Canvas.cpp
//  simple_card
//
//  Created by sk on 10/15/14.
//
//

#include "Canvas.h"

ofPoint Canvas::snap(ofPoint p) {

    if (px_step > 0) {
        p.x = round(p.x / px_step) * px_step;
        p.y = round(p.y / px_step) * px_step;
    }

    return p;
}

ofPoint Canvas::snapMm(ofPoint p) {

    p.x = round(p.x);
    p.y = round(p.y);

    return p;
}

ofPoint Canvas::getPx(ofPoint p) {

    // how many px per mm
    // points_step = 8.0f * zoom;
    p *= px_step;
    p += canvas_offset;

    return p;
}

ofPoint Canvas::getPx(Vertex *v) {
    return getPx(*v);
}

ofPoint Canvas::getMm(ofPoint p) {

    p -= canvas_offset;
    p /= px_step;
    
    return p;
}

void Canvas::zoomIn() {

    zoom += 0.2f;
    if (zoom > 5.0f) {
        zoom = 5.0f;
    }
}

void Canvas::zoomOut() {

    zoom -= 0.2f;
    if (zoom < 0.2f) {
        zoom = 0.2f;
    }
}

