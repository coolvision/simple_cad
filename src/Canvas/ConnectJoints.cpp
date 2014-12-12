//
//  Connect.cpp
//  simple_cad
//
//  Created by sk on 12/4/14.
//
//

#include "Canvas.h"

void Canvas::connectJoints() {

    // connect joints to the polygons
    // if they are intersecting
    for (int m = 0; m < joints.size(); m++) {
        Joint *j = joints[m];
        if (j == NULL) continue;

        if (j->connected) {
            continue;
        }

        for (int i = 0; i < lines.size(); i++) {

            Polyline *l = lines[i];

            if (l->inside(getPx(j->p))) {

                bool exists = false;
                for (int n = 0; n < l->links.size(); n++) {
                    if (l->links[n] == j->id) {
                        exists = true;
                        break;
                    }
                }
                if (!exists) {
                    l->links.push_back(j->id);
                    l->links_rel.push_back(j->p - l->p);
                }
            }
        }
    }

    for (int i = 0; i < lines.size(); i++) {

        Polyline *l = lines[i];

        for (int m = 0; m < l->links.size(); m++) {
            Joint *j_m = (Joint *)getItem(ItemId(-1, l->links[m]));

            for (int n = 0; n < l->links.size(); n++) {
                Joint *j_n = (Joint *)getItem(ItemId(-1, l->links[n]));

                if (j_n == NULL || j_m == NULL) continue;
                if (j_m->id == j_n->id) continue;

                bool exists;

                exists = false;
                for (int q = 0; q < j_m->links.size(); q++) {
                    if (j_m->links[q] == j_n->id) {
                        exists = true;
                        break;
                    }
                }
                LinkStatus ls;
                if (!exists) {
                    j_m->links.push_back(j_n->id);
                    j_m->links_length.push_back((j_m->p - j_n->p).length());
                    j_m->links_status.push_back(ls);
                    j_m->connected = true;
                }

                exists = false;
                for (int q = 0; q < j_n->links.size(); q++) {
                    if (j_n->links[q] == j_m->id) {
                        exists = true;
                        break;
                    }
                }
                if (!exists) {
                    j_n->links.push_back(j_m->id);
                    j_n->links_length.push_back((j_m->p - j_n->p).length());
                    j_n->links_status.push_back(ls);
                    j_n->connected = true;
                }
            }
        }
    }
}
