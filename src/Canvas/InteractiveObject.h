//
//  InteractiveObject.h
//  simple_cad
//
//  Created by sk on 10/28/14.
//
//

#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "Motion.h"

struct ItemId {
    int container_id;
    int item_id;
};

class SelectionList {
public:
    vector<ItemId> items;
    void add(ItemId item_id);
    void clear();
};

class InteractiveContainer;
class InteractiveObject;
class Canvas;

class InteractiveObject {
public:
    static int points_step;
    static float zoom;
    static ofPoint offset;

    InteractiveObject() {
        hover = false;
        selected = false;
        next = NULL;
        prev = NULL;
        parent = NULL;
        dragged = false;
        grid_snap = true;
        angle = 0.0f;
        fixed = false;
        connected = false;
        fit_angle = 0.0f;
        controlled = false;
    }
    virtual ~InteractiveObject() {};
    virtual void draw() {};
    virtual InteractiveObject *getCopy() {};
    ofPoint getPx();
    ofPoint getPx(ofPoint p);
    ItemId getId();

    InteractiveObject *next;
    InteractiveObject *prev;

    ofPoint p; // position
    ofPoint start_p; // for dragging
    float angle;

    // should be moved to the class for joints
    ofxFloatSlider angle_slider;
    ofxFloatSlider velocity;
    ofxPanel gui;

    bool selected;
    bool hover;
    bool dragged;
    bool grid_snap;
    bool fixed;
    bool connected;
    bool controlled;

    string label;
    ofPoint v1;
    ofPoint v2;
    float fit_angle;

    InteractiveContainer *parent;

    int id;
};

class InteractiveContainer {
public:
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

    int getId();
    void release();
    void reverse();
    int getLength();
    InteractiveObject *getItem(int i);

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
