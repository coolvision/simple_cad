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
    Vertex() {};
    virtual ~Vertex() {};
    void draw();
};

class Polyline: public InteractiveContainer {
public:
    Polyline() {
        front = NULL;
        back = NULL;
        closed = false;
    };
    virtual ~Polyline() {
        release();
    }

    void cloneFrom(Polyline *p);
    void init(ofPoint p);

    void addFront(ofPoint p);

    void addBack(ofPoint p);
    void addBack(Vertex *vertex);
    void addBack(Joint *j);

    void addFront(Polyline *p);
    void addBack(Polyline *p);

    // geometry specific
    void toPolygon();
    bool closed;

    ofPath path;
    ofPolyline ofp;

    // virtual
    void draw();
    void update();
};
