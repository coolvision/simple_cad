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

        angle = 0.0f;
        fixed = false;
        connected = false;
        fit_angle = 0.0f;
        controlled = false;

        gui.setup();
        gui.add(angle_slider.setup("angle", 0.0f, -360.0f, 360.0f));
        gui.add(velocity.setup("velocity", 0.0f, 0.0f, 10.0f));
    };
    virtual ~Joint() {};

    // interface implementation
    void draw();
    InteractiveObject *getCopy();

    // control
    float angle;
    ofxFloatSlider angle_slider;
    ofxFloatSlider velocity;
    ofxPanel gui;
    bool fixed;
    bool connected;
    bool controlled;

    // display
    int width;
    int height;
    static ofImage joint_icon_fixed;
    static ofImage joint_icon_r;
    int joint_type;

    // events
    ofEvent<JointClickEventData> click_event;
};