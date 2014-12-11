//
//  Draw.cpp
//  simple_cad
//
//  Created by sk on 11/3/14.
//
//

#include "Canvas.h"

void Canvas::draw() {

    ofSetColor(ofColor::black);
    ofSetLineWidth(1.0f);
    ofEnableAntiAliasing();
    ofEnableSmoothing();
    ofSetColor(ofColor::indigo);

    float point_size = 3.0f * zoom;
    if (point_size < 1.0f) {
        point_size = 1.0f;
    }
    if (point_size > 5.0f) {
        point_size = 5.0f;
    }
    ofSetColor(ofColor::black);
    for (int m = 0; m < joints.size(); m++) {
        Joint *j_m = joints[m];
        if (j_m == NULL) continue;

        if (j_m->supported) {
            ofSetColor(120);
            ofLine(j_m->getPx(j_m->p), j_m->getPx(j_m->intersection));
            ofCircle(j_m->getPx(j_m->intersection), point_size);

            ofSetColor(ofColor::red);
            ofCircle(j_m->getPx(j_m->target), point_size);
        }

        bool first = false;
        Joint *base_link = NULL;
        ofPoint base_n;
        for (int n = 0; n < j_m->links.size(); n++) {
            Joint *j_n = (Joint *)getItem(ItemId(-1, j_m->links[n]));
            if (j_n == NULL) continue;

            ofPoint mid = j_m->p + (j_n->p - j_m->p) * 0.5f;

            if (j_m->links_status[n].hover) {
                ofSetColor(ofColor::red);
            } else {
//                if (!first) {
//                    ofSetColor(ofColor::gray);
//                } else {
                    ofSetColor(ofColor::black);
//                }
                ofCircle(j_m->getPx(mid), point_size);
            }
            ofLine(getPx(j_m->p), getPx(mid));

            // draw the angle of this link
//            ofPoint curr_n = j_n->p - j_m->p;
//            float angle = j_m->links_status[n].angle;
//            ofDrawBitmapStringHighlight(ofToString(angle),
//                                        j_m->getPx((mid + j_m->p)/2));


            ofSetColor(ofColor::darkSlateGray);
            ofLine(getPx(j_m->p), getPx(j_m->rotation_target));
            ofCircle(getPx(j_m->rotation_target), point_size);

            if (!first) {
                first = true;
            }
        }

        for (int n = 0; n < j_m->links.size(); n++) {
            Joint *j_n = (Joint *)getItem(ItemId(-1, j_m->links[n]));
            if (j_n == NULL) continue;

            ofPoint mid = j_m->p + (j_n->p - j_m->p) * 0.5f;
            if (j_m->links_status[n].hover) {
                ofSetColor(ofColor::red);
            }
        }
    }
}