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
    ItemId(int c, int i) {
        container_id = c;
        item_id = i;
    };
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
        parent = NULL;
        dragged = false;
        grid_snap = true;
    };
    virtual ~InteractiveObject() {};

    // interface
    virtual void draw() {};
    virtual InteractiveObject *getCopy() {};
    virtual void release() {};
    virtual void update() {};

    // UI
    ofPoint p; // position
    ofPoint start_p; // for dragging
    ofPoint getPx();
    ofPoint getPx(ofPoint p);
    bool selected;
    bool hover;
    bool dragged;
    bool grid_snap;

    InteractiveContainer *parent;

    int id;
    ItemId getId();
};

class InteractiveContainer {
public:
    InteractiveContainer() {
        hover = false;
        selected = false;
        z_index = 0;
    }
    virtual ~InteractiveContainer() {
    };

    virtual void draw() {};
    virtual void update() {};
    virtual InteractiveObject *getItem(int id) {};

    bool hover;
    bool selected;
    int z_index;

    int id;
    ItemId getId();
};

