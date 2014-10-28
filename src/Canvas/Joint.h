//
//  Joint.h
//  simple_cad
//
//  Created by sk on 10/23/14.
//
//

#pragma once

#include "ofMain.h"

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

class Canvas;

class Joint {
public:
    Joint();

    ofPoint p; // position, in mm
    int width;
    int height;

    JointType type;

    bool hover;
    bool selected;
    bool dragging;
    ofPoint start_p;

    // events
    ofEvent<JointClickEventData> click_event;
};