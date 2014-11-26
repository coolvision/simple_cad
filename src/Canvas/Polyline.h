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
#include "Vertex.h"

class Polyline: public InteractiveContainer {
public:
    Polyline() {
        front = NULL;
        back = NULL;
        closed = false;
    };
    virtual ~Polyline() {
        release();
    };

    // interface implementation
    void draw();
    void update();
    InteractiveObject *Polyline::getItem(int i);
    
    // geometry specific
    ofPoint p;
    ofPath path;
    ofPolyline ofp;
    void update(ofPoint p);
    bool inside(ofPoint p);

    // manage linked list
    void updateIndexes();
    void cloneFrom(Polyline *p);
    void init(Vertex *p);
    void addBack(Vertex *p);
    void popBack();
    void addFront(Vertex *p);
    void addBack(Polyline *p);
    void addFront(Polyline *p);
    void release();
    void reverse();
    int getLength();

    Vertex *front;
    Vertex *back;
    bool closed; // list can be circular

protected:
    // array of ordered vertices,
    // for random access
    vector<Vertex *> items;
};
