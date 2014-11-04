//
//  Update.cpp
//  simple_cad
//
//  Created by sk on 10/31/14.
//
//

#include "Canvas.h"

void Canvas::update() {

    // use the list of the joints
    for (Joint *j = (Joint *)lines[0]->front; j != NULL; j = (Joint *)j->next) {

        j->links.clear();

        for (int i = 1; i < lines.size(); i++) {
            if (lines[i]->inside(getPx(j->p))) {
                j->links.push_back(i);
            }
        }
        if (j->next == (Joint *)lines[0]->front) break; // closed polylines
    }

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
        }
        if (j->next == (Joint *)lines[0]->front) break; // closed polylines
    }
}