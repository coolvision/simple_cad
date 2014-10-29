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

InteractiveObject *Joint::getCopy() {

    Joint *j = new Joint();
    j->p = this->p;
    j->start_p = this->start_p;
    j->type = this->type;
    j->parent = NULL;

    return j;
}

void Joint::draw() {

    ofPoint icon_offset(13, 13);
    
    if (selected) {
        ofSetColor(ofColor::steelBlue);
    } else if (hover) {
        ofSetColor(ofColor::red);
    } else {
        ofSetColor(ofColor::slateGrey);
    }

    ofPoint p1 = getPx(p);
    if (type == JOINT_FIXED) {
        joint_icon_fixed.draw(p1 - icon_offset);
    } else {
        joint_icon_r.draw(p1 - icon_offset);
    }
}