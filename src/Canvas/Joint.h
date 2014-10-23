//
//  Joint.h
//  simple_cad
//
//  Created by sk on 10/23/14.
//
//

#pragma once

#include "ofMain.h"
#include "ofxMSAInteractiveObject.h"

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

class Joint: public ofxMSAInteractiveObject {
public:

    ofPoint p; // position, in mm

    JointType type;

    Joint();

    // gui
    virtual void onPress(int x, int y, int button);
    virtual void onDragOver(int x, int y, int button);
    virtual void onDragOutside(int x, int y, int button);
    virtual void onRelease(int x, int y, int button);
    virtual void onReleaseOutside(int x, int y, int button);
    void onDragUpdate(int x, int y, int button);
    virtual bool hitTest(int tx, int ty);
    void setDraggable(bool d);
    void onReleaseAny(int x, int y, int button);

    bool hover;
    bool selected;
    bool dragging;
    bool draggable;
    ofPoint drag_start;

    // events
    ofEvent<JointClickEventData> click_event;

    void draw();
};