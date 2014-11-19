//
//  Update.cpp
//  simple_cad
//
//  Created by sk on 10/31/14.
//
//

#include "Canvas.h"

void Canvas::getConnectedPolygons(int joint_id,
                                vector<InteractiveContainer *> *connected,
                                vector<ofPoint *> *connected_rel) {

    connected->clear();
    connected_rel->clear();
    for (int j = 1; j < lines.size(); j++) {

        for (int i = 0; i < lines[j]->links.size(); i++) {

            if (joint_id == lines[j]->links[i]) {
                connected->push_back(lines[j]);
                connected_rel->push_back(&lines[j]->links_rel[i]);
            }
        }
    }
}

void Canvas::getConnectedJoints(int line_i, vector<Joint *> *connected_j,
                                vector<ofPoint *> *connected_rel) {

//    connected_j->clear();
//    connected_rel->clear();
//    for (Joint *j = (Joint *)lines[0]->front; j != NULL; j = (Joint *)j->next) {
//
//        for (int i = 0; i < j->links.size(); i++) {
//            if (line_i == j->links[i]) {
//                connected_j->push_back(j);
//                connected_rel->push_back(&j->links_rel[i]);
//            }
//        }
//    }
}

void Canvas::update() {

    bool moving = false;

    // joints rotation from UI
    for (Joint *j = (Joint *)lines[0]->front; j != NULL; j = (Joint *)j->next) {

        j->angle_slider = j->angle_slider + j->velocity;

        if (j->angle != j->angle_slider) {

            float diff = (j->angle_slider - j->angle);

            UpdateRelative *m = new UpdateRelative();
            m->receiver_id = j->id;
            m->sent_stamp = update_i;
            m->label = ofToString(diff) + " " + ofToString(update_i);
            j->updated_i = update_i;
            j->angle = j->angle_slider;
            j->motion_msgs.push_back(m);

            moving = true;

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

            while (!l->motion_msgs.empty()) {

                for (int m = 0; m < l->links.size(); m++) {

                    Joint *j = (Joint *)lines[0]->getItem(l->links[m]);

                    if (j->updated_i >= l->motion_msgs.front()->sent_stamp) {
                        continue;
                    }

                    moving = true;

                    l->motion_msgs.front()->apply(j, this);
                    j->motion_msgs.push_back(l->motion_msgs.front()->getCopy());
                    j->motion_msgs.back()->receiver_id = j->id;
                    j->motion_msgs.back()->forward_stamp = update_i;


//                    if (j->fixed) {
//                        //if (l->motion_msgs.front()->total_motion > 0.001) {
//                            update_i++;
//                            FitRelative *m = new FitRelative();
//                            m->receiver_id = j->id;
//                            m->sent_stamp = update_i;
//                            m->forward_stamp = update_i;
//                            m->label = "fit " + ofToString(update_i);
//                            j->updated_i = update_i;
//                            j->motion_msgs.push_back(m);
//                        //}
//                    }

                }

                delete l->motion_msgs.front();
                l->motion_msgs.pop_front();
            }
        }

    } else {

        // and from the joints to the polygons
        for (Joint *j = (Joint *)lines[0]->front; j != NULL; j = (Joint *)j->next) {

            vector <InteractiveContainer *> connected;
            vector<ofPoint *> connected_rel;
            getConnectedPolygons(j->id, &connected, &connected_rel);

            while (!j->motion_msgs.empty()) {

                for (int i = 0; i < connected.size(); i++) {

                    Polyline *l = (Polyline *)connected[i];

                    if (l->updated_i >= j->motion_msgs.front()->sent_stamp) {
                        continue;
                    }
                    moving = true;

                    j->motion_msgs.front()->apply(l, this);
                    l->motion_msgs.push_back(j->motion_msgs.front()->getCopy());
                    l->motion_msgs.back()->receiver_id = l->id;
                    l->motion_msgs.back()->forward_stamp = update_i;
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

            if (j->connected) {
                continue;
            }

            bool connected = false;
            for (int i = 1; i < lines.size(); i++) {

                InteractiveContainer *l = lines[i];

                if (l->inside(getPx(j->p))) {
                    bool exists = false;

                    if (l->fixed) {
                        j->fixed = true;
                    }

                    for (int m = 0; m < l->links.size(); m++) {
                        if (l->links[m] == j->id) {
                            exists = true;
                            break;
                        }
                    }
                    if (!exists) {
                        l->links.push_back(j->id);
                        l->links_rel.push_back(j->p - l->p);
                        lines[i]->reset();

                        connected = true;

                        if (j->joint_type == JOINT_FIXED) {
                            l->fixed = true;
                        }
                    }
                }
            }
            if (connected) {
                j->connected = true;
            }


        }
    }

}