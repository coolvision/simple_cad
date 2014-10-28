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

float segmentDistance(ofPoint v, ofPoint w, ofPoint p);
ofPoint lineProjection(ofPoint v, ofPoint w, ofPoint p);

class Polyline;

class Vertex: public ofPoint, public InteractiveObject {
public:

    Vertex() {
        next = NULL;
        prev = NULL;
        parent = NULL;
    }

    Vertex *next;
    Vertex *prev;

    Polyline *polyline;

    Vertex &operator =(const ofVec3f &p);
};

inline Vertex &Vertex::operator =(const ofVec3f &p) {
    x = p.x;
    y = p.y;
    z = p.z;
    return *this;
}

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

    void release();
    void cloneFrom(Polyline *p);

    void init(ofPoint p);
    void addFront(ofPoint p);
    void addBack(ofPoint p);
    void addBack(Vertex *vertex);
    void reverse();
    void addFront(Polyline *p);
    void addBack(Polyline *p);
    int getLength();
    Vertex *getVertex(int i);

    void toPolygon();

    bool closed;

    Vertex *front;
    Vertex *back;

    // for drawing
    void updatePath();
    ofPath path;
    ofPolyline ofp;

    void updateIndexes();

private:
    // array of ordered vertices,
    // for random access
    vector<Vertex *> vertices;
};
