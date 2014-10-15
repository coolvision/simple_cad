//
//  Canvas.cpp
//  simple_card
//
//  Created by sk on 10/15/14.
//
//

#include "Canvas.h"



ofPoint ofApp::snap(ofPoint p) {

    if (points_step > 0) {
        p.x = round(p.x / points_step) * points_step;
        p.y = round(p.y / points_step) * points_step;
    }

    return p;
}

ofPoint ofApp::snapMm(ofPoint p) {

    p.x = round(p.x);
    p.y = round(p.y);

    return p;
}

ofPoint ofApp::getPx(ofPoint p) {

    // how many px per mm
    // points_step = 8.0f * zoom;
    p *= points_step;
    p += canvas_offset;

    return p;
}

ofPoint ofApp::getPx(Vertex *v) {
    return getPx(*v);
}

ofPoint ofApp::getMm(ofPoint p) {

    p -= canvas_offset;
    p /= points_step;
    
    return p;
}

void ofApp::zoomIn() {

    zoom += 0.2f;
    if (zoom > 5.0f) {
        zoom = 5.0f;
    }
    setGrid();
}

void ofApp::zoomOut() {

    zoom -= 0.2f;
    if (zoom < 0.2f) {
        zoom = 0.2f;
    }
    setGrid();
}


