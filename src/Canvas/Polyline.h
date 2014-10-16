//
//  Geometry.h
//  simple_card
//
//  Created by sk on 9/24/14.
//
//

#pragma once

#include "ofMain.h"

float segmentDistance(ofPoint v, ofPoint w, ofPoint p);
ofPoint lineProjection(ofPoint v, ofPoint w, ofPoint p);

class Polyline;

struct VertexId {
    int line_i;
    int v_i;
};

class SelectionList {
public:
    vector<VertexId> vertices;
    vector<ofPoint> start_p;
    void add(VertexId v_id, ofPoint p);
    void clear();
};

class Vertex: public ofPoint {
public:

    static int points_step;
    static ofPoint offset;

    Vertex() {
        hover = false;
        selected = false;
        next = NULL;
        prev = NULL;
        p = NULL;
    }
    ofPoint getPx();
    VertexId getId();

    bool selected;
    bool hover;
    Vertex *next;
    Vertex *prev;
    Polyline *p;

    Vertex &operator =(const ofVec3f &p);

    int i; // index in the polyline
};

inline Vertex &Vertex::operator =(const ofVec3f &p) {
    x = p.x;
    y = p.y;
    z = p.z;
    return *this;
}

class Polyline {
public:
    Polyline() {
        selected = false;
        hover = false;
        front = NULL;
        back = NULL;
        closed = false;
    };
    ~Polyline() {
        release();
    }

    void release();
    void cloneFrom(Polyline *p);

    void init(ofPoint p);
    void addFront(ofPoint p);
    void addBack(ofPoint p);
    void reverse();
    void addFront(Polyline *p);
    void addBack(Polyline *p);
    int getLength();
    Vertex *getVertex(int i);
    int getId();

    bool closed;

    bool selected;
    bool hover;

    Vertex *front;
    Vertex *back;

    // for drawing
    void updatePath();
    ofPath path;
    ofPolyline ofp;

    // index in the polylines array
    int i;

private:
    // array of ordered vertices,
    // for random access
    vector<Vertex *> vertices;
    void updateIndexes();
};
