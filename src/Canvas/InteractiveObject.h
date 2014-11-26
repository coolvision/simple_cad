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
    }
    virtual ~InteractiveContainer() {
    };

    virtual void draw() = 0;
    virtual void update() = 0;

    int getId();
    InteractiveObject *getItem(int id);
};

