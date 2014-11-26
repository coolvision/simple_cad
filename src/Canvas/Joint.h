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
#include "ofxGui.h"

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
    Joint() {
        grid_snap = false;

        gui.setup();
        gui.add(angle_slider.setup("angle", 0.0f, -360.0f, 360.0f));
        gui.add(velocity.setup("velocity", 0.0f, 0.0f, 10.0f));
    };
    virtual ~Joint() {};

    void draw();
    InteractiveObject *getCopy();

    int width;
    int height;

    static ofImage joint_icon_fixed;
    static ofImage joint_icon_r;

    int joint_type;

    // events
    ofEvent<JointClickEventData> click_event;
};

class JointsContainer: public InteractiveContainer {
public:
    JointsContainer() {};
    virtual ~JointsContainer() {};

    void draw();
    void update();
    void update(ofPoint p);
};
