//
//  Joint.h
//  simple_cad
//
//  Created by sk on 10/23/14.
//
//

#pragma once

#include "ofMain.h"
#include "InteractiveObject.h"

enum JointType {
    JOINT_FIXED = 0,
    JOINT_REVOLUTE
};

class Joint;

class JointClickEventData {
public:
    JointClickEventData(Joint *b): joint_p(b) {};
    Joint *joint_p;
};

class Joint: public InteractiveObject {
public:
    Joint() {};
    virtual ~Joint() {};

    int width;
    int height;

    JointType type;

    // events
    ofEvent<JointClickEventData> click_event;
};

class JointContainer: public InteractiveContainer {
public:
    JointContainer() {
        joint = new Joint();
    };
    virtual ~JointContainer() {
        delete joint;
    };

    void draw();

    static ofImage joint_icon_fixed;
    static ofImage joint_icon_r;

    Joint *joint;
};
