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

//    c->updated_i = canvas->update_i;
    c->updated_i = sent_stamp;

    if (c->fixed) {
        return;
    }

    vector <Joint *> connected_j;
    vector<ofPoint *> connected_rel;
    canvas->getConnectedJoints(c->id, &connected_j, &connected_rel);

    Joint *sender = NULL;
    float diff;
    for (int j = 0; j < connected_j.size(); j++) {
        if (connected_j[j]->id == receiver_id) {

            sender = connected_j[j];

            ofPoint shift = (connected_j[j]->p - *connected_rel[j]) - c->p;
            //shift /= 2;

            for (InteractiveObject *v = c->front; v != NULL; v = v->next) {
                v->p += shift;
                if (v->next == c->front) break; // closed polylines
            }

            c->p = c->p + shift;

            // rotate by the angle difference
            diff = connected_j[j]->angle - c->angle;
            for (InteractiveObject *v = c->front; v != NULL; v = v->next) {
                v->p.rotate(diff, sender->p, ofPoint(0.0f, 0.0f, 1.0f));
                if (v->next == c->front) break; // closed polylines
            }
            c->angle = connected_j[j]->angle;

            break;
        }
    }

    if (sender == NULL) {
        return;
    }
    for (int j = 0; j < connected_j.size(); j++) {
        //if (connected_j[j]->id != receiver_id) {
            //float diff = sender->angle - connected_j[j]->angle;
            connected_j[j]->p.rotate(diff, sender->p, ofPoint(0.0f, 0.0f, 1.0f));
            connected_j[j]->angle = c->angle;
            connected_j[j]->angle_slider = c->angle;

            ofPoint l = *connected_rel[j] - c->p;
            l.rotate(diff, sender->p, ofPoint(0.0f, 0.0f, 1.0f));
             *connected_rel[j] = l + c->p;
        //}
    }
};


void UpdateRelative::apply(InteractiveObject *i, Canvas *canvas) {

//    i->updated_i = canvas->update_i;
    i->updated_i = sent_stamp;

    bool fixed = false;
    for (int j = 0; j < i->links.size(); j++) {
        if (canvas->lines[i->links[j]]->fixed) {
            fixed = true;
            break;
        }
    }

    for (int j = 0; j < i->links.size(); j++) {

        if (i->links[j] == receiver_id) {

            // rotate by the angle difference
//            if (!canvas->lines[receiver_id]->fixed) {
//                float diff = canvas->lines[receiver_id]->angle - i->angle;
//                for (int m = 0; m < i->links_rel.size(); m++) {
//                    if (!canvas->lines[i->links[m]]->fixed) {
//                        ofPoint l = i->p - i->links_rel[m];
//                        l.rotate(diff, i->p, ofPoint(0.0f, 0.0f, 1.0f));
//                        i->links_rel[m] = i->p - l;
//                    }
//                }
//            }

//            if (!canvas->lines[receiver_id]->fixed) {
//                i->angle = canvas->lines[receiver_id]->angle;
//                i->angle_slider = i->angle;
//            }

            if (!fixed) {
                if (receiver_id > 0 && receiver_id < canvas->lines.size()) {
                    i->p = canvas->lines[receiver_id]->p + i->links_rel[j];
                }
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
