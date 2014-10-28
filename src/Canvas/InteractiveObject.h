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

class InteractiveContainer {
public:
    InteractiveContainer() {
        hover = false;
        selected = false;
    }
    virtual ~InteractiveContainer() {};

    virtual void draw() {};

    int getId();
    
    bool selected;
    bool hover;

    int id;
};

class InteractiveObject {
public:
    InteractiveObject() {
        hover = false;
        selected = false;
    }

    ofPoint getPx();
    ofPoint getPx(ofPoint p);
    ItemId getId();

    static int points_step;
    static float zoom;
    static ofPoint offset;

    ofPoint p; // position
    bool selected;
    bool hover;
    ofPoint start_p; // for dragging

    InteractiveContainer *parent;

    int id;
};