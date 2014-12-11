//
//  Update.cpp
//  simple_cad
//
//  Created by sk on 10/31/14.
//
//

#include "Canvas.h"

int sign(float val) {
    return (0.0f < val) - (val < 0.0f);
}

void Canvas::updateDistances() {

    for (int p = 0; p < joints.size(); p++) {
        Joint *j = joints[p];
        if (j == NULL) continue;
        j->supported = false;
        j->supporting = false;
        j->intersection = j->p;
    }

    for (int p = 0; p < joints.size(); p++) {
        Joint *j = joints[p];
        if (j == NULL) continue;

        if (j->supported) continue;

        // look at all pairs of the connected joints
        bool found = false;
        for (int m = 0; m < j->links.size(); m++) {
            Joint *j_m = (Joint *)getItem(ItemId(-1, j->links[m]));

            for (int n = 0; n < j->links.size(); n++) {
                Joint *j_n = (Joint *)getItem(ItemId(-1, j->links[n]));

                if (j_m == NULL || j_n == NULL) continue;
                if (j_m->id == j_n->id) continue;

                bool connected = false;
                for (int q = 0; q < j_m->links.size(); q++) {
                    if (j_m->links[q] == j_n->id) {
                        connected = true;
                        break;
                    }
                }
                if (connected) {
                    ofPoint p0 = j_m->p;
                    ofPoint p1 = j_n->p;
                    ofPoint p = j->p;

                    float d = ((p0.y - p1.y) * p.x +
                               (p1.x - p0.x) * p.y +
                               (p0.x * p1.y - p1.x * p0.y)) /
                    (p1 - p0).length();

                    float u = ((p.x - p0.x) * (p1.x - p0.x) + (p.y - p0.y) * (p1.y - p0.y)) /
                            (p1 - p0).lengthSquared();

                    if(u < 0.0f || u > 1.0f) continue;

                    // mark this joints as supported
                    j->supported = true;
                    j_m->supporting = true;
                    j_n->supporting = true;
                    j->s_id[0] = j_m->id;
                    j->s_id[1] = j_n->id;

                    j->links[n] = -1;
                    j->links[m] = -1;

                    //j->d = d;
                    j->intersection.x = p0.x + u * (p1.x - p0.x);
                    j->intersection.y = p0.y + u * (p1.y - p0.y);

                    j->s_v = (j->intersection - p0).normalized();
                    j->s_l = (j->intersection - p0).length();
                    j->side = sign((p1.x - p0.x) * (p.y - p0.y) -
                                   (p1.y - p0.y) * (p.x - p0.x));
                    j->distance = (j->intersection - j->p).length();

                    found = true;
                    break;
                }
            }
            if (found) break;
        }
    }
}

void Canvas::updateSupported() {

    for (int p = 0; p < joints.size(); p++) {
        Joint *j = joints[p];
        if (j == NULL) continue;

        if (j->supported) {
            Joint *j_m = (Joint *)getItem(ItemId(-1, j->s_id[0]));
            Joint *j_n = (Joint *)getItem(ItemId(-1, j->s_id[1]));
            if (j_m == NULL || j_n == NULL) continue;
            j->intersection = j_m->p + (j_n->p - j_m->p).normalized() * j->s_l;

            j->target = (j_n->p - j->intersection).normalized() * j->distance;
            if (j->side > 0) {
                j->target.rotate(90, ofPoint(), ofPoint(0.0f, 0.0f, 1.0f));
            } else {
                j->target.rotate(-90, ofPoint(), ofPoint(0.0f, 0.0f, 1.0f));
            }
            j->target += j->intersection;
        }
    }
}

void Canvas::updateRotationTargets() {

    for (int p = 0; p < joints.size(); p++) {
        Joint *j = joints[p];
        if (j == NULL) continue;
        j->rotation_target = j->p;
    }

    for (int m = 0; m < joints.size(); m++) {
        Joint *j_m = joints[m];
        if (j_m == NULL) continue;
        if (!j_m->controlled_angle) continue;

        vector<Joint *> links;
        vector<float> length;

        getLinks(j_m, &links, &length);
        ofPoint curr_n;

        if (links.size() == 1) {

            curr_n = ofPoint(1.0f, 0.0f, 0.0f);
            curr_n *= length[0];
            curr_n.rotate(j_m->angle, ofPoint(), ofPoint(0.0f, 0.0f, 1.0f));
            links[0]->rotation_target = j_m->p + curr_n;

        } else if (links.size() >= 2) {

            curr_n = (links[0]->p - j_m->p).normalized();
            curr_n *= length[1];
            curr_n.rotate(-j_m->angle, ofPoint(), ofPoint(0.0f, 0.0f, 1.0f));
            links[1]->rotation_target = j_m->p + curr_n;

            curr_n = (links[1]->p - j_m->p).normalized();
            curr_n *= length[0];
            curr_n.rotate(j_m->angle, ofPoint(), ofPoint(0.0f, 0.0f, 1.0f));
            links[0]->rotation_target = j_m->p + curr_n;
        }
    }
}

void Canvas::update(bool update_angles) {

    float total_f = 1.0f;
    float total_ld = 0.0f;

    vector<Joint *> line;

    static int iter = 0;

    updateSupported();
    updateRotationTargets();

    int n_iter = 0;
    while (total_f > 0.001f) {

        if (n_iter % 10 == 0) {
            updateSupported();
        }
        updateRotationTargets();

        if (n_iter >= 100) {
            break;
        }
        n_iter++;

        total_f = 0.0f;

        for (int p = 0; p < joints.size(); p++) {

            Joint *j = joints[p];
            if (j == NULL) continue;

            j->f.set(0.0f, 0.0f);
            j->ld = 0.0f;
            for (int q = 0; q < j->links.size(); q++) {
                Joint *j_q = (Joint *)getItem(ItemId(-1, j->links[q]));
                if (j_q == NULL) continue;

                if (line.size() < 2) {
                    line.push_back(j_q);
                }

                ofPoint nd = (j_q->p - j->p).normalized();

                float ld = ld = (j_q->p - j->p).length() - j->links_length[q];

                total_ld += ld;

                if (ld > j->ld) {
                    j->ld = ld;
                }

                j->f += (nd * ld) * 0.2f;
            }

            j->f += (j->rotation_target - j->p) * 0.1f;

            if (j->supported) {
                ofPoint nd = (j->target - j->p).normalized();
                float ld = (j->target - j->p).length();
                total_ld += ld;
                j->f += (nd * ld) * 0.2f;
            }

            if ((update_i - j->moved_i) < 3) {
                if (!j->controlled) {
                    j->p += j->f;
                    total_f += j->f.length();
                }
            } else {
                if (!j->fixed && !j->dragged) {
                    j->p += j->f;
                    total_f += j->f.length();
                }
            }
        }
    }

    updateSupported();

    updatePolygons();
    connectJoints();
}

