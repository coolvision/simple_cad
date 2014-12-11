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
    controlled_angle = false;
    angle_change_i = 0;

    discovered = false;
    moved_i = 0;

    // connecting joints into rigid triangles
    supported = false;
    supporting = false;
    s_id[0] = -1;
    s_id[1] = -1;

    gui = NULL;
    angle_slider = NULL;
    velocity = NULL;
    fixed_toggle = NULL;
    fixed_angle_toggle = NULL;
};

void Joint::setupGui() {

    gui = new ofxPanel();
    angle_slider = new ofxFloatSlider();
    velocity = new ofxFloatSlider();
    fixed_toggle = new ofxToggle();
    fixed_angle_toggle = new ofxToggle();

    gui->setup();
    gui->add(fixed_toggle->setup("fixed", false));
    gui->add(fixed_angle_toggle->setup("fixed_angle", false));
    gui->add(angle_slider->setup("angle", 0.0f, -360.0f, 360.0f));
    gui->add(velocity->setup("velocity", 0.0f, 0.0f, 10.0f));
}

InteractiveObject *Joint::getCopy() {

    Joint *j = new Joint();

    *j = *this;

    j->setupGui();

    *j->fixed_toggle = *fixed_toggle;
    *j->fixed_angle_toggle = *fixed_angle_toggle;
    *j->angle_slider = *angle_slider;
    *j->velocity = *velocity;

    return (InteractiveObject *)j;
}

void Joint::draw() {

    if (joint_type == JOINT_REVOLUTE) {
        if (hover || selected) {
            gui->setPosition(getPx(p) + ofPoint(80.0f, 80.0f));
            gui->draw();
        }
    }

    ofPoint icon_offset(13, 13);

    if (joint_type == JOINT_FIXED) {
        fixed = true;
    }

    if (controlled || controlled_angle) {
        ofSetColor(ofColor::darkRed);
    } else if (selected) {
        ofSetColor(ofColor::steelBlue);
    } else if (hover) {
        ofSetColor(ofColor::red);
    } else if (fixed) {
        ofSetColor(ofColor::darkBlue);
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

    ofDrawBitmapStringHighlight(ofToString(angle), p1);
//    ofDrawBitmapStringHighlight("a"+ofToString(angle_change_i), p1);
//    ofDrawBitmapStringHighlight("m"+ofToString(moved_i), p1 + ofPoint(0.0f, 15.0f));

//    ofSetColor(ofColor::blue);
//    ofSetLineWidth(4.0f);
//    ofLine(getPx(p), getPx(target));
}