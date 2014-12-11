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

struct LinkStatus {
    bool hover;
    bool selected;
    bool fixed;
    float angle;
    float velocity;
    ofPoint target;
    bool base;
};

class Joint: public InteractiveObject {
public:
    Joint();
    virtual ~Joint() {
        delete gui;
        delete angle_slider;
        delete velocity;
        delete fixed_toggle;
        delete fixed_angle_toggle;
        gui = NULL;
        angle_slider = NULL;
        velocity = NULL;
        fixed_toggle = NULL;
        fixed_angle_toggle = NULL;
    };

    // interface implementation
    void draw();
    InteractiveObject *getCopy();

    void setupGui();

    // control
    float angle;
    int angle_change_i;

    ofxPanel *gui;
    ofxFloatSlider *angle_slider;
    ofxFloatSlider *velocity;
    ofxToggle *fixed_toggle;
    ofxToggle *fixed_angle_toggle;

    bool fixed;
    bool controlled;
    bool controlled_angle;
    bool fixed_angle;

    bool connected;

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
    vector<LinkStatus> links_status;

    ofPoint f; // force acting on a joint
    float ld; // current length error

    bool discovered;
    int moved_i;

    bool updated;
    bool supported;
    ofPoint s_v;
    float s_l;
    int side;
    float distance;
    ofPoint rotation_target;

    ofPoint intersection;
    bool supporting;
    int s_id[2];
    ofPoint target;

    // tmp
//    float d;
//    float d1;

};