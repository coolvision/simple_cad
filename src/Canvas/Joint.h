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
    Joint();
    virtual ~Joint() {};

    // interface implementation
    void draw();
    InteractiveObject *getCopy();

    // control
    float angle;
    ofxFloatSlider angle_slider;
    ofxFloatSlider velocity;
    ofxToggle fixed_toggle;

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

    // connected joints
    vector<int> links;
    vector<float> links_length;
    ofPoint f; // force acting on a joint
    float ld; // current length error

    bool updated;
    bool supported;
    ofPoint s_v;
    float s_l;
    int side;
    float distance;
    //ofPoint target;

    ofPoint intersection;
    bool supporting;
    int s_id[2];


    // tmp
    float d;
    float d1;
    ofPoint target;
};