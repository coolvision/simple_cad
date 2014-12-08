//
//  Joint.cpp
//  simple_cad
//
//  Created by sk on 10/23/14.
//
//

#include "Joint.h"
#include "Canvas.h"

ofImage Joint::joint_icon_fixed;
ofImage Joint::joint_icon_r;

Joint::Joint() {
    grid_snap = false;

    angle = 0.0f;
    fixed = false;
    connected = false;
    controlled = false;

    // connecting joints into rigid triangles
    supported = false;
    supporting = false;
    s_id[0] = -1;
    s_id[1] = -1;
    d = 0;

    gui.setup();
    gui.add(angle_slider.setup("angle", 0.0f, -360.0f, 360.0f));
    gui.add(velocity.setup("velocity", 0.0f, 0.0f, 10.0f));
    gui.add(fixed_toggle.setup("fixed", false));
};

InteractiveObject *Joint::getCopy() {

    Joint *j = new Joint();
    j->p = this->p;
    j->start_p = this->start_p;
    j->joint_type = this->joint_type;
    j->id = this->id;
    
    return (InteractiveObject *)j;
}

void Joint::draw() {

    if (joint_type == JOINT_REVOLUTE) {
        if (hover || selected) {
            gui.setPosition(getPx(p) + ofPoint(80.0f, 80.0f));
            gui.draw();
        }
    }

    ofPoint icon_offset(13, 13);

    if (joint_type == JOINT_FIXED) {
        fixed = true;
    }

    if (controlled || fixed) {
        ofSetColor(ofColor::darkRed);
    } else if (selected) {
        ofSetColor(ofColor::steelBlue);
    } else if (hover) {
        ofSetColor(ofColor::red);
    } else {
        ofSetColor(ofColor::black);
    }

    ofPoint p1 = getPx(p);
    if (joint_type == JOINT_FIXED) {
        joint_icon_fixed.draw(p1 - icon_offset);
    } else {
        joint_icon_r.draw(p1 - icon_offset);
    }

    ofSetColor(ofColor::red);
    ofSetLineWidth(4.0f);
    ofLine(getPx(p), getPx(p+f));

//    ofSetColor(ofColor::blue);
//    ofSetLineWidth(4.0f);
//    ofLine(getPx(p), getPx(target));
}