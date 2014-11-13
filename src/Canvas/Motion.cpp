//
//  Motion.cpp
//  simple_cad
//
//  Created by sk on 11/13/14.
//
//

#include "InteractiveObject.h"

void LinearMotion::apply(InteractiveContainer *c) {
    for (InteractiveObject *v = c->front; v != NULL; v = v->next) {
        apply(v);
        if (v->next == c->front) break; // closed polylines
    }
};

void LinearMotion::apply(InteractiveObject *i) {
    i->p += v;
};

Motion *LinearMotion::getCopy() {

    LinearMotion *j = new LinearMotion();
    *j = *this;
    return (Motion *)j;
}

void Rotation::apply(InteractiveContainer *c) {




}

void Rotation::apply(InteractiveObject *i) {

    
}

Motion *Rotation::getCopy() {

    Rotation *j = new Rotation();
    *j = *this;
    return (Motion *)j;
}


