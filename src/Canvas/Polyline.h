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

class Polyline: public InteractiveContainer {
public:
    Polyline() {};
    virtual ~Polyline() {};

    // geometry specific
    ofPath path;
    ofPolyline ofp;

    // virtual
    void draw();
    void update();
    void update(ofPoint p);
    bool inside(ofPoint p);

    InteractiveContainer() {
        hover = false;
        selected = false;
        z_index = 0;
        front = NULL;
        back = NULL;
        closed = false;
        angle = 0.0f;
        fixed = false;
        controlled = false;
    }
    virtual ~InteractiveContainer() {
        release();
    };

    virtual void draw() {};
    virtual void update() {};
    virtual void update(ofPoint p) {};
    virtual bool inside(ofPoint p) {};

    void updateIndexes();

    void cloneFrom(InteractiveContainer *p);
    void init(InteractiveObject *p);
    void addBack(InteractiveObject *p);
    void popBack();
    void addFront(InteractiveObject *p);
    void addBack(InteractiveContainer *p);
    void addFront(InteractiveContainer *p);

    void release();
    void reverse();
    int getLength();

    bool selected;
    bool hover;
    ofPoint p;
    float angle;
    bool fixed;
    bool controlled;
    int pivot_i;

    // larger indexes in front
    // using only 0 and 1 for now
    int z_index;

    InteractiveObject *front;
    InteractiveObject *back;
    bool closed; // list can be circular

    int id;

    // connections
    vector<int> links; // connected joints
    // joints relative positions
    vector<ofPoint> links_rel; // from center to the joints

protected:
    // array of ordered vertices,
    // for random access
    vector<InteractiveObject *> items;

};
