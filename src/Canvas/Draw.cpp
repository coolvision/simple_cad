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
    for (Joint *j = (Joint *)lines[0]->front; j != NULL; j = (Joint *)j->next) {

        ofPoint p1 = getPx(j->p);

        for (int i = 0; i < j->links.size(); i++) {

            ofPoint p2 = getPx(lines[j->links[i]]->p);
            ofLine(p1, p2);

            float point_size = 3.0f * zoom;
            if (point_size < 1.0f) {
                point_size = 1.0f;
            }
            ofCircle(p1, point_size);
            ofCircle(p2, point_size);
        }
        if (j->next == (Joint *)lines[0]->front) break; // closed polylines
    }

}