//
//  Geometry.h
//  simple_card
//
//  Created by sk on 9/24/14.
//
//

#pragma once

#include "ofMain.h"
#include "InteractiveObject.h"
#include "Joint.h"

float segmentDistance(ofPoint v, ofPoint w, ofPoint p);
ofPoint lineProjection(ofPoint v, ofPoint w, ofPoint p);

class Polyline;

class Vertex: public InteractiveObject {
public:
    Vertex() {
        type = VERTEX;
    };
    virtual ~Vertex() {};
    void draw();
    InteractiveObject *getCopy();
};

class Polyline: public InteractiveContainer {
public:
    Polyline() {
        front = NULL;
        back = NULL;
        closed = false;
    };
    virtual ~Polyline() {};

    // geometry specific
    ofPath path;
    ofPolyline ofp;

    // virtual
    void draw();
    void update();
};
