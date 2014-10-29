//
//  InteractiveObject.h
//  simple_cad
//
//  Created by sk on 10/28/14.
//
//

#pragma once

#include "ofMain.h"

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
    }
    virtual ~InteractiveObject() {};
    virtual void draw() {};
    ofPoint getPx();
    ofPoint getPx(ofPoint p);
    ItemId getId();

    InteractiveObject *next;
    InteractiveObject *prev;

    ofPoint p; // position
    ofPoint start_p; // for dragging

    bool selected;
    bool hover;

    InteractiveContainer *parent;

    int id;
};

class InteractiveContainer {
public:
    InteractiveContainer() {
        hover = false;
        selected = false;
        z_index = 0;
    }
    virtual ~InteractiveContainer() {};

    virtual void draw() {};
    virtual void update() {};

    int getId();
    void release();
    void reverse();
    int getLength();
    InteractiveObject *getItem(int i);

    bool selected;
    bool hover;
    // larger indexes in front
    // using only 0 and 1 for now
    int z_index;

    InteractiveObject *front;
    InteractiveObject *back;

    int id;

protected:
    // array of ordered vertices,
    // for random access
    vector<InteractiveObject *> items;
};