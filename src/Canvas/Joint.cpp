//
//  Joint.cpp
//  simple_cad
//
//  Created by sk on 10/23/14.
//
//

#include "Joint.h"
#include "Canvas.h"

ofImage JointContainer::joint_icon_fixed;
ofImage JointContainer::joint_icon_r;

void JointContainer::draw() {

    ofPoint icon_offset(13, 13);
    
    if (joint->hover) {
        ofSetColor(ofColor::orangeRed);
    } else {
        ofSetColor(ofColor::red);
    }

    ofPoint p = joint->getPx(joint->p);
    if (joint->type == JOINT_FIXED) {
        joint_icon_fixed.draw(p - icon_offset);
    } else {
        joint_icon_r.draw(p - icon_offset);
    }
}