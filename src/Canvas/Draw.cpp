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

//    for (int l = 0; l < lines.size(); l++) {
//
//        ofPoint p1 = getPx(lines[l]->p);
//        ofPoint p2;
//
//        for (int i = 0; i < lines[l]->links.size(); i++) {
//
//            Joint *j = (Joint *)getItem(ItemId(-1, lines[l]->links[i]));
//            if (j == NULL) continue;
//
////            ofSetColor(ofColor::indigo);
////            ofPoint p2 = getPx(j->p);
////            ofLine(p1, p2);
//
//            ofSetColor(ofColor::darkSlateBlue);
//            p2 = getPx(lines[l]->p + lines[l]->links_rel[i]);
//            ofLine(p1, p2);
//
//            float point_size = 3.0f * zoom;
//            if (point_size < 1.0f) {
//                point_size = 1.0f;
//            }
//            ofCircle(p1, point_size);
//            ofCircle(p2, point_size);
//        }
//    }

    float point_size = 3.0f * zoom;
    if (point_size < 1.0f) {
        point_size = 1.0f;
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
        //else {

            for (int n = 0; n < j_m->links.size(); n++) {
                Joint *j_n = (Joint *)getItem(ItemId(-1, j_m->links[n]));
                if (j_n == NULL) continue;

                if (j_n->supported) continue;

                ofSetColor(ofColor::black);
                ofLine(getPx(j_m->p), getPx(j_n->p));
            }
       // }

    }
}