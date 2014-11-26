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

//bool Canvas::updateMessages(bool reverse) {
//
//    bool moving = false;
//
//    static int i = 0;
//    i++;
//
//    if (i % 2 == 0) {
//
//        // propagate motion to the joints
//        for (int i = 1; i < lines.size(); i++) {
//
//            Polyline *l = (Polyline *)lines[i];
//            if (l->motion_msgs.empty()) {
//                continue;
//            }
//
//            while (!l->motion_msgs.empty()) {
//
//                Motion *mm = l->motion_msgs.front();
//
//                for (int m = 0; m < l->links.size(); m++) {
//
//                    Joint *j = (Joint *)lines[0]->getItem(l->links[m]);
//
//                    if (j->updated[mm->type_i] >= mm->sent_stamp) {
//                        continue;
//                    }
//
//                    moving = true;
//
//                    l->motion_msgs.front()->apply(j, this);
//                    j->motion_msgs.push_back(mm->getCopy());
//                    j->motion_msgs.back()->receiver_id = j->id;
//                    j->motion_msgs.back()->forward_stamp = update_i;
//
//
//                    if (j->fixed) {
//                        if (l->motion_msgs.front()->type_i != UPDATE_RELATIVE) {
//                            //if (l->motion_msgs.front()->total_motion > 0.01) {
//                                update_i++;
//                                FitRelative *m = new FitRelative();
//                                m->receiver_id = j->id;
//                                m->sent_stamp = update_i;
//                                m->forward_stamp = update_i;
//                                m->label = "fit " + ofToString(update_i);
//                                m->type_i = FIT_RELATIVE;
//                                j->updated[m->type_i] = update_i;
//                                j->motion_msgs.push_back(m);
//
//                                // now, need to "fix" the joint here
//                                // find the fixed polygon
//                                vector <InteractiveContainer *> connected;
//                                vector<ofPoint *> connected_rel;
//                                getConnectedPolygons(j->id, &connected, &connected_rel);
//
//                                for (int q = 0; q < connected.size(); q++) {
//                                    if (connected[q]->fixed) {
//                                        ofPoint new_p = (connected[q]->p + *connected_rel[q]);
//                                        j->motion_msgs.back()->total_motion +=
//                                                    (new_p - j->p).length();
//                                        j->p = new_p;
//                                    }
//                                }
//
//                            //}
//                        }
//                    }
//
//                }
//
//                delete mm;
//                l->motion_msgs.pop_front();
//            }
//        }
//
//    } else {
//
//        // and from the joints to the polygons
//        for (Joint *j = (Joint *)lines[0]->front; j != NULL; j = (Joint *)j->next) {
//
//            vector <InteractiveContainer *> connected;
//            vector<ofPoint *> connected_rel;
//            getConnectedPolygons(j->id, &connected, &connected_rel);
//
//            while (!j->motion_msgs.empty()) {
//
//                Motion *mm = j->motion_msgs.front();
//
//                for (int i = 0; i < connected.size(); i++) {
//
//                    Polyline *l = (Polyline *)connected[i];
//
//                    if (mm->type_i == ROTATION && l->fixed) {
//                        continue;
//                    }
//                    if (mm->type_i == FIT_RELATIVE && l->fixed) {
//                        continue;
//                    }
//
//                    if (l->updated[mm->type_i] >= mm->sent_stamp) {
//                        continue;
//                    }
//                    moving = true;
//                    
//                    mm->apply(l, this);
//                    l->motion_msgs.push_back(mm->getCopy());
//                    l->motion_msgs.back()->receiver_id = l->id;
//                    l->motion_msgs.back()->forward_stamp = update_i;
//                }
//                delete mm;
//                j->motion_msgs.pop_front();
//            }
//        }
//    }
//    
//    for (int i = 1; i < lines.size(); i++) {
//        lines[i]->update();
//    }
//    
//    update_i++;
//
//    return moving;
//}

void Canvas::update(bool moving) {

    // joints rotation from UI
    for (Joint *j = (Joint *)lines[0]->front; j != NULL; j = (Joint *)j->next) {

        j->angle_slider = j->angle_slider + j->velocity;

//        if (j->angle != j->angle_slider) {
//
//            j->controlled = true;
//
//            float diff = (j->angle_slider - j->angle);
//
//            Rotation *m = new Rotation();
//            m->receiver_id = j->id;
//            update_i++;
//            m->sent_stamp = update_i;
//            m->label = ofToString(diff) + " " + ofToString(update_i);
//            m->type_i = ROTATION;
//            j->angle = j->angle_slider;
//            j->motion_msgs.push_back(m);
//            j->updated[m->type_i] = update_i;
//            moving = true;
//
//            update_i++;
//        }

        if (j->next == (Joint *)lines[0]->front) break; // closed polylines
    }

    for (int i = 1; i < lines.size(); i++) {
        InteractiveContainer *l = lines[i];
        for (int m = 0; m < l->links.size(); m++) {
            Joint *j = (Joint *)lines[0]->getItem(l->links[m]);
            if (j->controlled) {
                l->controlled = true;
            }
        }
    }

    // connect joints to the polygons
    // if they are intersecting
    if (!moving) {
        for (Joint *j = (Joint *)lines[0]->front; j != NULL; j = (Joint *)j->next) {

            bool connected = false;
            for (int i = 1; i < lines.size(); i++) {

                InteractiveContainer *l = lines[i];

                if (l->inside(getPx(j->p))) {
                    bool exists = false;

                    if (l->fixed) {
                        j->fixed = true;
                    }

                    if (j->connected) {
                        continue;
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
                        //lines[i]->reset();

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