//
//  Update.cpp
//  simple_cad
//
//  Created by sk on 10/31/14.
//
//

#include "Canvas.h"

void Canvas::getConnectedJoints(int line_i, vector<Joint *> *connected_j,
                                vector<ofPoint> *connected_rel) {

    connected_j->clear();
    connected_rel->clear();
    for (Joint *j = (Joint *)lines[0]->front; j != NULL; j = (Joint *)j->next) {

        for (int i = 0; i < j->links.size(); i++) {
            if (line_i == j->links[i]) {
                connected_j->push_back(j);
                connected_rel->push_back(j->links_rel[i]);
            }
        }
    }
}

void Canvas::update() {

    // connect joints to the polygons
    // if they are intersecting
    for (Joint *j = (Joint *)lines[0]->front; j != NULL; j = (Joint *)j->next) {

        for (int i = 1; i < lines.size(); i++) {
            if (lines[i]->inside(getPx(j->p))) {
                bool exists = false;
                for (int m = 0; m < j->links.size(); m++) {
                    if (j->links[m] == i) {
                        exists = true;
                        break;
                    }
                }
                if (!exists) {
                    j->links.push_back(i);
                    j->links_rel.push_back(j->p - lines[i]->p);
                    lines[i]->reset();
                }
            }
        }
    }

    static int i = 0;
    i++;

    // propagate motion to the joints
    vector <Joint *> connected_j;
    vector<ofPoint> connected_rel;
    for (int i = 1; i < lines.size(); i++) {
        Polyline *l = (Polyline *)lines[i];
        if (l->motion_msgs.empty()) {
            continue;
        }
        // get the list of connected joints
        getConnectedJoints(i, &connected_j, &connected_rel);

        while (!l->motion_msgs.empty()) {
            for (int j = 0; j < connected_j.size(); j++) {
                if (connected_j[j]->id != l->motion_msgs.front()->sender_id) {
                    if (connected_j[j]->updated_i > l->motion_msgs.front()->sent_stamp) {
                        continue;
                    }
                    l->motion_msgs.front()->apply(connected_j[j], this);
                    connected_j[j]->motion_msgs.push_back(l->motion_msgs.front()->getCopy());
                    connected_j[j]->motion_msgs.back()->sender_id = l->id;
                    connected_j[j]->motion_msgs.back()->receiver_id = connected_j[j]->id;
                }
            }
            delete l->motion_msgs.front();
            l->motion_msgs.pop_front();
        }
    }

    // and from the joints to the polygons
    for (Joint *j = (Joint *)lines[0]->front; j != NULL; j = (Joint *)j->next) {

        while (!j->motion_msgs.empty()) {

            for (int i = 0; i < j->links.size(); i++) {
                if (j->links[i] != j->motion_msgs.front()->sender_id &&
                    j->links[i] != j->motion_msgs.front()->origin_id) {
                    Polyline *l = (Polyline *)lines[j->links[i]];

                    if (l->updated_i > j->motion_msgs.front()->sent_stamp) {
                        continue;
                    }

                    j->motion_msgs.front()->apply(l, this);

                    l->motion_msgs.push_back(j->motion_msgs.front()->getCopy());
                    l->motion_msgs.back()->sender_id = j->id;
                    l->motion_msgs.back()->receiver_id = l->id;
                }
            }
            delete j->motion_msgs.front();
            j->motion_msgs.pop_front();
        }
    }


    for (int i = 1; i < lines.size(); i++) {
        lines[i]->update();
    }

    // joints rotation from UI
    for (Joint *j = (Joint *)lines[0]->front; j != NULL; j = (Joint *)j->next) {

        j->angle = j->angle + j->velocity;

        if (j->curr_angle != j->angle) {

            float diff = (j->angle - j->curr_angle);

            Rotation *m = new Rotation();
            m->sender_id = -1;
            m->receiver_id = j->id;
            m->origin_id = j->id;
            m->sent_stamp = update_i;
            m->label = ofToString(diff) + " " + ofToString(update_i);
            m->pivot = j->p;
            m->angle = diff;
            j->updated_i = update_i;
            j->motion_msgs.push_back(m);

            j->motion_msgs.back()->apply(j, this);

//            for (int i = 0; i < j->links.size(); i++) {
//
//                Polyline *l = (Polyline *)lines[j->links[i]];
//
//                for (Vertex *v = (Vertex *)l->front; v != NULL;
//                    v = (Vertex *)v->next) {
//
//                    v->p.rotate(diff, j->p, ofPoint(0.0f, 0.0f, 1.0f));
//
//                    if (v->next == l->front) break; // closed polylines
//                }
//                l->update();
//            }

            j->curr_angle = j->angle;
        }

        if (j->next == (Joint *)lines[0]->front) break; // closed polylines
    }

    update_i++;
}