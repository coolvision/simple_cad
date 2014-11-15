//
//  Motion.cpp
//  simple_cad
//
//  Created by sk on 11/13/14.
//
//

#include "InteractiveObject.h"
#include "Canvas.h"

void LinearMotion::apply(InteractiveContainer *c, Canvas *canvas) {
    for (InteractiveObject *v = c->front; v != NULL; v = v->next) {
        apply(v, canvas);
        if (v->next == c->front) break; // closed polylines
    }
};

void LinearMotion::apply(InteractiveObject *i, Canvas *canvas) {
    i->p += v;
};

Motion *LinearMotion::getCopy() {

    LinearMotion *j = new LinearMotion();
    *j = *this;
    return (Motion *)j;
}

//==============================================================================

void UpdateRelative::apply(InteractiveContainer *c, Canvas *canvas) {

    c->updated_i = canvas->update_i;

    vector <Joint *> connected_j;
    vector<ofPoint> connected_rel;
    canvas->getConnectedJoints(c->id, &connected_j, &connected_rel);

    for (int j = 0; j < connected_j.size(); j++) {
        if (connected_j[j]->id == receiver_id) {

            ofPoint shift = (connected_j[j]->p - connected_rel[j]) - c->p;
            c->p = (connected_j[j]->p - connected_rel[j]);

            for (InteractiveObject *v = c->front; v != NULL; v = v->next) {
                v->p += shift;
                if (v->next == c->front) break; // closed polylines
            }
            break;
        }
    }
};


void UpdateRelative::apply(InteractiveObject *i, Canvas *canvas) {

    i->updated_i = canvas->update_i;

    for (int j = 0; j < i->links.size(); j++) {

        if (i->links[j] == receiver_id) {
            if (receiver_id > 0 && receiver_id < canvas->lines.size()) {
                i->p = canvas->lines[receiver_id]->p + i->links_rel[j];
            }
            break;
        }
    }
};

Motion *UpdateRelative::getCopy() {
    UpdateRelative *j = new UpdateRelative();
    *j = *this;
    return (Motion *)j;
}

//==============================================================================

void Rotation::apply(InteractiveContainer *c, Canvas *canvas) {

    c->updated_i = canvas->update_i;

    pivot = canvas->lines[0]->getItem(origin_id)->p;

    for (InteractiveObject *v = c->front; v != NULL; v = v->next) {
        v->p.rotate(angle, pivot, ofPoint(0.0f, 0.0f, 1.0f));
        if (v->next == c->front) break; // closed polylines
    }
}

void Rotation::apply(InteractiveObject *i, Canvas *canvas) {

    i->updated_i = canvas->update_i;

    pivot = canvas->lines[0]->getItem(origin_id)->p;

    ofPoint p_rotated = i->p.rotated(angle, pivot, ofPoint(0.0f, 0.0f, 1.0f));

    for (int j = 0; j < i->links_rel.size(); j++) {
        ofPoint l = i->p - i->links_rel[j];
        l.rotate(angle, pivot, ofPoint(0.0f, 0.0f, 1.0f));
        i->links_rel[j] = p_rotated - l;
    }

    i->p = p_rotated;
}

Motion *Rotation::getCopy() {

    Rotation *j = new Rotation();
    *j = *this;
    return (Motion *)j;
}
