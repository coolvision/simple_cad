//
//  Motion.cpp
//  simple_cad
//
//  Created by sk on 11/13/14.
//
//

#include "Canvas.h"

void Canvas::updateAngles() {

    // joints rotation from UI
    for (int p = 0; p < joints.size(); p++) {
        Joint *j = joints[p];
        if (j == NULL) continue;

        j->fixed = j->fixed_toggle;

        j->angle_slider = j->angle_slider + j->velocity;

        if (j->angle != j->angle_slider) {

            j->controlled = true;

            float diff = (j->angle_slider - j->angle);

            for (int m = 0; m < j->links.size(); m++) {
                Joint *j_m = (Joint *)getItem(ItemId(-1, j->links[m]));
                if (j_m == NULL) continue;
                if (j_m->id == j->id) continue;
                if (j_m->fixed) continue;
                j_m->p.rotate(diff, j->p, ofPoint(0.0f, 0.0f, 1.0f));
                j_m->controlled = true;
            }
            
            j->angle = j->angle_slider;
        }
    }
}

void Canvas::updatePolygons() {

    // update polygons positions
    for (int i = 0; i < lines.size(); i++) {
        Polyline *l = lines[i];

        vector<Joint *> line;
        vector<ofPoint> rel;

        for (int m = 0; m < l->links.size(); m++) {
            Joint *j = (Joint *)getItem(ItemId(-1, l->links[m]));
            if (j == NULL) continue;

            if (line.size() < 2) {
                line.push_back(j);
                rel.push_back(l->links_rel[m]);
            } else {
                break;
            }
        }

        if (line.size() >= 1) {
            Joint *j = line[0];
            if (j->p != (l->p + rel[0])) {
                for (Vertex *v = l->front; v != NULL; v = v->next) {
                    v->p += j->p - (l->p + rel[0]);
                    if (v->next == l->front) break; // closed polylines
                }
                l->update();
            }
        }

        if (line.size() >= 2) {

            // now, rotate
            Joint *j = line[1];

            ofPoint v1 = (rel[1] + l->p) - (rel[0] + l->p);
            ofPoint v2 = line[1]->p - line[0]->p;

            float angle =  atan2(v2.y,v2.x) - atan2(v1.y,v1.x);

            for (Vertex *v = l->front; v != NULL; v = v->next) {
                v->p.rotateRad(angle, line[0]->p, ofPoint(0.0f, 0.0f, 1.0f));
                if (v->next == l->front) break; // closed polylines
            }
            l->update();
            for (int m = 0; m < l->links.size(); m++) {
                l->links_rel[m].rotateRad(angle, ofPoint(), ofPoint(0.0f, 0.0f, 1.0f));
            }
        }
    }
}
