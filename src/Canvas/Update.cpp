//
//  Update.cpp
//  simple_cad
//
//  Created by sk on 10/31/14.
//
//

#include "Canvas.h"

void Canvas::getConnectedJoints(int line_i, vector<Joint *> *connected_j,
                                vector<ofPoint *> *connected_rel) {

    connected_j->clear();
    connected_rel->clear();
    for (Joint *j = (Joint *)lines[0]->front; j != NULL; j = (Joint *)j->next) {

        for (int i = 0; i < j->links.size(); i++) {
            if (line_i == j->links[i]) {
                connected_j->push_back(j);
                connected_rel->push_back(&j->links_rel[i]);
            }
        }
    }
}

void Canvas::update() {

    bool moving = false;

    // joints rotation from UI
    for (Joint *j = (Joint *)lines[0]->front; j != NULL; j = (Joint *)j->next) {

        j->angle_slider = j->angle_slider + j->velocity;

        if (j->angle != j->angle_slider) {

            float diff = (j->angle_slider - j->angle);

            UpdateRelative *m = new UpdateRelative();
            m->sender_id = -1;
            m->receiver_id = j->id;
            m->origin_id = j->id;
            m->sent_stamp = update_i;
            m->sender_type = 1; // a hack, should not use a magic number
            m->label = ofToString(diff) + " " + ofToString(update_i);
            j->updated_i = update_i;
            j->angle = j->angle_slider;
            j->motion_msgs.push_back(m);

            moving = true;

//            for (int i = 0; i < j->links_rel.size(); i++) {
//                if (!lines[j->links[i]]->fixed) {
//                    ofPoint l = j->p - j->links_rel[i];
//                    l.rotate(diff, j->p, ofPoint(0.0f, 0.0f, 1.0f));
//                    j->links_rel[i] = j->p - l;
//                }
//            }

            update_i++;
        }

        if (j->next == (Joint *)lines[0]->front) break; // closed polylines
    }

    static int i = 0;
    i++;

    if (i % 2 == 0) {

        // propagate motion to the joints
        vector <Joint *> connected_j;
        vector<ofPoint *> connected_rel;
        for (int i = 1; i < lines.size(); i++) {
            Polyline *l = (Polyline *)lines[i];
            if (l->motion_msgs.empty()) {
                continue;
            }
            // get the list of connected joints
            getConnectedJoints(i, &connected_j, &connected_rel);

            while (!l->motion_msgs.empty()) {
                for (int m = 0; m < connected_j.size(); m++) {
                    Joint *j = connected_j[m];

                    if (j->updated_i >= l->motion_msgs.front()->sent_stamp) {
                        continue;
                    }

                    l->motion_msgs.front()->apply(j, this);
                    j->motion_msgs.push_back(l->motion_msgs.front()->getCopy());
                    j->motion_msgs.back()->sender_id = l->id;
                    j->motion_msgs.back()->receiver_id = j->id;
                    j->motion_msgs.back()->forward_stamp = update_i;

                    moving = true;
                    if (j->fixed) {
                        update_i++;
                        j->motion_msgs.back()->sent_stamp = update_i;
                        j->updated_i = update_i;
                    }
                }

                delete l->motion_msgs.front();
                l->motion_msgs.pop_front();
            }
        }

    } else {

        // and from the joints to the polygons
        for (Joint *j = (Joint *)lines[0]->front; j != NULL; j = (Joint *)j->next) {

            while (!j->motion_msgs.empty()) {

                for (int i = 0; i < j->links.size(); i++) {

                    Polyline *l = (Polyline *)lines[j->links[i]];

                    if (l->updated_i >= j->motion_msgs.front()->sent_stamp) {
                        continue;
                    }
                    moving = true;

                    if (!l->fixed) {
                        j->motion_msgs.front()->apply(l, this);
                        l->motion_msgs.push_back(j->motion_msgs.front()->getCopy());
                        l->motion_msgs.back()->sender_id = j->id;
                        l->motion_msgs.back()->receiver_id = l->id;
                        l->motion_msgs.back()->forward_stamp = update_i;
                    }
                }
                delete j->motion_msgs.front();
                j->motion_msgs.pop_front();
            }
        }
    }

    for (int i = 1; i < lines.size(); i++) {
        lines[i]->update();
    }

    update_i++;


    // connect joints to the polygons
    // if they are intersecting
    if (!moving) {
        for (Joint *j = (Joint *)lines[0]->front; j != NULL; j = (Joint *)j->next) {

            for (int i = 1; i < lines.size(); i++) {

                if (lines[i]->inside(getPx(j->p))) {
                    bool exists = false;

                    if (lines[i]->fixed) {
                        j->fixed = true;
                    }

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

                        if (j->joint_type == JOINT_FIXED) {
                            lines[i]->fixed = true;
                        }

                    }
                }
            }
        }
    }

}