//
//  Motion.h
//  simple_cad
//
//  Created by sk on 11/19/14.
//
//

#pragma once

#include "ofMain.h"
#include "ofxGui.h"

class InteractiveContainer;
class InteractiveObject;
class Canvas;

enum MessageType {
    UPDATE_RELATIVE = 0,
    ROTATION,
    FIT_RELATIVE,
    MESSAGE_TYPES_N
};

class Motion {
public:
    Motion(): total_motion(0.0f) {};
    virtual void apply(InteractiveContainer *c, Canvas *canvas) {};
    virtual void apply(InteractiveObject *i, Canvas *canvas) {};
    virtual Motion *getCopy() {};
    string label;
    int receiver_id;
    int sent_stamp;
    int forward_stamp;
    int sender_type;
    float total_motion;
    int type_i;
};

struct LinearMotion: public Motion {
    void apply(InteractiveContainer *c, Canvas *canvas);
    void apply(InteractiveObject *i, Canvas *canvas);
    Motion *getCopy();
    ofPoint v;
};

struct UpdateRelative: public Motion {
    void apply(InteractiveContainer *c, Canvas *canvas);
    void apply(InteractiveObject *i, Canvas *canvas);
    Motion *getCopy();
};

struct FitRelative: public Motion {
    void apply(InteractiveContainer *c, Canvas *canvas);
    void apply(InteractiveObject *i, Canvas *canvas);
    Motion *getCopy();
};

struct Rotation: public Motion {
    void apply(InteractiveContainer *c, Canvas *canvas);
    void apply(InteractiveObject *i, Canvas *canvas);
    Motion *getCopy();
    ofPoint pivot;
    float angle;
};