//
//  Geometry.h
//  simple_card
//
//  Created by sk on 9/24/14.
//
//

#pragma once

#include "ofMain.h"

class Polyline;

class Vertex: public ofPoint {
public:
    Vertex() {
        selected = false;
        hover = false;
        next = NULL;
        prev = NULL;
    }

    bool selected;
    bool hover;
    Vertex *next;
    Vertex *prev;
    Polyline *p;

    Vertex &operator =(const ofVec3f &p);
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

    int getLength();
    void release();
    void cloneFrom(Polyline *p);

    void init(ofPoint p);
    void addFront(ofPoint p);
    void addBack(ofPoint p);
    void reverse();
    void connectToFront(Polyline *p);
    void connectToBack(Polyline *p);

    bool closed;

    bool selected;
    bool hover;

    Vertex *front;
    Vertex *back;

    // for drawing
    void updatePath();
    ofPath path;

private:
    int length;
};
