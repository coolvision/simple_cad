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

enum ItemType {
    VERTEX = 0,
    JOINT
};

class InteractiveContainer;
class InteractiveObject;

class Motion {
public:
    virtual void apply(InteractiveContainer *c) {};
    virtual void apply(InteractiveObject *i) {};
    virtual Motion *getCopy() {};
    string label;
    int sender_id;
    int origin_id;
};

struct LinearMotion: public Motion {
    void apply(InteractiveContainer *c);
    void apply(InteractiveObject *i);
    Motion *getCopy();
    ofPoint v;
};

struct Rotation: public Motion {
    void apply(InteractiveContainer *c);
    void apply(InteractiveObject *i);
    Motion *getCopy();
    ofPoint pivot;
    float angle;
};

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

    // motion
    deque<Motion *> motion_msgs;
    void reset();

    // connections
    vector<int> links;
    vector<ofPoint> links_rel;

    bool selected;
    bool hover;
    bool dragged;
    bool grid_snap;

    InteractiveContainer *parent;

    ItemType type;
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

    // larger indexes in front
    // using only 0 and 1 for now
    int z_index;

    ofxFloatSlider angle;
    ofxPanel gui;

    InteractiveObject *front;
    InteractiveObject *back;
    bool closed; // list can be circular

    int id;

    // motion
    deque<Motion *> motion_msgs;
    void reset();

protected:
    // array of ordered vertices,
    // for random access
    vector<InteractiveObject *> items;
};
