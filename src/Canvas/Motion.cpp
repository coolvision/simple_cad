//
//  Motion.cpp
//  simple_cad
//
//  Created by sk on 11/13/14.
//
//

#include "Canvas.h"

void Canvas::getLinks(Joint *j, vector<Joint *> *links, vector<float> *length) {

    for (int n = 0; n < j->links.size(); n++) {
        Joint *j_n = (Joint *)getItem(ItemId(-1, j->links[n]));
        if (j_n == NULL) continue;
        links->push_back(j_n);
        if (length != NULL) {
            length->push_back(j->links_length[n]);
        }
    }
}

void Canvas::updateAngles() {

    for (int m = 0; m < joints.size(); m++) {
        Joint *j = joints[m];
        if (j == NULL) continue;

        if (j->fixed_angle && !*j->fixed_angle_toggle) {
            j->controlled_angle = false;
        }

        j->fixed = *j->fixed_toggle;
        j->fixed_angle = *j->fixed_angle_toggle;

        if (j->fixed_angle) {
            j->controlled_angle = true;
            j->angle = *j->angle_slider;
        }
    }

    for (int m = 0; m < joints.size(); m++) {
        Joint *j_m = joints[m];
        if (j_m == NULL) continue;
        if (j_m->controlled_angle) continue;

        vector<Joint *> links;
        getLinks(j_m, &links);

        if (links.size() == 1) {

            ofPoint v1 = links[0]->p - j_m->p;

            float angle = RAD_TO_DEG * atan2(v1.y, v1.x);
            j_m->angle = angle;
            *j_m->angle_slider = angle;

        } else if (links.size() >= 2) {

            ofPoint v1 = links[0]->p - j_m->p;
            ofPoint v2 = links[1]->p - j_m->p;

            float angle = RAD_TO_DEG * (atan2(v1.y, v1.x) - atan2(v2.y, v2.x));
            j_m->angle = angle;
            *j_m->angle_slider = angle;
        }
    }

    // joints rotation from UI
    for (int p = 0; p < joints.size(); p++) {
        Joint *j = joints[p];
        if (j == NULL) continue;

        if ((update_i - j->moved_i) < 3) {
            continue;
        }

        if (*j->velocity > FLT_EPSILON) {
            j->controlled_angle = true;
            j->angle += *j->velocity;
            j->angle_change_i = update_i;
        } else {
            if (j->angle_change_i != 0 && update_i - j->angle_change_i > 3) {
                j->angle_change_i = 0;
                j->controlled_angle = false;
            }
        }
    }
}

void Canvas::updatePolygons() {

    // update polygons positions
    for (int i = 0; i < lines.size(); i++) {
        Polyline *l = lines[i];

        bool modified = false;
        for (Vertex *v = l->front; v != NULL; v = v->next) {
            if (v->dragged) {
                modified = true;
                break;
            }
            if (v->next == l->front) break; // closed polylines
        }
        if (modified) continue;

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
