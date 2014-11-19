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

void JointsContainer::draw() {

    for (InteractiveObject *v = front; v != NULL; v = v->next) {
        v->draw();
        if (v->next == front) break; // closed polylines
    }
}

void JointsContainer::update(ofPoint p) {

    update();
}

void JointsContainer::update() {

    updateIndexes();
}

InteractiveObject *Joint::getCopy() {

    Joint *j = new Joint();
    j->p = this->p;
    j->start_p = this->start_p;
    j->joint_type = this->joint_type;
    j->parent = NULL;

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

    if (selected) {
        ofSetColor(ofColor::steelBlue);
    } else if (hover) {
        ofSetColor(ofColor::red);
    } else {
        if (fixed) {
            ofSetColor(ofColor::darkRed);
        } else {
            ofSetColor(ofColor::black);
        }
    }

    ofPoint p1 = getPx(p);
    if (joint_type == JOINT_FIXED) {
        joint_icon_fixed.draw(p1 - icon_offset);
    } else {
        joint_icon_r.draw(p1 - icon_offset);
    }

    ofSetColor(ofColor::black);
    ofDrawBitmapString(label + " " + ofToString(updated_i) + " fa" + ofToString(fit_angle), getPx(p) + ofPoint(0, 15 * 0));
    for (int i = 0; i < motion_msgs.size(); i++) {
        ofDrawBitmapString(motion_msgs[i]->label + " " + ofToString(motion_msgs[i]->forward_stamp) + "m" + ofToString(motion_msgs[i]->total_motion), getPx(p) + ofPoint(0, 15 * (i+1)));
    }
    ofDrawBitmapStringHighlight(ofToString(angle), getPx(p) + ofPoint(20, -20));

    ofSetLineWidth(1.0f);
    ofSetColor(ofColor::green);
    ofLine(getPx(p), getPx(p + v1));
    ofSetLineWidth(4.0f);
    ofSetColor(ofColor::blue);
    ofLine(getPx(p), getPx(p + v2));
}