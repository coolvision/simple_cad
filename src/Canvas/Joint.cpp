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

//    gui.setPosition(getPx(p) + ofPoint(20.0f, 20.0f));
//    gui.draw();

    ofPoint icon_offset(13, 13);
    
    if (selected) {
        ofSetColor(ofColor::steelBlue);
    } else if (hover) {
        ofSetColor(ofColor::red);
    } else {
        ofSetColor(ofColor::indigo);
    }

    ofPoint p1 = getPx(p);
    if (joint_type == JOINT_FIXED) {
        joint_icon_fixed.draw(p1 - icon_offset);
    } else {
        joint_icon_r.draw(p1 - icon_offset);
    }
}