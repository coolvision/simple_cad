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


    for (Joint *j = (Joint *)lines[0]->front; j != NULL; j = (Joint *)j->next) {

        j->angle = j->angle + j->velocity;

        if (j->curr_angle != j->angle) {

            float diff = (j->angle - j->curr_angle);
            for (int i = 0; i < j->links.size(); i++) {

                Polyline *l = (Polyline *)lines[j->links[i]];

                for (Vertex *v = (Vertex *)l->front; v != NULL;
                     v = (Vertex *)v->next) {

                    v->p.rotate(diff, j->p, ofPoint(0.0f, 0.0f, 1.0f));

                    if (v->next == l->front) break; // closed polylines
                }
                l->update();

            }
            j->curr_angle = j->angle;
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